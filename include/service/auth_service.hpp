#pragma once
#include "core/service/service.hpp"
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <iomanip>
#include <sstream>
#include <tuple>

const unsigned int SALT_LENGTH = 16;
const unsigned int SHA256_LENGTH = SHA256_DIGEST_LENGTH;


class AuthService : public IService {
private:
    std::string _toHex(const unsigned char* data, size_t length) {
        std::stringstream ss;
        for (size_t i = 0; i < length; ++i)
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
        return ss.str();
    }

    std::string _sha256(const std::string& input) {
        unsigned char hash[SHA256_LENGTH];
        SHA256((const unsigned char*)input.c_str(), input.size(), hash);
        return _toHex(hash, SHA256_LENGTH);
    }

    std::string _generateSalt() {
        unsigned char salt[SALT_LENGTH];
        RAND_bytes(salt, sizeof(salt));
        return _toHex(salt, sizeof(salt));
    }
public:

    std::tuple<std::string, std::string> hashPassword(const std::string& password) {
        std::string salt = _generateSalt();
        std::string saltedPassword = salt + password;
        std::string hash = _sha256(saltedPassword);
        return std::make_tuple(salt, hash);
    }

    bool verifyPassword(const std::string& password, const std::string& hashedPassword, const std::string& salt) {
        std::string saltedPassword = salt + password;
        return _sha256(saltedPassword) == hashedPassword;
    }
};