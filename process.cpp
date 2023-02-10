#include "process.hpp"

Process::Process (int st, int par, int prio) {
    state = st;
    parent = par;
    priority = prio;
}

int Process::getState() {
    return state;
}

void Process::block() {
    state = BLOCKED;
}

void Process::ready() {
    state = READY;
}

void Process::open() {
    state = OPEN;
}

int Process::getParent() {
    return parent;
}

void Process::setParent(int par) {
    parent = par;
}

int Process::getPriority() {
    return priority;
}

void Process::setPriority(int prio) {
    priority = prio;
}

int Process::childrenSize() {
    return children.size();
}

// std::unordered_set<int>& Process::getChildren() {
//     // if (ind < 0 or ind >= children.size())
//     //     return -1;
//     return children;
// }

bool Process::removeChild(int proc) {
    auto it = children.begin();
    for (; it != children.end(); it++)
    {
        if (*it == proc)
            break;
    }
    if (it != children.end()) {
        children.erase(it);
        return true;
    }
    return false;
}

void Process::printChildren() {
    for (int j = 0; j < children.size(); j++) {
        std::cout << children[j] << " ";
    }
    std::cout << std::endl;
}

bool Process::findChild(int child) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == child)
            return true;
    }
    return false;
}

int Process::frontChildren() {
    return children.front();
}

void Process::popChildren() {
    children.pop_front();
}

void Process::pushChild(int child) {
    children.push_back(child);
}

void Process::clearChildren() {
    children.clear();
}

std::unordered_map<int,int> Process::getResources() {
    return resources;
}

void Process::addResource(int r, int n) {
    if (resources.count(r) == 0)
        resources[r] = n;
    else
        resources[r] += n;
}

void Process::removeResource(int r) {
    resources.erase(r);
}

void Process::clearResources() {
    resources.clear();
}

void Process::pushWaiting(int r) {
    waiting.push_back(r);
}

int Process::frontWaiting() {
    return waiting.front();
}

void Process::popWaiting() {
    waiting.pop_front();
}

void Process::clearWaiting() {
    waiting.clear();
}

int Process::waitingSize() {
    return waiting.size();
}