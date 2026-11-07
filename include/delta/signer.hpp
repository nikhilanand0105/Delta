#ifndef DELTA_SIGNER_HPP
#define DELTA_SIGNER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

namespace delta {

class Signer {
public:
    static std::string hmac_sha256_hex(std::string_view key, std::string_view message);
    static std::string sha256_hex(std::string_view message);

    static std::string create_delta_signature(
        std::string_view api_secret,
        std::string_view method,
        int64_t timestamp,
        std::string_view path,
        std::string_view query_or_body = ""
    );
};

} // namespace delta

#endif // DELTA_SIGNER_HPP
