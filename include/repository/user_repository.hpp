#include "core/repository/repository.hpp"
#include <iostream>

class UserRepository : public IRepository {
public:
    void createUser(const std::string& username, const std::string& password);
};