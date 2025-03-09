// SPDX-License-Identifier: Apache-2.0

//
// Created by Keren Dong on 2019-06-20.
//

#ifndef WINGCHUN_RUNNER_H
#define WINGCHUN_RUNNER_H

#include <kungfu/wingchun/strategy/runtime.h>
#include <kungfu/wingchun/strategy/strategy.h>
#include <kungfu/yijinjing/practice/apprentice.h>

namespace kungfu::wingchun::strategy {
class Runner : public yijinjing::practice::apprentice {
public:
  Runner(yijinjing::data::locator_ptr locator, const std::string &group, const std::string &name,
         longfist::enums::mode m, bool low_latency, const std::string &arguments = "");

  virtual ~Runner();

  [[nodiscard]] RuntimeContext_ptr get_context() const;

  void add_strategy(const Strategy_ptr &strategy);

  void on_exit() override;

  void on_trading_day(const event_ptr &event, int64_t daytime) override;

protected:
  void react() override;

  void on_react() override;

  void on_start() override;

  void on_active() override;

  virtual RuntimeContext_ptr make_context();

  virtual void pre_start();

  virtual void post_start();

  virtual void pre_stop();

  virtual void post_stop();

private:
  bool positions_requested_ = false;
  bool broker_states_requested_ = false;
  bool positions_set_;
  bool started_;
  std::vector<Strategy_ptr> strategies_ = {};
  RuntimeContext_ptr context_;
  const std::string arguments_;

  void prepare(const event_ptr &event);
  void inspect_channel(const event_ptr &event);

  template <typename OnMethod = void (Strategy::*)(Context_ptr &)> void invoke(OnMethod method) {
    auto context = std::dynamic_pointer_cast<Context>(context_);
    for (const auto &strategy : strategies_) {
      (*strategy.*method)(context);
    }
  }

  template <typename TradingData, typename OnMethod = void (Strategy::*)(Context_ptr &, const TradingData &)>
  void invoke(OnMethod method, const TradingData &data) {
    auto context = std::dynamic_pointer_cast<Context>(context_);
    for (const auto &strategy : strategies_) {
      (*strategy.*method)(context, data);
    }
  }

  // 遍历所有的策略，调用注册的回调函数
  template <typename TradingData, typename OnMethod = void (Strategy::*)(Context_ptr &, const TradingData &,
                                                                         const kungfu::yijinjing::data::location_ptr &)>
  void invoke(OnMethod method, const TradingData &data, const kungfu::yijinjing::data::location_ptr &location) {
    auto context = std::dynamic_pointer_cast<Context>(context_);
    for (const auto &strategy : strategies_) {
      (*strategy.*method)(context, data, location);
    }
  }

  template <typename OnMethod = void (Strategy::*)(Context_ptr &, uint32_t, const std::vector<uint8_t> &, uint32_t,
                                                   const kungfu::yijinjing::data::location_ptr &)>
  void invoke(OnMethod method, uint32_t msg_type, const std::vector<uint8_t> &data, uint32_t length,
              const kungfu::yijinjing::data::location_ptr &location) {
    auto context = std::dynamic_pointer_cast<Context>(context_);
    for (const auto &strategy : strategies_) {
      (*strategy.*method)(context, msg_type, data, length, location);
    }
  }

  class BookListener : public wingchun::book::BookListener {
  public:
    explicit BookListener(Runner &runner);

    ~BookListener() = default;

    void on_position_sync_reset(const wingchun::book::Book &old_book, const wingchun::book::Book &new_book) override;

    void on_asset_sync_reset(const longfist::types::Asset &old_asset, const longfist::types::Asset &new_asset) override;

    void on_asset_margin_sync_reset(const longfist::types::AssetMargin &old_asset_margin,
                                    const longfist::types::AssetMargin &new_asset_margin) override;

  private:
    Runner &runner_;
  };
  DECLARE_PTR(BookListener);
};

} // namespace kungfu::wingchun::strategy

#endif // WINGCHUN_RUNNER_H
