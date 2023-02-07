#pragma once
#include <deque>
#include <iostream>

#define ALLOCATED 1
#define FREE 0

class Resource {
private:
    int state;
    int inventory;
    std::deque<std::pair<int,int>> waitlist;
public:
    Resource() {state = -1; inventory = -1;}
    Resource(int st, int inv) {state = st; inventory = inv;}
    void printResource();

    int getState();
    void setState(int st);

    int getInventory();
    void setInventory(int n);

    std::deque<std::pair<int,int>>& getWaitlist();
    void addToWaitlist(int proc, int n);
    bool removeFromWaitlist(int proc);
    void clearWaitlist();
};