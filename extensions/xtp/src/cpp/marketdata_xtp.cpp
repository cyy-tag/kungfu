//
// Created by qlu on 2019/2/11.
//

#include "marketdata_xtp.h"
#include "serialize_xtp.h"
#include "type_convert.h"

using namespace kungfu::yijinjing;
using namespace kungfu::yijinjing::data;

namespace kungfu::wingchun::xtp {
struct MDConfiguration {
  int client_id;
  std::string account_id;
  std::string password;
  std::string md_ip;
  int md_port;
  std::string protocol;
  int buffer_size;
};

void from_json(const nlohmann::json &j, MDConfiguration &c) {
  j.at("client_id").get_to(c.client_id);
  j.at("account_id").get_to(c.account_id);
  j.at("password").get_to(c.password);
  j.at("md_ip").get_to(c.md_ip);
  j.at("md_port").get_to(c.md_port);
  c.protocol = j.value("protocol", "tcp");
  if (c.protocol != "udp") {
    c.protocol = "tcp";
  }
  c.buffer_size = j.value("buffer_size", 64);
}

MarketDataXTP::MarketDataXTP(broker::BrokerVendor &vendor) : MarketData(vendor), api_(nullptr) { //
  KUNGFU_SETUP_LOG();
}

MarketDataXTP::~MarketDataXTP() {
  if (api_ != nullptr) {
    api_->Release();
  }
}

// 登录行情柜台
void MarketDataXTP::on_start() {
  // public_wirter存储普通行情信息
  public_wirter_ = get_writer(0);
  // 根据名称分配uid
  level2_tick_band_uid_ = request_band("market-data-band");

  MDConfiguration config = nlohmann::json::parse(get_config());
  if (config.client_id < 1 or config.client_id > 99) {
    SPDLOG_ERROR("client_id must between 1 and 99");
  }
  auto md_ip = config.md_ip.c_str();
  auto account_id = config.account_id.c_str();
  auto password = config.password.c_str();
  auto protocol_type = get_xtp_protocol_type(config.protocol);
  std::string runtime_folder = get_runtime_folder();
  SPDLOG_INFO("Connecting XTP MD for {} at {}://{}:{}", account_id, config.protocol, md_ip, config.md_port);
  api_ = XTP::API::QuoteApi::CreateQuoteApi(config.client_id, runtime_folder.c_str());
  if (config.protocol == "udp") {
    api_->SetUDPBufferSize(config.buffer_size);
  }
  api_->RegisterSpi(this);
  if (api_->Login(md_ip, config.md_port, account_id, password, protocol_type) == 0) {
    update_broker_state(BrokerState::LoggedIn);
    update_broker_state(BrokerState::Ready);
    SPDLOG_INFO("login success! (account_id) {}", config.account_id);
    api_->QueryAllTickers(XTP_EXCHANGE_SH);
    api_->QueryAllTickers(XTP_EXCHANGE_SZ);
    api_->QueryAllTickersFullInfo(XTP_EXCHANGE_SH);
    api_->QueryAllTickersFullInfo(XTP_EXCHANGE_SZ);
  } else {
    update_broker_state(BrokerState::LoginFailed);
    SPDLOG_ERROR("failed to login, [{}] {}", api_->GetApiLastError()->error_id, api_->GetApiLastError()->error_msg);
  }
}

bool MarketDataXTP::subscribe(const std::vector<InstrumentKey> &instrument_keys) {
  bool result = true;
  std::vector<std::string> sse_tickers;
  std::vector<std::string> sze_tickers;
  for (const auto &inst : instrument_keys) {
    std::string ticker = inst.instrument_id;
    if (strcmp(inst.exchange_id, EXCHANGE_SSE) == 0) {
      sse_tickers.push_back(ticker);
    } else if (strcmp(inst.exchange_id, EXCHANGE_SZE) == 0) {
      sze_tickers.push_back(ticker);
    }
  }
  if (!sse_tickers.empty()) {
    result = result && subscribe(sse_tickers, EXCHANGE_SSE);
  }
  if (!sze_tickers.empty()) {
    result = result && subscribe(sze_tickers, EXCHANGE_SZE);
  }
  return result;
}

bool MarketDataXTP::subscribe(const std::vector<std::string> &instruments, const std::string &exchange_id) {
  int size = instruments.size();
  std::vector<char *> insts;
  insts.reserve(size);
  for (auto &s : instruments) {
    insts.push_back((char *)&s[0]);
  }
  XTP_EXCHANGE_TYPE exchange;
  to_xtp(exchange, (char *)exchange_id.c_str());
  int level1_result = api_->SubscribeMarketData(insts.data(), size, exchange);
  int level2_result = api_->SubscribeTickByTick(insts.data(), size, exchange);
  SPDLOG_INFO("subscribe {} from {}, l1 rtn code {}, l2 rtn code {}", size, exchange_id, level1_result, level2_result);
  return level1_result == 0 and level2_result == 0;
}

bool MarketDataXTP::subscribe_all() {
  auto result = api_->SubscribeAllMarketData() && api_->SubscribeAllTickByTick();
  SPDLOG_INFO("subscribe all, rtn code {}", result);
  return result;
}

bool MarketDataXTP::subscribe_custom(const longfist::types::CustomSubscribe &custom_sub) {
  SPDLOG_INFO("custom_sub, market_type {} instrument_type {} data_type {} update_time {}", int(custom_sub.market_type),
              long(custom_sub.instrument_type), long(custom_sub.data_type), long(custom_sub.update_time));
  subscribe_all();
  return true;
}

void MarketDataXTP::OnDisconnected(int reason) {
  SPDLOG_ERROR("disconnected with reason {}", reason);
  update_broker_state(BrokerState::DisConnected);
}

void MarketDataXTP::OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) {
  if (error_info != nullptr && error_info->error_id != 0) {
    SPDLOG_ERROR("failed to subscribe level 1, [{}] {}", error_info->error_id, error_info->error_msg);
  }
}

void MarketDataXTP::OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last) {
  if (error_info != nullptr && error_info->error_id != 0) {
    SPDLOG_ERROR("failed to subscribe level 2, [{}] {}", error_info->error_id, error_info->error_msg);
  }
}

void MarketDataXTP::OnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) {
  if (error_info != nullptr && error_info->error_id != 0) {
    SPDLOG_ERROR("failed to subscribe level 2 all, [{}] {}", error_info->error_id, error_info->error_msg);
  }
}

// 获取沪深当前交易日合约部分静态信息
void MarketDataXTP::OnQueryAllTickers(XTPQSI *ticker_info, XTPRI *error_info, bool is_last) {
  if (nullptr != error_info && error_info->error_id != 0) {
    SPDLOG_ERROR("error_id : {} , error_msg : {}", error_info->error_id, error_info->error_msg);
    return;
  }

  if (nullptr == ticker_info) {
    SPDLOG_ERROR("ticker_info is nullptr");
    return;
  }

  Instrument &instrument = public_wirter_->open_data<Instrument>(0);
  from_xtp(ticker_info, instrument);
  public_wirter_->close_data();
  SPDLOG_TRACE("instrument {}", instrument.to_string());
}

// 行情通知函数
void MarketDataXTP::OnDepthMarketData(XTPMD *market_data, int64_t *bid1_qty, int32_t bid1_count, int32_t max_bid1_count,
                                      int64_t *ask1_qty, int32_t ask1_count, int32_t max_ask1_count) {
  Quote &quote = public_wirter_->open_data<Quote>(0);
  from_xtp(*market_data, quote);
  public_wirter_->close_data();
}

// 订阅逐笔行情接口通知函数, 写入深度行情
void MarketDataXTP::OnTickByTick(XTPTBT *tbt_data) {
  if (tbt_data->type == XTP_TBT_ENTRUST) {
    Entrust &entrust = get_writer(level2_tick_band_uid_)->open_data<Entrust>(0);
    from_xtp(*tbt_data, entrust);
    get_writer(level2_tick_band_uid_)->close_data();
  } else if (tbt_data->type == XTP_TBT_TRADE) {
    Transaction &transaction = get_writer(level2_tick_band_uid_)->open_data<Transaction>(0);
    from_xtp(*tbt_data, transaction);
    get_writer(level2_tick_band_uid_)->close_data();
  }
}

void MarketDataXTP::OnQueryAllTickersFullInfo(XTPQFI *ticker_info, XTPRI *error_info, bool is_last) {
  if (nullptr != error_info && error_info->error_id != 0) {
    SPDLOG_INFO("error_id : {} , error_msg : {}", error_info->error_id, error_info->error_msg);
    return;
  }

  if (nullptr == ticker_info) {
    SPDLOG_ERROR("ticker_info is nullptr");
    return;
  }

  Instrument &instrument = public_wirter_->open_data<Instrument>(0);
  strcpy(instrument.instrument_id, ticker_info->ticker);
  if (ticker_info->exchange_id == 1) {
    instrument.exchange_id = EXCHANGE_SSE;
  } else if (ticker_info->exchange_id == 2) {
    instrument.exchange_id = EXCHANGE_SZE;
  }

  memcpy(instrument.product_id, ticker_info->ticker_name, strlen(ticker_info->ticker_name));
  instrument.instrument_type = get_instrument_type(instrument.exchange_id, instrument.instrument_id);
  public_wirter_->close_data();
  SPDLOG_TRACE("instrument {}", instrument.to_string());
}
} // namespace kungfu::wingchun::xtp
