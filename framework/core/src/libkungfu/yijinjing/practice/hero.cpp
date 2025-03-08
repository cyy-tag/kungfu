// SPDX-License-Identifier: Apache-2.0

//
// Created by Keren Dong on 2019-06-15.
//

#include <kungfu/common.h>
#include <kungfu/yijinjing/journal/journal.h>
#include <kungfu/yijinjing/log.h>
#include <kungfu/yijinjing/nanomsg/socket.h>
#include <kungfu/yijinjing/practice/hero.h>
#include <kungfu/yijinjing/time.h>
#include <kungfu/yijinjing/util/os.h>

using namespace kungfu::rx;
using namespace kungfu::longfist::enums;
using namespace kungfu::longfist::types;
using namespace kungfu::yijinjing;
using namespace kungfu::yijinjing::cache;
using namespace kungfu::yijinjing::data;
using namespace kungfu::yijinjing::journal;
using namespace kungfu::yijinjing::nanomsg;

namespace kungfu::yijinjing::practice {

inline std::string encode(const io_device_ptr &io_device) {
  return fmt::format("{:08x}", io_device->get_live_home()->uid);
}

hero::hero(io_device_ptr io_device)
    : begin_time_(time::now_in_nano()), end_time_(INT64_MAX),
      master_home_location_(make_system_location("master", "master", io_device->get_locator())),
      master_cmd_location_(make_system_location("master", encode(io_device), io_device->get_locator())),
      cached_home_location_(make_system_location("service", "cached", io_device->get_locator())),
      ledger_home_location_(make_system_location("service", "ledger", io_device->get_locator())),
      io_device_(std::move(io_device)), now_(0) {

  os::handle_os_signals(this); // 每个进程只构造一个, 判断是否重复构造
  add_location(0, get_io_device()->get_home());
  add_location(0, master_home_location_);
  add_location(0, master_cmd_location_);
  add_location(0, cached_home_location_);
  add_location(0, ledger_home_location_);
  reader_ = io_device_->open_reader_to_subscribe();
}

hero::~hero() {
  writers_.clear();
  reader_.reset();
  io_device_.reset();
  ensure_sqlite_shutdown();
}

bool hero::is_usable() { return io_device_->is_usable(); }

void hero::setup() {
  io_device_->setup();
  //rxcpp 响应函数式编程, 创建一个返回event_ptr监听事件 events_
  events_ = observable<>::create<event_ptr>([this](auto &s) { delegate_produce(this, s); }) | holdon();
  react();
  live_ = true;
}

void hero::step() {
  continual_ = false;
  events_.connect(cs_);
}

// master启动入口
void hero::run() {
  SPDLOG_INFO("[{:08x}] {} running", get_home_uid(), get_home_uname());
  SPDLOG_TRACE("from {} until {}", time::strftime(begin_time_), time::strftime(end_time_));
  // 设置好订阅信息
  setup();
  continual_ = true;
  // 开启主循环
  events_.connect(cs_);
  on_exit();
  SPDLOG_INFO("[{:08x}] {} done", get_home_uid(), get_home_uname());
}

bool hero::is_live() const { return live_; }

void hero::signal_stop() { live_ = false; }

int64_t hero::now() const { return now_; }

void hero::set_begin_time(int64_t begin_time) { begin_time_ = begin_time; }

void hero::set_end_time(int64_t end_time) { end_time_ = end_time; }

const locator_ptr &hero::get_locator() const { return io_device_->get_locator(); }

io_device_ptr hero::get_io_device() const { return io_device_; }

const location_ptr &hero::get_home() const { return get_io_device()->get_home(); }

uint32_t hero::get_home_uid() const { return get_io_device()->get_home()->uid; }

const std::string &hero::get_home_uname() const { return get_io_device()->get_home()->uname; }

[[maybe_unused]] const location_ptr &hero::get_live_home() const { return get_io_device()->get_live_home(); }

uint32_t hero::get_live_home_uid() const { return get_io_device()->get_live_home()->uid; }

[[maybe_unused]] reader_ptr hero::get_reader() const { return reader_; }

bool hero::has_writer(uint32_t dest_id) const { return writers_.find(dest_id) != writers_.end(); }

writer_ptr hero::get_writer(uint32_t dest_id) const {
  if (writers_.find(dest_id) == writers_.end()) {
    SPDLOG_ERROR("no writer for {}", get_location_uname(dest_id));
  }
  return writers_.at(dest_id);
}

[[maybe_unused]] const WriterMap &hero::get_writers() const { return writers_; }

bool hero::has_location(uint32_t uid) const { return locations_.find(uid) != locations_.end(); }

location_ptr hero::get_location(uint32_t uid) const {
  if (not has_location(uid)) {
    SPDLOG_ERROR("no location {} in locations_", uid);
  }

  assert(has_location(uid));
  return locations_.at(uid);
}

std::string hero::get_location_uname(uint32_t uid) const {
  if (uid == location::PUBLIC) {
    return "public";
  }
  if (uid == location::SYNC) {
    return "sync";
  }
  if (not has_location(uid)) {
    return fmt::format("{:08x}", uid);
  }
  return get_location(uid)->uname;
}

bool hero::is_location_live(uint32_t uid) const { return registry_.find(uid) != registry_.end(); }

bool hero::has_channel(uint32_t source, uint32_t dest) const {
  return has_channel(make_source_dest_hash(source, dest));
}

bool hero::has_channel(uint64_t hash) const { return channels_.find(hash) != channels_.end(); }

[[maybe_unused]] const longfist::types::Channel &hero::get_channel(uint32_t source, uint32_t dest) const {
  return get_channel(make_source_dest_hash(source, dest));
}

const Channel &hero::get_channel(uint64_t hash) const {
  assert(has_channel(hash));
  return channels_.at(hash);
}

[[maybe_unused]] const std::unordered_map<uint64_t, longfist::types::Channel> &hero::get_channels() const {
  return channels_;
}

[[maybe_unused]] bool hero::has_band(uint32_t source, uint32_t dest) const {
  return has_band(make_source_dest_hash(source, dest));
}

bool hero::has_band(uint64_t hash) const { return bands_.find(hash) != bands_.end(); }

[[maybe_unused]] const longfist::types::Band &hero::get_band(uint32_t source, uint32_t dest) const {
  return get_band(make_source_dest_hash(source, dest));
}

const longfist::types::Band &hero::get_band(uint64_t hash) const {
  assert(has_band(hash));
  return bands_.at(hash);
}

[[maybe_unused]] const std::unordered_map<uint64_t, longfist::types::Band> &hero::get_bands() const { return bands_; }

const std::unordered_map<uint32_t, longfist::types::Register> &hero::get_registry() const { return registry_; }

const std::unordered_map<uint32_t, yijinjing::data::location_ptr> &hero::get_locations() const { return locations_; }

void hero::on_notify() {}

void hero::on_exit() { SPDLOG_INFO("default on_exit"); }

location_ptr hero::get_ledger_home_location() const { return ledger_home_location_; }

location_ptr hero::get_master_home_location() const { return master_home_location_; }

location_ptr hero::get_master_cmd_location() const { return master_cmd_location_; }

location_ptr hero::get_cached_home_location() const { return cached_home_location_; }

const rx::connectable_observable<event_ptr> &hero::get_events() const { return events_; }

uint64_t hero::make_source_dest_hash(uint32_t source_id, uint32_t dest_id) {
  return uint64_t(source_id) << 32u | uint64_t(dest_id);
}

bool hero::check_location_exists(uint32_t source_id, uint32_t dest_id) const {
  if (not has_location(source_id)) {
    SPDLOG_ERROR("source_id {}, {} does not exist", source_id, get_location_uname(source_id));
    return false;
  }
  if (dest_id != location::PUBLIC and dest_id != location::SYNC and not has_location(dest_id)) {
    SPDLOG_ERROR("dest_id {}, {} does not exist", dest_id, get_location_uname(dest_id));
    return false;
  }
  return true;
}

bool hero::check_location_live(uint32_t source_id, uint32_t dest_id) const {
  if (not check_location_exists(source_id, dest_id)) {
    return false;
  }
  if (!is_location_live(source_id)) {
    SPDLOG_ERROR("{} is not live", get_location_uname(source_id));
    return false;
  }
  if (!is_location_live(dest_id)) {
    SPDLOG_ERROR("{} is not live", get_location_uname(dest_id));
    return false;
  }
  return true;
}

void hero::add_location(int64_t, const location_ptr &location) { locations_.try_emplace(location->uid, location); }

void hero::add_location(int64_t trigger_time, const Location &location) {
  add_location(trigger_time, data::location::make_shared(location, get_locator()));
}

void hero::remove_location(int64_t trigger_time, uint32_t location_uid) { locations_.erase(location_uid); }

void hero::register_location(int64_t, const Register &register_data) {
  uint32_t location_uid = register_data.location_uid;
  auto result = registry_.try_emplace(location_uid, register_data);
  if (result.second) {
    SPDLOG_TRACE("location [{:08x}] {} up", location_uid, get_location_uname(location_uid));
  }
}

void hero::deregister_location(int64_t, const uint32_t location_uid) {
  auto result = registry_.erase(location_uid);
  if (result) {
    SPDLOG_TRACE("location [{:08x}] {} down", location_uid, get_location_uname(location_uid));
  }
}

void hero::register_channel(int64_t, const Channel &channel) {
  [[maybe_unused]] uint64_t channel_uid = make_source_dest_hash(channel.source_id, channel.dest_id);
  auto result = channels_.try_emplace(channel_uid, channel);
  if (result.second) {
    auto source_uname = get_location_uname(channel.source_id);
    auto dest_uname = get_location_uname(channel.dest_id);
    SPDLOG_TRACE("channel [{:08x}] {} -> {} up", channel_uid, source_uname, dest_uname);
  }
}

void hero::deregister_channel(uint32_t source_id) {
  auto channel_it = channels_.begin();
  while (channel_it != channels_.end()) {
    if (channel_it->second.source_id == source_id) {
      const auto &channel_uid = channel_it->first;
      const auto &channel = channel_it->second;
      auto source_uname = get_location_uname(channel.source_id);
      auto dest_uname = get_location_uname(channel.dest_id);
      SPDLOG_TRACE("channel [{:08x}] {} -> {} down", channel_uid, source_uname, dest_uname);
      channel_it = channels_.erase(channel_it);
      continue;
    }
    channel_it++;
  }
}

void hero::register_band(int64_t, const Band &band) {
  uint64_t band_uid = make_source_dest_hash(band.source_id, band.dest_id);
  auto result = bands_.try_emplace(band_uid, band);
  if (result.second) {
    auto source_uname = get_location_uname(band.source_id);
    auto dest_uname = get_location_uname(band.dest_id);
    SPDLOG_TRACE("band [{:08x}] {} -> {} up", band_uid, source_uname, dest_uname);
  }
}

void hero::deregister_band(uint32_t source_id) {
  auto band_it = bands_.begin();
  while (band_it != bands_.end()) {
    if (band_it->second.source_id == source_id) {
      const auto &band_uid = band_it->first;
      const auto &band = band_it->second;
      auto source_uname = get_location_uname(band.source_id);
      auto dest_uname = get_location_uname(band.dest_id);
      SPDLOG_TRACE("band [{:08x}] {} -> {} down", band_uid, source_uname, dest_uname);
      band_it = bands_.erase(band_it);
      continue;
    }
    band_it++;
  }
}

void hero::require_read_from(int64_t trigger_time, uint32_t dest_id, uint32_t source_id, int64_t from_time) {
  do_require_read_from<RequestReadFrom>(get_writer(dest_id), trigger_time, dest_id, source_id, from_time);
}

void hero::require_read_from_public(int64_t trigger_time, uint32_t dest_id, uint32_t source_id, int64_t from_time) {
  do_require_read_from<RequestReadFromPublic>(get_writer(dest_id), trigger_time, dest_id, source_id, from_time);
}

void hero::require_read_from_sync(int64_t trigger_time, uint32_t dest_id, uint32_t source_id, int64_t from_time) {
  do_require_read_from<RequestReadFromSync>(get_writer(dest_id), trigger_time, dest_id, source_id, from_time);
}

void hero::require_write_to(int64_t trigger_time, uint32_t source_id, uint32_t dest_id) {
  if (not check_location_exists(source_id, dest_id)) {
    return;
  }
  auto writer = get_writer(source_id);
  RequestWriteTo &msg = writer->open_data<RequestWriteTo>(trigger_time);
  msg.dest_id = dest_id;
  writer->close_data();
}

void hero::require_write_to_band(int64_t trigger_time, uint32_t source_id,
                                 const yijinjing::data::location_ptr &location) const {
  auto writer = get_writer(source_id);
  RequestWriteToBand msg = {};
  location->to<RequestWriteToBand>(msg);
  writer->write(trigger_time, msg);
}

//核心函数, 主循环从reader中获取新的信息数据
//并调用on_next发送给所有订阅的观察者
void hero::produce(const rx::subscriber<event_ptr> &sb) {
  try {
    do {
      live_ = drain(sb) && live_;
      //处理完消息事件
      on_active();
      //处理定时器事件
    } while (continual_ and live_);
  } catch (...) {
    live_ = false;
    sb.on_error(std::current_exception());
  }
  if (not live_) {
    sb.on_completed(); // 完成退出通知
  }
}

bool hero::drain(const rx::subscriber<event_ptr> &sb) {
  if (io_device_->get_home()->mode == mode::LIVE and io_device_->get_observer()->wait()) {
    //盘中实时运行模式, 且是阻塞模式,等待消息通知
    const std::string &notice = io_device_->get_observer()->get_notice();
    now_ = time::now_in_nano();
    if (notice.length() > 2) {
      //socket通信采用json格式信息，nanomsg_json和frame一样，都继承自event
      sb.on_next(std::make_shared<nanomsg_json>(notice));
    } else {
      on_notify();
    }
  }
  //然后reader判断journal上的数据是否可读
  //如果可读，就读取frame，调用on_next发送到观察者
  while (live_ and reader_->data_available()) {
    if (reader_->current_frame()->gen_time() <= end_time_) {
      int64_t frame_time = reader_->current_frame()->gen_time();
      if (frame_time > now_) {
        now_ = frame_time;
      }
      //获取到新的数据，发送当前frame的地址
      sb.on_next(reader_->current_frame());
      on_frame();
      reader_->next(); //获取下一个可读的frame
    } else {
      SPDLOG_INFO("reached journal end {}", time::strftime(reader_->current_frame()->gen_time()));
      return false;
    }
  }
  //回放或回测模式下, reader没有可读数据，代表回测结束
  if (get_io_device()->get_home()->mode != mode::LIVE and not reader_->data_available()) {
    SPDLOG_INFO("reached journal end {}", time::strftime(reader_->current_frame()->gen_time()));
    return false;
  }
  return true;
}

void hero::delegate_produce(hero *instance, const rx::subscriber<event_ptr> &subscriber) {
#ifdef _WINDOWS
  __try {
    instance->produce(subscriber);
  } __except (util::print_stack_trace(GetExceptionInformation())) {
  }
#else
  instance->produce(subscriber);
#endif
}
} // namespace kungfu::yijinjing::practice
