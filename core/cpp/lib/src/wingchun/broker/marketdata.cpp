//
// Created by Keren Dong on 2019-06-20.
//

#include <utility>
#include <hffix.hpp>

#include <kungfu/wingchun/broker/marketdata.h>

using namespace kungfu::rx;
using namespace kungfu::longfist::types;
using namespace kungfu::yijinjing::practice;
using namespace kungfu::yijinjing;
using namespace kungfu::yijinjing::data;

namespace kungfu::wingchun::broker
{
    MarketData::MarketData(bool low_latency, locator_ptr locator, const std::string &source) :
            apprentice(location::make_shared(mode::LIVE, category::MD, source, source, std::move(locator)), low_latency)
    {
        log::copy_log_settings(get_io_device()->get_home(), source);
    }

    void MarketData::on_start()
    {
        apprentice::on_start();

        events_ | is(SubscribeAll::tag) |
        $([&](const event_ptr &event)
          {
              SPDLOG_INFO("subscribe all request");
              subscribe_all();
          });

        events_ | is(Subscribe::tag) |
        $([&](const event_ptr &event)
          {
              SPDLOG_INFO("subscribe request");
              std::vector<Instrument> symbols;
              const char *buffer = &(event->data<char>());
              hffix::message_reader reader(buffer, buffer + event->data_length());
              for (; reader.is_complete(); reader = reader.next_message_reader())
              {
                  if (reader.is_valid())
                  {
                      auto group_mdentry_begin = std::find_if(reader.begin(), reader.end(), hffix::tag_equal(hffix::tag::MDEntryType));
                      hffix::message_reader::const_iterator group_mdentry_end;
                      for (; group_mdentry_begin != reader.end(); group_mdentry_begin = group_mdentry_end)
                      {
                          group_mdentry_end = std::find_if(group_mdentry_begin + 1, reader.end(), hffix::tag_equal(hffix::tag::MDEntryType));

                          auto group_instrument_begin = std::find_if(group_mdentry_begin, group_mdentry_end,
                                                                     hffix::tag_equal(hffix::tag::Symbol));
                          hffix::message_reader::const_iterator group_instrument_end;

                          for (; group_instrument_begin != group_mdentry_end; group_instrument_begin = group_instrument_end)
                          {
                              group_instrument_end = std::find_if(group_instrument_begin + 1, group_mdentry_end,
                                                                  hffix::tag_equal(hffix::tag::Symbol));
                              hffix::message_reader::const_iterator symbol = group_instrument_begin;
                              hffix::message_reader::const_iterator exchange = group_instrument_begin;
                              reader.find_with_hint(hffix::tag::SecurityExchange, exchange);

                              Instrument instrument{};
                              strcpy(instrument.instrument_id, symbol->value().as_string().c_str());
                              strcpy(instrument.exchange_id, exchange->value().as_string().c_str());
                              symbols.push_back(instrument);
                          }
                      }
                  }
              }
              subscribe(symbols);
          });
    }

    void MarketData::publish_state(longfist::enums::BrokerState state)
    {
        longfist::types::BrokerStateUpdate update{};
        update.state = state;
        write_to(0, update);
    }
}
