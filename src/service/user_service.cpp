#include "service/user_service.hpp"



void UserService::createUser(const std::string& username, const std::string& password) {
    _userRepository->createUser(username, password);
}