#include "core/repository/repository.hpp"
#include <iostream>

class UserRepository : public IRepository {
public:
    void add(const std::string& item) override {
        // Implementation for adding a user
        std::cout << "Adding user: " << item << "\n";
    }
};