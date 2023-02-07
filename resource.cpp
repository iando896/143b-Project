#include "resource.hpp"

void Resource::printResource() {
    std::cout << "State: " << state << std::endl;
    std::cout << "Inventory: " << inventory << std::endl;
    std::cout << "Waitlist: ";
    for (int i = 0; i < waitlist.size(); i++) {
        std::cout << waitlist[i].first << ":" << waitlist[i].second << " ";
    }
    std::cout << std::endl;
}

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
        //std::cout << it->first << std::endl;
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