#include "process.hpp"

Process::Process (int st, int par, int prio) {
    state = st;
    parent = par;
    priority = prio;
}

int Process::getState() {
    return state;
}

void Process::setState(int st) {
    state = st;
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

std::unordered_set<int> Process::getChildren() {
    // if (ind < 0 or ind >= children.size())
    //     return -1;
    return children;
}

void Process::removeChild(int proc) {
    children.erase(proc);
}

void Process::addChild(int child) {
    children.insert(child);
}

void Process::clearChildren() {
    children.clear();
}

void Process::addResource(int res, int n) {
    resources[res] = n;
}

void Process::clearResources() {
    resources.clear();
}