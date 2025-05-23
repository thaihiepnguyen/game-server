#include <string>
#include <cxxabi.h>


class String {
public:
    /*
     * Convert a mangled C++ type name to a human-readable format.
     * 
     * @brief Demangle a C++ type name
     * @param name The mangled type name
     * @return The demangled type name
     */
    static std::string demangle(const char* name) {
        int status = -1;
        char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        std::string result = (status == 0 && demangled != nullptr) ? demangled : name;
        std::free(demangled);
        return result;
    }
};