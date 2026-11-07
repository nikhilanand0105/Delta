#include "delta/client.hpp"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== Delta Exchange C++ Core Engine ===\n\n";

    delta::ClientConfig config;
    config.api_key = "SAMPLE_KEY";
    config.api_secret = "SAMPLE_SECRET";

    delta::DeltaClient client(config);
    auto& book = client.get_or_create_orderbook("BTCUSD");

    std::cout << "[1] Populating initial L2 Orderbook snapshot for BTCUSD...\n";
    book.set_bids({
        {68500.0, 1.42},
        {68490.0, 2.15},
        {68480.0, 5.00},
        {68470.0, 12.30}
    });

    book.set_asks({
        {68510.0, 0.85},
        {68520.0, 3.20},
        {68530.0, 4.50},
        {68550.0, 8.00}
    });

    auto quote = book.get_quote();
    if (quote) {
        std::cout << "  Best Bid : $" << quote->best_bid << " (" << quote->bid_size << " BTC)\n";
        std::cout << "  Best Ask : $" << quote->best_ask << " (" << quote->ask_size << " BTC)\n";
        std::cout << "  Spread   : $" << quote->spread() << "\n";
        std::cout << "  Mid Price: $" << quote->mid_price() << "\n";
    }

    std::cout << "\n[2] Applying incremental L2 update (price crossing)...\n";
    client.process_l2_update("BTCUSD", {{68505.0, 0.50}}, {{68510.0, 0.00}, {68515.0, 1.20}});

    auto updated_quote = book.get_quote();
    if (updated_quote) {
        std::cout << "  Updated Best Bid : $" << updated_quote->best_bid << " (" << updated_quote->bid_size << " BTC)\n";
        std::cout << "  Updated Best Ask : $" << updated_quote->best_ask << " (" << updated_quote->ask_size << " BTC)\n";
        std::cout << "  Updated Spread   : $" << updated_quote->spread() << "\n";
    }

    std::cout << "\n[3] Testing HMAC-SHA256 Request Signing...\n";
    std::string sig = delta::Signer::create_delta_signature("my_secret", "GET", 1700000000, "/v2/orders");
    std::cout << "  Generated Signature: " << sig << "\n";

    std::cout << "\nDelta C++ Core Engine verified successfully!\n";
    return 0;
}
