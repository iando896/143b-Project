#include "resource.hpp"

int Resource::getState() {
    return state;
}

void Resource::setState(int st) {
    state = st;
}

int Resource::getInventory() {
    return inventory;
}

void Resource::setInventory(int n) {
    inventory = n;
} 

void Resource::addToWaitlist(int proc, int n) {
    waitlist.push_back(std::pair<int,int> (proc,n));
}

std::deque<std::pair<int,int>>& Resource::getWaitlist() {
    return waitlist;
}

bool Resource::removeFromWaitlist(int proc) {
    auto it = waitlist.begin();
    for (; it != waitlist.end(); it++) {
        if (it->first == proc)
            break;
    }
    if (it == waitlist.end())
        return false;
    waitlist.erase(it);
    return true;
}

void Resource::clearWaitlist() {
    waitlist.clear();
}