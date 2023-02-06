#include <string.h>
#include <iostream>
#include <vector>
#include <deque>
#include <bits/stdc++.h>
#include "process.hpp"
#include "resource.hpp"
#include "helper.hpp"
using namespace std;

#define PCB_SIZE 16
#define RCB_SIZE 4
#define READY_LIST_SIZE 3
#define ERROR -1
#define PRIO_LOW 0
#define PRIO_HIGH 2

static Process PCB[PCB_SIZE];
static Resource RCB[RCB_SIZE];
static std::deque<int> RL[READY_LIST_SIZE];

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
        //clear waitlist
    }

    for (int i = 0; i < READY_LIST_SIZE; i++) {
        RL[i].clear();
    }
    PCB[0].setState(READY);
    PCB[0].setParent(-1);
    PCB[0].setPriority(PRIO_LOW);
    RL[0].push_back(PRIO_LOW);
}

int scheduler() {
    for (int i = READY_LIST_SIZE - 1; i >= 0; i--) {
        if (!RL[i].empty()) {
            return RL[i].front();
        }
    }
    return ERROR;
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
    if (prio < PRIO_LOW or prio > PRIO_HIGH)
        return false;
    int free = -1;
    for (int i = 0; i < PCB_SIZE; i++)
    {
        if (PCB[i].getState() == -1) {
            free = i;
            break;
        }
    }

    if (free == -1)
        return false;

    int running = scheduler();
    PCB[free].setState(READY);
    PCB[free].setParent(running);
    PCB[free].clearChildren();
    PCB[free].clearResources();

    PCB[running].addChild(free);
    RL[prio].push_back(free);
    return true;
}

bool destroy(int proc) {
    Process& dest = PCB[proc];
    // for (int i = 0; i < dest.childrenSize(); i++) {
    //     int child = dest.getChild(i);
    //     if (child == -1)
    //         return false;
    //     if (destroy(child) == false)
    //         return false;
    // }
    for (auto it = dest.getChildren().begin(); it != dest.getChildren().end(); it++) {
        if (destroy(*it) == false)
            return false;
    }
    //remove from parent list
    PCB[dest.getParent()].removeChild(proc);
    //remove from RL or waiting
    if (dest.getState())
        RL[dest.getPriority()].pop_front();
    else {

    }
    //release all resources
    //for ()
    dest.setState(FREE);
    return true;
}

bool request(int res, int n) {
    return true;
}

bool release(int res, int n) {
    return true;
}

void timeout() {
    for (int i = PRIO_HIGH; i >= PRIO_LOW; i--) { //find running process
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

    while(line != "q") {
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
        } else if (words.size() == 2 and words[0] == "cr" 
                    and isnumber(words[1]) and create(stoi(words[1]))) {
            cout << scheduler() << " ";
        } else if (words.size() == 1 and words[0] == "to"){
            timeout();
            cout << scheduler() << " ";
        } else if (words.size() == 3 and words[0] == "rq"
                    and isnumber(words[1]) and isnumber(words[2])
                    and request(stoi(words[1]), stoi(words[2]))) {
            cout << scheduler() << " ";
        } else if (words.size() == 3 and words[0] == "rl"
                    and isnumber(words[1]) and isnumber(words[2])
                    and release(stoi(words[1]), stoi(words[2]))) {
            cout << scheduler() << " ";
        } else if (words.size() == 2 and words[0] == "de"
                    and isnumber(words[1]) and destroy(stoi(words[1]))) {
            cout << scheduler() << " ";
        } else {
            cout << ERROR << " ";
        }
        getline(std::cin, line);
        if (!cin)
            break;
    }
    return 0;
}


