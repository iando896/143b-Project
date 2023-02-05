#include "Process.hpp"

Process::Process (int st, int par) {
        state = st;
        parent = par;
    }

int Process::getState() {
    return state;
}

int Process::getParent() {
    return parent;
}