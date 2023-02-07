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

//for debugging
void printRL() {
    cout << endl << "READY LIST: " << endl;
    for (int i = PRIO_HIGH; i >= PRIO_LOW; i--) {
        cout << "Priority " << i << ": ";
        for (int j = 0; j < RL[i].size(); j++)
        {
            cout << RL[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void printAllChildren() {
    cout << endl << "PCB LIST: " << endl;
    for (int i = 0; i < PCB_SIZE; i++) {
        Process& proc = PCB[i];
        cout << "PCB [" << i << "]: ";
        if (proc.getState() != OPEN) {
            //cout << "READY" << endl;
            for (int j = 0; j < proc.childrenSize(); j++) {
                cout << proc.frontChildren() << " ";
                proc.popChildren();
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Erase all previous contents of the data structures PCB, RCB, RL
// Create a single running process at PCB[0] with priority 0
// Enter the process into the RL at the lowest-priority level 0
void init () {
    for (int i = 0; i < PCB_SIZE; i++) {
        PCB[i].open();
        PCB[i].clearChildren();
        PCB[i].clearResources();
    }

    for(int i = 0; i < RCB_SIZE; i++) {
        RCB[i].clearWaitlist();
        if (i < 2) {
            RCB[i].setInventory(1);
            RCB[i].setState(1);
        } else if (i < 3) {
            RCB[i].setInventory(2);
            RCB[i].setState(2);
        } else {
            RCB[i].setInventory(3);
            RCB[i].setState(3);
        }
    }

    for (int i = 0; i < READY_LIST_SIZE; i++) {
        RL[i].clear();
    }
    PCB[0].ready();
    PCB[0].setParent(-1);
    PCB[0].setPriority(PRIO_LOW);
    RL[0].push_back(PRIO_LOW);
}

int scheduler() {
    for (int i = PRIO_HIGH; i >= PRIO_LOW; i--) {
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
    PCB[free].ready();
    PCB[free].setParent(running);
    PCB[free].setPriority(prio);
    PCB[free].clearChildren();
    PCB[free].clearResources();

    PCB[running].pushChild(free);
    RL[prio].push_back(free);
    return true;
}

bool release(int r, int n) {
    int running = scheduler();
    int rState = RCB[r].getState();
    Process& curr_proc = PCB[running];
    pair<int,int> held;

    auto it = curr_proc.getResources().begin();
    for (; it != curr_proc.getResources().end(); it++) {
        if (it->first == r and it->second == n)
            held = *it;
    }
    if (it == curr_proc.getResources().end())
        return false;
    curr_proc.removeResource(r);
    RCB[r].setState(RCB[r].getState() + n);

    while (!RCB[r].getWaitlist().empty() and rState > 0) {
        pair<int,int> next = RCB[r].getWaitlist().front();
        if (rState >= next.second) {
            RCB[r].setState(RCB[r].getState() - next.second);
            PCB[next.first].addResource(r, next.second);
            PCB[next.first].ready();
            RCB[r].getWaitlist().pop_front();
            RL[PCB[next.first].getPriority()].push_back(next.first);
        } else
            break;
    }

    return true;
}

// for all k in children of j destroy(k)
// remove j from parent's list
// remove j from RL or waiting list
// release all resources of j
// free PCB of j
bool destroy(int proc) {
    //check that process is in bound and in ready list
    printAllChildren();
    if (proc < 0 or proc > PCB_SIZE - 1)
        return false;
    Process& curr_proc = PCB[proc];
    for (int i = 0; i < curr_proc.childrenSize(); i++) {
        int child = curr_proc.frontChildren();
        cout << "Destroying: " << child << endl; 
        curr_proc.popChildren();
        if (destroy(child) == false)
            return false;
    }
    // for (auto it = curr_proc.getChildren().begin(); it != curr_proc.getChildren().end(); it++) { //destroy children
    //     cout << "Destroying: " << *it << endl; 
    //     if (destroy(*it) == false)
    //         return false;
    // }

    //remove from parent list
    PCB[curr_proc.getParent()].removeChild(proc);
    //remove from RL or waiting
    //cout << "State = " << curr_proc.getState() << endl;
    if (curr_proc.getState()) {
        //TODO find out why prio is negative
        //cout << "Prio: " << curr_proc.getPriority() << endl;
        RL[curr_proc.getPriority()].pop_front();
    }
    else {
        //check all resources 
        for (int i = 0; i < curr_proc.waitingSize(); i++) {
            int resource_waiting = curr_proc.frontWaiting();
            curr_proc.popWaiting();
            RCB[resource_waiting].removeFromWaitlist(proc);
        }
    }
    //release all resources
    for (auto it = curr_proc.getResources().begin(); it != curr_proc.getResources().end(); it++) {
        release(it->first, it->second);
    }
    curr_proc.open();
    return true;
}

bool request(int r, int n) {
    int rState = RCB[r].getState();
    int running = scheduler();
    // cout << "Running = " << running << endl;
    // cout << "R = " << r << " " << "N = " << n << endl;
    // cout << "Rstate = " << rState << endl;
    if (rState >= n) {
        RCB[r].setState(rState - n);
        PCB[running].addResource(r, n);
    } else {
        // cout << "Blocking" << endl;
        PCB[running].block();
        for (int i = PRIO_HIGH; i >= PRIO_LOW; i--) { //find running process
            if (!RL[i].empty()) {
                //RL[i].push_back(RL[i].front());
                // cout << "Removing = " << i << endl;
                RL[i].pop_front();
                break;
            }
        }
        RCB[r].addToWaitlist(running, n);
    }
    return true;
}

void timeout() {
    for (int i = PRIO_HIGH; i >= PRIO_LOW; i--) { //find running process
        if (!RL[i].empty()) {
            RL[i].push_back(RL[i].front());
            RL[i].pop_front();
            break;
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
            //printRL();
        } else if (words.size() == 3 and words[0] == "rl"
                    and isnumber(words[1]) and isnumber(words[2])
                    and release(stoi(words[1]), stoi(words[2]))) {
            cout << scheduler() << " ";
        } else if (words.size() == 2 and words[0] == "de"
                    and isnumber(words[1]) and destroy(stoi(words[1]))) {
            //printRL();
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


