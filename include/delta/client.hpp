#ifndef DELTA_CLIENT_HPP
#define DELTA_CLIENT_HPP

#include "types.hpp"
#include "orderbook.hpp"
#include "signer.hpp"
#include <string>
#include <memory>
#include <unordered_map>

namespace delta {

struct ClientConfig {
    std::string api_key;
    std::string api_secret;
    std::string base_url{"https://api.delta.exchange"};
    std::string ws_url{"wss://socket.delta.exchange"};
    int timeout_ms{5000};
};

class DeltaClient {
public:
    explicit DeltaClient(ClientConfig config);

    const ClientConfig& config() const { return config_; }

    LimitOrderBook& get_or_create_orderbook(const std::string& symbol);

    std::string prepare_auth_header(std::string_view method, std::string_view path, std::string_view body = "") const;

    void process_l2_update(const std::string& symbol, const std::vector<Level>& bids, const std::vector<Level>& asks);

private:
    ClientConfig config_;
    std::unordered_map<std::string, std::unique_ptr<LimitOrderBook>> orderbooks_;
};

} // namespace delta

#endif // DELTA_CLIENT_HPP
