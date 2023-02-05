#pragma once
#include <vector>

class Process {
private:
    int state;
    int parent;
    std::vector<int> children;
    std::vector<int> resources;
public:
    Process() {}
    Process(int st, int par);
    int getState();
    int getParent();
};
