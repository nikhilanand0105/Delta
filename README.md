# Delta C++ Trading & Orderbook Engine

High-performance C++17 client library, Level-2 Limit Orderbook (LOB), and cryptographic request signer for Delta Exchange.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

---

## Overview

`delta_core` is a lightweight, zero-external-dependency C++17 core engine designed for high-frequency trading applications interacting with Delta Exchange. It provides:

- **Level-2 Limit Orderbook (LOB):** In-memory orderbook maintaining bid/ask depth, sub-microsecond best bid/ask lookups, and dynamic spread calculations.
- **Incremental L2 Updates:** Fast updates and price-level cancellations (`size == 0`).
- **Cryptographic Request Signer:** Self-contained RFC-compliant SHA-256 and HMAC-SHA256 signature generator for REST API and WebSocket channel authentication.
- **Deterministic Unit Testing:** Automated test suites verifying orderbook sorting invariants, spread metrics, and NIST/RFC test vectors.

---

## Architecture

```
                  ┌────────────────────────────────────────┐
                  │          Delta Client Engine           │
                  └──────────────────┬─────────────────────┘
                                     │
           ┌─────────────────────────┴────────────────────────┐
           ▼                                                  ▼
┌───────────────────────┐                          ┌───────────────────────┐
│   Limit Orderbook     │                          │     Signer Module     │
│   (std::map sorted)   │                          │   (RFC HMAC-SHA256)   │
├───────────────────────┤                          ├───────────────────────┤
│ Bids: Descending      │                          │ Auth Headers          │
│ Asks: Ascending       │                          │ Nonce / Timestamp     │
│ O(1) Top of Book      │                          │ Zero Dependency       │
└───────────────────────┘                          └───────────────────────┘
```

---

## Quickstart

### Prerequisites
- Modern C++ compiler supporting C++17 (`clang++` or `g++`)
- `make` or `cmake` (>= 3.14)

### Building and Running Tests

```bash
# Clone the repository
git clone https://github.com/nikhilanand0105/Delta.git
cd Delta

# Build and execute unit tests
make test
```

### Running the Demo

```bash
make bin/delta_demo
./bin/delta_demo
```

---

## Code Example

```cpp
#include "delta/client.hpp"
#include <iostream>

int main() {
    delta::ClientConfig config;
    config.api_key = "MY_API_KEY";
    config.api_secret = "MY_API_SECRET";

    delta::DeltaClient client(config);
    auto& book = client.get_or_create_orderbook("BTCUSD");

    // Populate snapshot
    book.set_bids({{68500.0, 1.5}, {68490.0, 3.0}});
    book.set_asks({{68510.0, 0.8}, {68520.0, 2.0}});

    auto quote = book.get_quote();
    std::cout << "Spread: $" << quote->spread() << "\n";
    std::cout << "Mid Price: $" << quote->mid_price() << "\n";

    // Sign authenticated REST request
    std::string sig = delta::Signer::create_delta_signature(
        config.api_secret, "GET", 1700000000, "/v2/orders"
    );
    std::cout << "HMAC Signature: " << sig << "\n";
    return 0;
}
```

---

## License

This project is licensed under the [MIT License](LICENSE).
