#include "service/user_service.hpp"



void UserService::createUser(const std::string& username, const std::string& password, const std::string& salt) {
    _userRepository->createUser(username, password, salt);
}