#ifndef DELTA_ORDERBOOK_HPP
#define DELTA_ORDERBOOK_HPP

#include "types.hpp"
#include <map>
#include <vector>
#include <string>
#include <optional>
#include <functional>

namespace delta {

class LimitOrderBook {
public:
    explicit LimitOrderBook(std::string symbol);

    void set_bids(const std::vector<Level>& bids);
    void set_asks(const std::vector<Level>& asks);

    void update_bid(double price, double size);
    void update_ask(double price, double size);

    void clear();

    std::optional<Level> best_bid() const;
    std::optional<Level> best_ask() const;

    std::optional<MarketQuote> get_quote() const;

    std::vector<Level> get_bids(size_t depth = 10) const;
    std::vector<Level> get_asks(size_t depth = 10) const;

    double mid_price() const;
    double spread() const;

    const std::string& symbol() const { return symbol_; }
    size_t bid_depth() const { return bids_.size(); }
    size_t ask_depth() const { return asks_.size(); }

private:
    std::string symbol_;
    // Bids sorted in descending order (highest price first)
    std::map<double, double, std::greater<double>> bids_;
    // Asks sorted in ascending order (lowest price first)
    std::map<double, double, std::less<double>> asks_;
};

} // namespace delta

#endif // DELTA_ORDERBOOK_HPP
