#include "helper.hpp"

bool isnumber(std::string s) {
    for (int i = 0; i < s.size(); i++) {
        if (!isdigit(s[i])) {
            return false;
        } 
    }
    return true;
}