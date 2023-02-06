#pragma once
#include <vector>

class Resource {
private:
    int state;
    int inventory;
    std::vector<int> waitlist;
public:
    Resource() {state = -1; inventory = -1;}
    Resource(int st, int inv) {state = st; inventory = inv;}

    int getState();
    void setState(int st);

    int getInventory();
    void setInventory(int n);

    void addToWaitlist(int proc);
};