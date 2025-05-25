#include "service/user_service.hpp"
#include "models/user.hpp"


void UserService::createUser(User& user) {
    this->repository()->create<User>(user);
}

bool UserService::isUserExists(const std::string& username) {
    auto db = this->repository()->db();

    if (!db->isConnected()) {
        throw std::runtime_error("Database connection is not established");
    }

    std::string query = "SELECT COUNT(*) as cnt FROM " + User::tableName() + " WHERE username = '" + username + "'";
    DBResult result = db->execute(query);

    return result.size() > 0 && std::get<int64_t>(result[0].at("cnt")) > 0;
}

User UserService::getUserById(long long id) {
    return this->repository()->findById<User>(id);
}