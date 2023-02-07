#pragma once
#include <deque>
#include <unordered_map>
#include <iostream>

#define READY 1
#define BLOCKED 0
#define OPEN -1

class Process {
private:
    //-1 - clear to be replaced
    //0 - blocked
    //1 - ready
    int state;

    //-1 if no parent
    int parent;
    int priority;
    std::deque<int> children;
    std::unordered_map<int,int> resources;
    std::deque<int> waiting;
public:
    Process() {state = -1; parent = -1; priority = -1;}
    Process(int st, int par, int prio);
    int getState();
    //void setState(int st);
    void ready();
    void open();
    void block();

    int getPriority();
    void setPriority(int pr);

    int getParent();
    void setParent(int par);

    int childrenSize();
    //std::unordered_set<int>& getChildren();
    bool findChild(int child);
    void printChildren();
    int frontChildren();
    void popChildren();
    void pushChild(int child);
    bool removeChild(int proc);
    void clearChildren();

    std::unordered_map<int,int> getResources();
    void addResource(int r, int n);
    void removeResource(int r);
    void clearResources();

    void pushWaiting(int r);
    int frontWaiting();
    void popWaiting();
    int waitingSize();
    void clearWaiting();
};
