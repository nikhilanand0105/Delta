#include "delta/client.hpp"

namespace delta {

DeltaClient::DeltaClient(ClientConfig config)
    : config_(std::move(config)) {}

LimitOrderBook& DeltaClient::get_or_create_orderbook(const std::string& symbol) {
    auto it = orderbooks_.find(symbol);
    if (it == orderbooks_.end()) {
        auto book = std::make_unique<LimitOrderBook>(symbol);
        auto [inserted_it, _] = orderbooks_.emplace(symbol, std::move(book));
        return *inserted_it->second;
    }
    return *it->second;
}

std::string DeltaClient::prepare_auth_header(std::string_view method, std::string_view path, std::string_view body) const {
    int64_t ts = current_timestamp_ms() / 1000;
    return Signer::create_delta_signature(config_.api_secret, method, ts, path, body);
}

void DeltaClient::process_l2_update(const std::string& symbol, const std::vector<Level>& bids, const std::vector<Level>& asks) {
    auto& book = get_or_create_orderbook(symbol);
    for (const auto& b : bids) {
        book.update_bid(b.price, b.size);
    }
    for (const auto& a : asks) {
        book.update_ask(a.price, a.size);
    }
}

} // namespace delta
