// SPDX-License-Identifier: Apache-2.0

//
// Created by Keren Dong on 2019-06-15.
//

#ifndef KUNGFU_HERO_H
#define KUNGFU_HERO_H

#include <kungfu/longfist/longfist.h>
#include <kungfu/yijinjing/index/session.h>
#include <kungfu/yijinjing/io.h>
#include <kungfu/yijinjing/journal/journal.h>
#include <kungfu/yijinjing/log.h>
#include <kungfu/yijinjing/time.h>

#ifndef KUNGFU_SETUP_LOG
#define KUNGFU_SETUP_LOG() kungfu::yijinjing::log::copy_log_settings(get_home(), get_home()->name)
#endif // KUNGFU_SETUP_LOG

namespace kungfu::yijinjing::practice {

inline yijinjing::data::location_ptr make_system_location(const std::string &group, const std::string &name,
                                                          const data::locator_ptr &locator) {
  return yijinjing::data::location::make_shared(longfist::enums::mode::LIVE, longfist::enums::category::SYSTEM, group,
                                                name, locator);
}

typedef std::unordered_map<uint32_t, yijinjing::journal::writer_ptr> WriterMap;

// hero 是个单例, 每个进程只能构造一个
// 否则会抛出异常
// 通过io_device构造, 管理底层的journal、reader、writer
class hero : public resource {
public:
  explicit hero(yijinjing::io_device_ptr io_device);

  virtual ~hero();

  bool is_usable() override;

  void setup() override;

  void step();

  void run();

  bool is_live() const;

  void signal_stop();

  int64_t now() const;

  void set_begin_time(int64_t begin_time);

  void set_end_time(int64_t end_time);

  [[nodiscard]] const data::locator_ptr &get_locator() const;

  [[nodiscard]] yijinjing::io_device_ptr get_io_device() const;

  const yijinjing::data::location_ptr &get_home() const;

  uint32_t get_home_uid() const;

  const std::string &get_home_uname() const;

  [[maybe_unused]] const yijinjing::data::location_ptr &get_live_home() const;

  uint32_t get_live_home_uid() const;

  [[maybe_unused]] [[nodiscard]] yijinjing::journal::reader_ptr get_reader() const;

  bool has_writer(uint32_t dest_id) const;

  [[nodiscard]] yijinjing::journal::writer_ptr get_writer(uint32_t dest_id) const;

  [[maybe_unused]] [[nodiscard]] const WriterMap &get_writers() const;

  bool has_location(uint32_t uid) const;

  [[nodiscard]] yijinjing::data::location_ptr get_location(uint32_t uid) const;

  [[nodiscard]] std::string get_location_uname(uint32_t uid) const;

  bool is_location_live(uint32_t uid) const;

  bool has_channel(uint32_t source, uint32_t dest) const;

  bool has_channel(uint64_t hash) const;

  [[maybe_unused]] const longfist::types::Channel &get_channel(uint32_t source, uint32_t dest) const;

  const longfist::types::Channel &get_channel(uint64_t hash) const;

  [[maybe_unused]] const std::unordered_map<uint64_t, longfist::types::Channel> &get_channels() const;

  [[maybe_unused]] bool has_band(uint32_t source, uint32_t dest) const;

  bool has_band(uint64_t hash) const;

  [[maybe_unused]] const longfist::types::Band &get_band(uint32_t source, uint32_t dest) const;

  const longfist::types::Band &get_band(uint64_t hash) const;

  [[maybe_unused]] const std::unordered_map<uint64_t, longfist::types::Band> &get_bands() const;

  const std::unordered_map<uint32_t, longfist::types::Register> &get_registry() const;

  const std::unordered_map<uint32_t, yijinjing::data::location_ptr> &get_locations() const;

  virtual void on_notify();

  virtual void on_exit();

  void request_deregister() {
    continual_ = false;
    live_ = false;
  }

  yijinjing::data::location_ptr get_ledger_home_location() const;

  yijinjing::data::location_ptr get_master_home_location() const;

  yijinjing::data::location_ptr get_master_cmd_location() const;

  yijinjing::data::location_ptr get_cached_home_location() const;

  const rx::connectable_observable<event_ptr> &get_events() const;

protected:
  int64_t begin_time_;
  int64_t end_time_;
  yijinjing::journal::reader_ptr reader_;
  WriterMap writers_ = {};
  std::unordered_map<uint64_t, longfist::types::Band> bands_ = {};
  std::unordered_map<uint64_t, longfist::types::Channel> channels_ = {};
  std::unordered_map<uint32_t, yijinjing::data::location_ptr> locations_ = {};
  std::unordered_map<uint32_t, longfist::types::Register> registry_ = {};
  rx::connectable_observable<event_ptr> events_ = {};

  const yijinjing::data::location_ptr master_home_location_;
  const yijinjing::data::location_ptr master_cmd_location_;
  const yijinjing::data::location_ptr cached_home_location_;
  const yijinjing::data::location_ptr ledger_home_location_;

  static uint64_t make_source_dest_hash(uint32_t source_id, uint32_t dest_id);

  bool check_location_exists(uint32_t source_id, uint32_t dest_id) const;

  bool check_location_live(uint32_t source_id, uint32_t dest_id) const;

  void add_location(int64_t trigger_time, const yijinjing::data::location_ptr &location);

  void add_location(int64_t trigger_time, const longfist::types::Location &location);

  [[maybe_unused]] void remove_location(int64_t trigger_time, uint32_t location_uid);

  void register_location(int64_t trigger_time, const longfist::types::Register &register_data);

  void deregister_location(int64_t trigger_time, uint32_t location_uid);

  void register_channel([[maybe_unused]] int64_t trigger_time, const longfist::types::Channel &channel);

  void deregister_channel(uint32_t source_id);

  void register_band(int64_t trigger_time, const longfist::types::Band &band);

  void deregister_band(uint32_t source_id);

  void require_read_from(int64_t trigger_time, uint32_t dest_id, uint32_t source_id, int64_t from_time);

  void require_read_from_public(int64_t trigger_time, uint32_t dest_id, uint32_t source_id, int64_t from_time);

  void require_read_from_sync(int64_t trigger_time, uint32_t dest_id, uint32_t source_id, int64_t from_time);

  void require_write_to(int64_t trigger_time, uint32_t source_id, uint32_t dest_id);

  void require_write_to_band(int64_t trigger_time, uint32_t source_id,
                             const yijinjing::data::location_ptr &location) const;

  virtual void react() = 0;

  virtual void on_active() = 0;

  virtual void on_frame() = 0;

  static constexpr auto feed_profile_data = [](const event_ptr &event, auto &receiver) {
    boost::hana::for_each(longfist::ProfileDataTypes, [&](auto it) {
      using DataType = typename decltype(+boost::hana::second(it))::type;
      if (DataType::tag == event->msg_type()) {
        receiver << typed_event_ptr<DataType>(event);
      }
    });
  };

  static constexpr auto feed_state_data = [](const event_ptr &event, auto &receiver) {
    boost::hana::for_each(longfist::StateDataTypes, [&](auto it) {
      using DataType = typename decltype(+boost::hana::second(it))::type;
      if (DataType::tag == event->msg_type()) {
        receiver << typed_event_ptr<DataType>(event);
      }
    });
  };

  static constexpr auto feed_trading_data = [](const event_ptr &event, auto &receiver) {
    boost::hana::for_each(longfist::TradingDataTypes, [&](auto it) {
      using DataType = typename decltype(+boost::hana::second(it))::type;
      if (DataType::tag == event->msg_type()) {
        receiver << typed_event_ptr<DataType>(event);
      }
    });
  };

private:
  yijinjing::io_device_ptr io_device_;
  rx::composite_subscription cs_;
  int64_t now_;
  volatile bool continual_ = true;
  volatile bool live_ = false;

  void produce(const rx::subscriber<event_ptr> &sb);

  bool drain(const rx::subscriber<event_ptr> &sb);

  template <typename T>
  std::enable_if_t<T::reflect> do_require_read_from(yijinjing::journal::writer_ptr &&writer, int64_t trigger_time,
                                                    uint32_t dest_id, uint32_t source_id, int64_t from_time) {
    if (check_location_exists(source_id, dest_id)) {
      T &msg = writer->template open_data<T>(trigger_time);
      msg.source_id = source_id;
      msg.from_time = from_time;
      writer->close_data();
    }
  }

  static void delegate_produce(hero *instance, const rx::subscriber<event_ptr> &subscriber);
};
} // namespace kungfu::yijinjing::practice
#endif // KUNGFU_HERO_H
