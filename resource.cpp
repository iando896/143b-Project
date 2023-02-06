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

void Resource::removeFromWaitlist(int proc, int n) {
    waitlist.push_back(std::pair<int,int> (proc, n));
}