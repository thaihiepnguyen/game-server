#include "service/auth_service.hpp"

std::string AuthService::_toHex(const unsigned char* data, size_t length) {
    std::stringstream ss;
    for (size_t i = 0; i < length; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    return ss.str();
}

std::string AuthService::_sha256(const std::string& input) {
    unsigned char hash[SHA256_LENGTH];
    SHA256((const unsigned char*)input.c_str(), input.size(), hash);
    return _toHex(hash, SHA256_LENGTH);
}

std::string AuthService::_generateSalt() {
    unsigned char salt[SALT_LENGTH];
    RAND_bytes(salt, sizeof(salt));
    return _toHex(salt, sizeof(salt));
}

std::tuple<std::string, std::string> AuthService::_hashPassword(const std::string& password) {
    std::string salt = _generateSalt();
    std::string saltedPassword = salt + password;
    std::string hash = _sha256(saltedPassword);
    return std::make_tuple(salt, hash);
}

bool AuthService::_verifyPassword(const std::string& password, const std::string& hashedPassword, const std::string& salt) {
    std::string saltedPassword = salt + password;
    return _sha256(saltedPassword) == hashedPassword;
}

std::string AuthService::_generateToken(const User& user) {
    auto token = jwt::create()
        .set_type("JWT")
        .set_issuer("auth_service")
        .set_payload_claim("user_id", jwt::claim(std::to_string(user.id)))
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(1))
        .sign(jwt::algorithm::hs256{"secret_key"});
    return token;
}

std::tuple<bool, std::string> AuthService::registerUser(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        return { false, "Username and password cannot be empty" };
    }

    if (username.length() < 3 || username.length() > 50){
        return { false, "Username must be between 3 and 50 characters" };
    }

    if (password.length() < 6 || password.length() > 100) {
        return { false, "Password must be between 6 and 100 characters" };
    }

    try {
        if (_userService->isUserExists(username)) {
            return { false, "User already exists" };
        }

        auto [salt, hashedPassword] = _hashPassword(password);
        User user(username, hashedPassword, salt);
        _userService->createUser(user);
        return { true, "User registered successfully" };
    } catch (const std::exception& e) {
        return { false, std::string("Error registering user: ") + e.what() };
    }
}

long long AuthService::verifyToken(const std::string& token) {
    auto verifier = jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{"secret_key"})
        .with_issuer("auth_service");

    auto decoded = jwt::decode(token);
    verifier.verify(decoded);

    if (!decoded.has_payload_claim("user_id")) {
        throw std::runtime_error("Token does not contain user_id claim");
    }
    auto userIdClaim = decoded.get_payload_claim("user_id");
    if (userIdClaim.get_type() != jwt::json::type::string) {
        throw std::runtime_error("user_id claim is not a string");
    }
    long long userId = std::stoll(userIdClaim.as_string());
    return userId;
}

std::tuple<bool, std::string, std::string> AuthService::loginUser(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        return { false, "Username and password cannot be empty", "" };
    }

    try {
        auto user = _userService->getUserByUsername(username);
        if (!user.has_value()) {
            return { false, "User not found", "" };
        }

        const User& value = user.value();

        if (_verifyPassword(password, value.password, value.salt)) {
            return { true, "Login successful", _generateToken(value) };
        } else {
            return { false, "Invalid password", "" };
        }
    } catch (const std::exception& e) {
        return { false, std::string("Error logging in: ") + e.what(), ""};
    }
}