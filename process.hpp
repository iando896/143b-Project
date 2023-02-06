#pragma once
#include <deque>

#define READY 1
#define BLOCKED 0
#define FREE -1
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
    std::deque<int> resources;
public:
    Process() {state = -1; parent = -1; priority = -1;}
    Process(int st, int par, int prio);
    int getState();
    void setState(int st);

    int getPriority();
    void setPriority(int pr);

    int getParent();
    void setParent(int par);

    void addChild(int child);
    void clearChildren();
    void addResource(int res);
    void clearResources();
};
