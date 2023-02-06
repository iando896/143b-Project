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

void Resource::addToWaitlist(int proc) {
    waitlist.push_back(proc);
}