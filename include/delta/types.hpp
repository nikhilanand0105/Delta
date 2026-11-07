#ifndef DELTA_TYPES_HPP
#define DELTA_TYPES_HPP

#include <string>
#include <cstdint>
#include <chrono>

namespace delta {

enum class Side {
    Buy,
    Sell
};

enum class OrderType {
    Limit,
    Market,
    StopOrder
};

struct Level {
    double price{0.0};
    double size{0.0};

    bool operator==(const Level& other) const {
        return price == other.price && size == other.size;
    }
};

struct Order {
    std::string order_id;
    std::string symbol;
    Side side;
    OrderType order_type;
    double price{0.0};
    double size{0.0};
    int64_t timestamp_ms{0};
};

struct Trade {
    std::string trade_id;
    std::string symbol;
    double price{0.0};
    double size{0.0};
    Side taker_side;
    int64_t timestamp_ms{0};
};

struct MarketQuote {
    std::string symbol;
    double best_bid{0.0};
    double best_ask{0.0};
    double bid_size{0.0};
    double ask_size{0.0};
    double spread() const { return best_ask - best_bid; }
    double mid_price() const { return (best_bid + best_ask) / 2.0; }
};

inline int64_t current_timestamp_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

} // namespace delta

#endif // DELTA_TYPES_HPP
