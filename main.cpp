#include <string.h>
#include <iostream>
#include <vector>
#include <deque>
#include <bits/stdc++.h>
#include <string.h>
#include "process.hpp"
#include "resource.hpp"
using namespace std;

#define PCB_SIZE 16
#define RCB_SIZE 4
#define READY_LIST_SIZE 3
#define ERROR -1

Process PCB[PCB_SIZE];
Resource RCB[RCB_SIZE];
//vector<vector<int>> ready_list (READY_LIST_SIZE, vector<int>());
std::deque<int> RL[READY_LIST_SIZE];

// Erase all previous contents of the data structures PCB, RCB, RL
// Create a single running process at PCB[0] with priority 0
// Enter the process into the RL at the lowest-priority level 0

void init () {
    for (int i = 0; i < PCB_SIZE; i++) {
        PCB[i].setState(FREE);
        PCB[i].clearChildren();
        PCB[i].clearResources();
    }

    for(int i = 0; i < RCB_SIZE; i++) {
        RCB[i].setState(FREE);
    }

    for (int i = 0; i < READY_LIST_SIZE; i++) {
        RL[i].clear();
    }
    PCB[0] = Process(1,-1,0);
    RL[0].push_back(0);
}

int scheduler() {
    for (int i = READY_LIST_SIZE - 1; i >= 0; i--) {
        if (!RL[i].empty()) {
            return RL[i].front();
        }
    }
    return -1;
}

// allocate new PCB[j]
// state = ready
// insert j into children of i
// parent = i
// children = NULL
// resources = NULL
// insert j into RL
// display: “process j created”

bool create(int prio) {
    //find free spot in PCB
    //create process in ready state
    int free = -1;
    for (int i = 0; i < PCB_SIZE; i++)
    {
        if (PCB[i].getState() == -1) {
            free = i;
            break;
        }
    }

    if (free == -1) {
        return false;
    }
    int running = scheduler();
    //PCB[free] = Process(READY, running, prio);
    PCB[free].setState(READY);
    PCB[free].setParent(running);

    PCB[running].addChild(free);
    RL[prio].push_back(free);
    return true;
}

void destroy(int proc) {

}

void request(int res, int n) {

}

void timeout() {
    for (int i = READY_LIST_SIZE - 1; i >= 0; i--) { //find running process
        if (!RL[i].empty()) {
            RL[i].push_back(RL[i].front());
            RL[i].pop_front();
        }
    }
}



int main() {
    cout << "Starting OS" << endl;
    init();

    string line; //tokenize
    getline(std::cin, line);

    while(line != "exit") {
        vector<string> words;
        stringstream stream (line);
        string intermediate;
        while (getline(stream, intermediate, ' ')) {
            words.push_back(intermediate);
        }

        if (words.size() == 1 and words[0] == "in") {
            init();
            cout << endl;
            cout << scheduler() << " ";
        } else if (words.size() == 2 and words[0] == "cr") {

        } else if (words.size() == 1 and words[0] == "to"){
            timeout();
            cout << scheduler() << " ";
        } else {
            cout << ERROR << " ";
        }
        getline(std::cin, line);
    }
    return 0;
}


