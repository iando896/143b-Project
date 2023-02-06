#pragma once
#include <deque>
#include <unordered_map>

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
    std::unordered_set<int> children;
    std::unordered_map<int,int> resources;
public:
    Process() {state = -1; parent = -1; priority = -1;}
    Process(int st, int par, int prio);
    int getState();
    void setState(int st);

    int getPriority();
    void setPriority(int pr);

    int getParent();
    void setParent(int par);

    int childrenSize();
    void addChild(int child);
    std::unordered_set<int> getChildren();
    void removeChild(int proc);
    void clearChildren();

    void addResource(int res, int n);
    void clearResources();
};
