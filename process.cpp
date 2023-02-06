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

void Process::addChild(int child) {
    children.push_back(child);
}

void Process::clearChildren() {
    children.clear();
}

void Process::addResource(int res) {
    resources.push_back(res);
}

void Process::clearResources() {
    resources.clear();
}