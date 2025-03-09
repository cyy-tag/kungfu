// SPDX-License-Identifier: Apache-2.0

//
// Created by Keren Dong on 2020/3/10.
//

#ifndef WINGCHUN_BROKER_H
#define WINGCHUN_BROKER_H

#include <kungfu/longfist/longfist.h>
#include <kungfu/wingchun/common.h>
#include <kungfu/yijinjing/io.h>
#include <kungfu/yijinjing/log.h>
#include <kungfu/yijinjing/practice/apprentice.h>

namespace kungfu::wingchun::broker {

FORWARD_DECLARE_CLASS_PTR(BrokerVendor)
FORWARD_DECLARE_CLASS_PTR(BrokerService)

// 行情代理供应商 继承apprentice
class BrokerVendor : public yijinjing::practice::apprentice {
public:
  typedef yijinjing::data::locator_ptr locator_ptr;
  typedef yijinjing::data::location_ptr location_ptr;
  typedef longfist::enums::BrokerState BrokerState;

  BrokerVendor(location_ptr location, bool low_latency);

  void on_exit() override;

protected:
  virtual BrokerService_ptr get_service() = 0;

  void on_start() override;

private:
  void notify_broker_state();
};

// 代理服务, 提供供应商的功能
class BrokerService {
public:
  typedef longfist::enums::BrokerState BrokerState;

  explicit BrokerService(BrokerVendor &vendor);

  virtual ~BrokerService() = default;

  virtual void on_start();

  virtual void on_exit();

  virtual void on_trading_day(const event_ptr &event, int64_t daytime);

  [[nodiscard]] int64_t now() const;

  [[nodiscard]] BrokerState get_state();

  [[nodiscard]] std::string get_runtime_folder();

  [[nodiscard]] const std::string &get_config() const;

  [[maybe_unused]] [[nodiscard]] const std::string &get_risk_setting() const;

  [[nodiscard]] const yijinjing::data::location_ptr &get_home() const;

  [[nodiscard]] uint32_t get_home_uid() const;

  [[nodiscard]] yijinjing::io_device_ptr get_io_device() const;

  [[nodiscard]] yijinjing::journal::writer_ptr get_writer(uint32_t dest_id) const;

  [[nodiscard]] bool has_writer(uint32_t dest_id) const;

  template <typename DataType> void write_to(DataType &data, uint32_t dest_id = yijinjing::data::location::PUBLIC) {
    vendor_.write_to(now(), data, dest_id);
  }

  [[nodiscard]] const yijinjing::cache::bank &get_state_bank() const;

  [[maybe_unused]] [[nodiscard]] bool check_if_stored_instruments(const std::string &trading_day) const;

  [[maybe_unused]] void record_stored_instruments_trading_day(const std::string &trading_day) const;

  void add_timer(int64_t nanotime, const std::function<void(const event_ptr &)> &callback);

  void add_time_interval(int64_t nanotime, const std::function<void(const event_ptr &)> &callback);

  void update_broker_state(BrokerState state);

  [[maybe_unused]] void request_deregister() { vendor_.request_deregister(); }

  [[maybe_unused]] [[nodiscard]] BrokerVendor &get_vendor() const { return vendor_; }

  [[maybe_unused]] uint32_t request_band(const std::string &band_name) { return vendor_.request_band(band_name); }

protected:
  volatile BrokerState state_;

private:
  BrokerVendor &vendor_;
};

} // namespace kungfu::wingchun::broker

#endif // WINGCHUN_BROKER_H
