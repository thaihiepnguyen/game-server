#pragma once
#include "core/service/service.hpp"
#include "service/user_service.hpp"
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <iomanip>
#include <sstream>
#include <tuple>
#include <jwt-cpp/jwt.h>

const unsigned int SALT_LENGTH = 16;
const unsigned int SHA256_LENGTH = SHA256_DIGEST_LENGTH;


class AuthService : public IService {
private:
    std::shared_ptr<UserService> _userService;
private:
    std::string _toHex(const unsigned char* data, size_t length);

    std::string _sha256(const std::string& input);

    std::string _generateSalt();

    std::tuple<std::string, std::string> _hashPassword(const std::string& password);

    bool _verifyPassword(const std::string& password, const std::string& hashedPassword, const std::string& salt);
    
    std::string _generateToken(const User& user);
public:
    void inject(std::shared_ptr<Provider> provider) override {
        _userService = provider->getService<UserService>();
    }

    std::tuple<bool, std::string> registerUser(const std::string& username, const std::string& password);

    long long verifyToken(const std::string& token);

    std::tuple<bool, std::string, std::string> loginUser(const std::string& username, const std::string& password);
};