// SPDX-License-Identifier: Apache-2.0

#include "py-wingchun.h"

#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <kungfu/wingchun/strategy/context.h>
#include <kungfu/wingchun/strategy/runner.h>

//策略类python版本封装
using namespace kungfu::longfist;
using namespace kungfu::longfist::types;
using namespace kungfu::longfist::enums;
using namespace kungfu::yijinjing;
using namespace kungfu::yijinjing::data;
using namespace kungfu::yijinjing::journal;
using namespace kungfu::wingchun;
using namespace kungfu::wingchun::book;

namespace py = pybind11;

namespace kungfu::wingchun::pybind {

class PyRunner : public strategy::Runner {
public:
  using strategy::Runner::Runner;

  void on_trading_day(const event_ptr &event, int64_t daytime) override {
    PYBIND11_OVERLOAD(void, strategy::Runner, on_trading_day, event, daytime);
  }
};

class PyStrategy : public strategy::Strategy {
public:
  using strategy::Strategy::Strategy; // Inherit constructors

  void pre_start(strategy::Context_ptr &context) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, pre_start, context);
  }

  void post_start(strategy::Context_ptr &context) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, post_start, context);
  }

  void pre_stop(strategy::Context_ptr &context) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, pre_stop, context);
  }

  void post_stop(strategy::Context_ptr &context) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, post_stop, context);
  }

  void on_trading_day(strategy::Context_ptr &context, int64_t daytime) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_trading_day, context, daytime);
  }

  void on_quote(strategy::Context_ptr &context, const Quote &quote,
                const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_quote, context, quote, location);
  }

  void on_tree(strategy::Context_ptr &context, const Tree &tree,
               const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_tree, context, tree, location);
  }

  void on_bar(strategy::Context_ptr &context, const Bar &bar,
              const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_bar, context, bar, location);
  }

  void on_entrust(strategy::Context_ptr &context, const Entrust &entrust,
                  const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_entrust, context, entrust, location);
  }

  void on_transaction(strategy::Context_ptr &context, const Transaction &transaction,
                      const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_transaction, context, transaction, location);
  }

  void on_order(strategy::Context_ptr &context, const Order &order,
                const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_order, context, order, location);
  }

  void on_order_action_error(strategy::Context_ptr &context, const OrderActionError &error,
                             const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_order_action_error, context, error, location);
  }

  void on_trade(strategy::Context_ptr &context, const Trade &trade,
                const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_trade, context, trade, location);
  }

  void on_deregister(strategy::Context_ptr &context, const Deregister &deregister,
                     const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_deregister, context, deregister, location);
  }

  void on_broker_state_change(strategy::Context_ptr &context, const BrokerStateUpdate &brokerStateUpdate,
                              const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_broker_state_change, context, brokerStateUpdate, location);
  }

  void on_history_order(strategy::Context_ptr &context, const HistoryOrder &history_order,
                        const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_history_order, context, history_order, location);
  }

  void on_history_trade(strategy::Context_ptr &context, const HistoryTrade &history_trade,
                        const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_history_trade, context, history_trade, location);
  }

  void on_req_history_order_error(strategy::Context_ptr &context, const RequestHistoryOrderError &error,
                                  const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_req_history_order_error, context, error, location);
  }

  void on_req_history_trade_error(strategy::Context_ptr &context, const RequestHistoryTradeError &error,
                                  const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_req_history_trade_error, context, error, location);
  }

  void on_position_sync_reset(strategy::Context_ptr &context, const Book &old_book, const Book &new_book) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_position_sync_reset, context, old_book, new_book);
  }

  void on_asset_sync_reset(strategy::Context_ptr &context, const Asset &old_asset, const Asset &new_asset) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_asset_sync_reset, context, old_asset, new_asset);
  }

  void on_asset_margin_sync_reset(strategy::Context_ptr &context, const AssetMargin &old_asset_margin,
                                  const AssetMargin &new_asset_margin) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_asset_margin_sync_reset, context, old_asset_margin,
                      new_asset_margin);
  }

  void on_custom_data(strategy::Context_ptr &context, uint32_t msg_type, const std::vector<uint8_t> &data,
                      uint32_t length, const kungfu::yijinjing::data::location_ptr &location) override {
    PYBIND11_OVERLOAD(void, strategy::Strategy, on_custom_data, context, msg_type, data, length, location);
  }
};

void bind_strategy(pybind11::module &m) {

  py::class_<strategy::Runner, PyRunner, kungfu::yijinjing::practice::apprentice, std::shared_ptr<strategy::Runner>>(
      m, "Runner")
      .def(py::init<kungfu::yijinjing::data::locator_ptr, const std::string &, const std::string &,
                    longfist::enums::mode, bool, const std::string &>())
      .def_property_readonly("context", &strategy::Runner::get_context)
      .def("set_begin_time", &strategy::Runner::set_begin_time)
      .def("set_end_time", &strategy::Runner::set_end_time)
      .def("now", &strategy::Runner::now)
      .def("run", &strategy::Runner::run)
      .def("setup", &strategy::Runner::setup)
      .def("step", &strategy::Runner::step)
      .def("on_trading_day", &strategy::Runner::on_trading_day)
      .def("on_exit", &strategy::Runner::on_exit)
      .def("add_strategy", &strategy::Runner::add_strategy);

  py::class_<strategy::Context, std::shared_ptr<strategy::Context>>(m, "Context")
      .def_property_readonly("trading_day", &strategy::Context::get_trading_day)
      .def("now", &strategy::Context::now)
      .def("add_timer", &strategy::Context::add_timer)
      .def("add_time_interval", &strategy::Context::add_time_interval)
      .def("add_account", &strategy::Context::add_account)
      .def("subscribe", &strategy::Context::subscribe)
      .def("subscribe_all", &strategy::Context::subscribe_all, py::arg("source"),
           py::arg("market_type") = MarketType::All, py::arg("instrument_type") = SubscribeInstrumentType::All,
           py::arg("data_type") = SubscribeDataType::All)
      .def("insert_order_input", &strategy::Context::insert_order_input)
      .def("insert_order", &strategy::Context::insert_order, py::arg("instrument_id"), py::arg("exchange"),
           py::arg("source"), py::arg("account"), py::arg("limit_price"), py::arg("volume"), py::arg("type"),
           py::arg("side"), py::arg("offset") = Offset::Open, py::arg("hedge_flag") = HedgeFlag::Speculation,
           py::arg("is_swap") = false, py::arg("block_id") = 0, py::arg("parent_id") = 0)
      .def("insert_block_message", &strategy::Context::insert_block_message, py::arg("source"), py::arg("account"),
           py::arg("opponent_seat"), py::arg("match_number"), py::arg("is_specific") = false)
      .def("insert_batch_orders", &strategy::Context::insert_batch_orders)
      .def("insert_array_orders", &strategy::Context::insert_array_orders)
      .def("insert_basket_order", &strategy::Context::insert_basket_order, py::arg("basket_id"), py::arg("source"),
           py::arg("account"), py::arg("side"), py::arg("price_type") = PriceType::Limit,
           py::arg("price_level") = PriceLevel::Last, py::arg("price_offset") = 0, py::arg("volume") = 0)
      .def("cancel_order", &strategy::Context::cancel_order)
      .def("req_history_order", &strategy::Context::req_history_order, py::arg("source"), py::arg("account"),
           py::arg("query_num") = 0)
      .def("req_history_trade", &strategy::Context::req_history_trade, py::arg("source"), py::arg("account"),
           py::arg("query_num") = 0)
      .def("hold_book", &strategy::Context::hold_book)
      .def("hold_positions", &strategy::Context::hold_positions)
      .def("is_book_held", &strategy::Context::is_book_held)
      .def("is_positions_mirrored", &strategy::Context::is_positions_mirrored)
      .def("req_deregister", &strategy::Context::req_deregister)
      .def("update_strategy_state", &strategy::Context::update_strategy_state)
      .def("get_writer", &strategy::Context::get_writer)
      .def("is_bypass_accounting", &strategy::Context::is_bypass_accounting)
      .def("bypass_accounting", &strategy::Context::bypass_accounting);

  py::class_<strategy::RuntimeContext, strategy::Context, strategy::RuntimeContext_ptr>(m, "RuntimeContext")
      .def_property_readonly("bookkeeper", &strategy::RuntimeContext::get_bookkeeper,
                             py::return_value_policy::reference)
      .def_property_readonly("basketorder_engine", &strategy::RuntimeContext::get_basketorder_engine,
                             py::return_value_policy::reference);

  py::class_<strategy::Strategy, PyStrategy, strategy::Strategy_ptr>(m, "Strategy")
      .def(py::init())
      .def("pre_start", &strategy::Strategy::pre_start)
      .def("post_start", &strategy::Strategy::post_start)
      .def("pre_stop", &strategy::Strategy::pre_stop)
      .def("post_stop", &strategy::Strategy::post_stop)
      .def("on_trading_day", &strategy::Strategy::on_trading_day)
      .def("on_quote", &strategy::Strategy::on_quote)
      .def("on_tree", &strategy::Strategy::on_tree)
      .def("on_bar", &strategy::Strategy::on_bar)
      .def("on_entrust", &strategy::Strategy::on_entrust)
      .def("on_transaction", &strategy::Strategy::on_transaction)
      .def("on_order", &strategy::Strategy::on_order)
      .def("on_order_action_error", &strategy::Strategy::on_order_action_error)
      .def("on_trade", &strategy::Strategy::on_trade)
      .def("on_position_sync_reset", &strategy::Strategy::on_position_sync_reset)
      .def("on_asset_sync_reset", &strategy::Strategy::on_asset_sync_reset)
      .def("on_asset_margin_sync_reset", &strategy::Strategy::on_asset_margin_sync_reset)
      .def("on_deregister ", &strategy::Strategy::on_deregister)
      .def("on_broker_state_change ", &strategy::Strategy::on_broker_state_change)
      .def("on_history_order", &strategy::Strategy::on_history_order)
      .def("on_history_trade", &strategy::Strategy::on_history_trade)
      .def("on_req_history_order_error", &strategy::Strategy::on_req_history_order_error)
      .def("on_req_history_trade_error", &strategy::Strategy::on_req_history_trade_error)
      .def("on_custom_data", &strategy::Strategy::on_custom_data);
}
} // namespace kungfu::wingchun::pybind