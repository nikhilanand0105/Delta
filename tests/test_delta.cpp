#include "delta/orderbook.hpp"
#include "delta/signer.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

void test_orderbook_basics() {
    std::cout << "Running test_orderbook_basics... ";
    delta::LimitOrderBook book("BTCUSD");

    assert(book.symbol() == "BTCUSD");
    assert(!book.best_bid().has_value());
    assert(!book.best_ask().has_value());

    book.update_bid(100.0, 1.5);
    book.update_bid(102.0, 2.0); // Higher bid should be top
    book.update_bid(98.0, 5.0);

    book.update_ask(105.0, 1.0);
    book.update_ask(103.0, 2.5); // Lower ask should be top
    book.update_ask(110.0, 4.0);

    auto bb = book.best_bid();
    auto ba = book.best_ask();

    assert(bb.has_value());
    assert(bb->price == 102.0);
    assert(bb->size == 2.0);

    assert(ba.has_value());
    assert(ba->price == 103.0);
    assert(ba->size == 2.5);

    assert(std::abs(book.spread() - 1.0) < 1e-6);
    assert(std::abs(book.mid_price() - 102.5) < 1e-6);

    // Cancel top bid (size = 0)
    book.update_bid(102.0, 0.0);
    assert(book.best_bid()->price == 100.0);

    std::cout << "PASSED\n";
}

void test_depth_extraction() {
    std::cout << "Running test_depth_extraction... ";
    delta::LimitOrderBook book("ETHUSD");

    book.set_bids({{2500.0, 1.0}, {2490.0, 2.0}, {2480.0, 3.0}, {2470.0, 4.0}});
    book.set_asks({{2510.0, 1.5}, {2520.0, 2.5}, {2530.0, 3.5}, {2540.0, 4.5}});

    auto bids = book.get_bids(2);
    assert(bids.size() == 2);
    assert(bids[0].price == 2500.0);
    assert(bids[1].price == 2490.0);

    auto asks = book.get_asks(3);
    assert(asks.size() == 3);
    assert(asks[0].price == 2510.0);
    assert(asks[1].price == 2520.0);
    assert(asks[2].price == 2530.0);

    std::cout << "PASSED\n";
}

void test_signer_hash() {
    std::cout << "Running test_signer_hash... ";
    // SHA256 of empty string is e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
    std::string empty_hash = delta::Signer::sha256_hex("");
    assert(empty_hash == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

    // Standard RFC 4231 HMAC-SHA256 Test Case 2
    // Key: "Jefe", Data: "what do ya want for nothing?"
    // Digest: 5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843
    std::string hmac_test = delta::Signer::hmac_sha256_hex("Jefe", "what do ya want for nothing?");
    assert(hmac_test == "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843");

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "=== Delta C++ Unit Test Suite ===\n";
    test_orderbook_basics();
    test_depth_extraction();
    test_signer_hash();
    std::cout << "All 3 test suites passed successfully!\n";
    return 0;
}
