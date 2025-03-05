// SPDX-License-Identifier: Apache-2.0

//
// Created by Keren Dong on 2019-06-01.
//

#ifndef KUNGFU_IO_H
#define KUNGFU_IO_H

#include <kungfu/yijinjing/journal/journal.h>
#include <kungfu/yijinjing/nanomsg/socket.h>

namespace kungfu::yijinjing {
FORWARD_DECLARE_CLASS_PTR(session)

/*
 * 管理了publisher、observer、url_factory，
 * 可以通过open_writer打开默认目录下的writer
 * 通过open_reader打开相应路径下的reader。
*/
class io_device : public resource {
public:
  io_device(data::location_ptr home, bool low_latency, bool lazy);

  virtual ~io_device() = default;

  bool is_usable() override { return publisher_ and observer_ and publisher_->is_usable() and observer_->is_usable(); }

  void setup() override {
    publisher_->setup();
    observer_->setup();
  }

  [[nodiscard]] const data::locator_ptr &get_locator() const { return home_->locator; }

  [[nodiscard]] const data::location_ptr &get_home() const { return home_; }

  [[nodiscard]] const data::location_ptr &get_live_home() const { return live_home_; }

  [[nodiscard]] bool is_low_latency() const { return low_latency_; }

  journal::reader_ptr open_reader_to_subscribe();

  [[maybe_unused]] journal::reader_ptr open_reader(const data::location_ptr &location, uint32_t dest_id);

  journal::writer_ptr open_writer(uint32_t dest_id);

  journal::writer_ptr open_writer_at(const data::location_ptr &location, uint32_t dest_id);

  [[maybe_unused]] nanomsg::socket_ptr connect_socket(const data::location_ptr &location, const nanomsg::protocol &p,
                                                      int timeout = 0);

  [[maybe_unused]] nanomsg::socket_ptr bind_socket(const nanomsg::protocol &p, int timeout = 0);

  [[nodiscard]] nanomsg::url_factory_ptr get_url_factory() const { return url_factory_; }

  [[nodiscard]] publisher_ptr get_publisher() { return publisher_; }

  [[nodiscard]] observer_ptr get_observer() { return observer_; }

protected:
  data::location_ptr home_;
  data::location_ptr live_home_;
  const bool low_latency_;
  const bool lazy_;
  nanomsg::url_factory_ptr url_factory_;
  publisher_ptr publisher_;
  observer_ptr observer_;
};

DECLARE_PTR(io_device)

class io_device_master : public io_device {
public:
  io_device_master(data::location_ptr home, bool low_latency);
};

DECLARE_PTR(io_device_master)

class io_device_client : public io_device {
public:
  io_device_client(data::location_ptr home, bool low_latency);

  bool is_usable() override;

  void setup() override;
};

DECLARE_PTR(io_device_client)

class io_device_console : public io_device {
public:
  io_device_console(data::location_ptr home, int32_t console_width, int32_t console_height);

  [[maybe_unused]] void trace(int64_t begin_time, int64_t end_time, bool in, bool out, std::string csv);

  [[maybe_unused]] void show(int64_t begin_time, int64_t end_time, bool in, bool out, std::string csv);

private:
  int32_t console_width_;
  int32_t console_height_;
};

DECLARE_PTR(io_device_console)

void handle_sql_error(int rc, const std::string &error_tip);
void ensure_sqlite_initilize();
void ensure_sqlite_shutdown();
} // namespace kungfu::yijinjing
#endif // KUNGFU_IO_H
