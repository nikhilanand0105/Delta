#include "delta/orderbook.hpp"
#include <algorithm>

namespace delta {

LimitOrderBook::LimitOrderBook(std::string symbol)
    : symbol_(std::move(symbol)) {}

void LimitOrderBook::set_bids(const std::vector<Level>& bids) {
    bids_.clear();
    for (const auto& level : bids) {
        if (level.size > 0.0) {
            bids_[level.price] = level.size;
        }
    }
}

void LimitOrderBook::set_asks(const std::vector<Level>& asks) {
    asks_.clear();
    for (const auto& level : asks) {
        if (level.size > 0.0) {
            asks_[level.price] = level.size;
        }
    }
}

void LimitOrderBook::update_bid(double price, double size) {
    if (size <= 0.0) {
        bids_.erase(price);
    } else {
        bids_[price] = size;
    }
}

void LimitOrderBook::update_ask(double price, double size) {
    if (size <= 0.0) {
        asks_.erase(price);
    } else {
        asks_[price] = size;
    }
}

void LimitOrderBook::clear() {
    bids_.clear();
    asks_.clear();
}

std::optional<Level> LimitOrderBook::best_bid() const {
    if (bids_.empty()) {
        return std::nullopt;
    }
    const auto& [price, size] = *bids_.begin();
    return Level{price, size};
}

std::optional<Level> LimitOrderBook::best_ask() const {
    if (asks_.empty()) {
        return std::nullopt;
    }
    const auto& [price, size] = *asks_.begin();
    return Level{price, size};
}

std::optional<MarketQuote> LimitOrderBook::get_quote() const {
    auto bb = best_bid();
    auto ba = best_ask();
    if (!bb || !ba) {
        return std::nullopt;
    }
    MarketQuote q;
    q.symbol = symbol_;
    q.best_bid = bb->price;
    q.bid_size = bb->size;
    q.best_ask = ba->price;
    q.ask_size = ba->size;
    return q;
}

std::vector<Level> LimitOrderBook::get_bids(size_t depth) const {
    std::vector<Level> result;
    result.reserve(std::min(depth, bids_.size()));
    for (const auto& [price, size] : bids_) {
        if (result.size() >= depth) break;
        result.push_back({price, size});
    }
    return result;
}

std::vector<Level> LimitOrderBook::get_asks(size_t depth) const {
    std::vector<Level> result;
    result.reserve(std::min(depth, asks_.size()));
    for (const auto& [price, size] : asks_) {
        if (result.size() >= depth) break;
        result.push_back({price, size});
    }
    return result;
}

double LimitOrderBook::mid_price() const {
    auto quote = get_quote();
    return quote ? quote->mid_price() : 0.0;
}

double LimitOrderBook::spread() const {
    auto quote = get_quote();
    return quote ? quote->spread() : 0.0;
}

} // namespace delta
