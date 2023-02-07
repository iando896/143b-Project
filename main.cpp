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
        cout << "PCB [" << i << "] children: ";
        proc.printChildren();
    }
}

void printAllResources() {
    cout << endl << "RCB LIST: " << endl;
    for (int i = 0; i < RCB_SIZE; i++) {
        Resource& res = RCB[i];
        cout << "RCB [" << i << "]: " << endl;
        //proc.printChildren();
        res.printResource();
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

//need to be able to release from other processes
bool release(int r, int n, int proc) {
    //cout << "Releasing:" << r << " of " << n << endl;
    //int running = scheduler();
    int rState = RCB[r].getState();
    Process& curr_proc = PCB[proc];
    pair<int,int> held;
    unordered_map<int,int> procResources = curr_proc.getResources();
    auto it = procResources.begin();
    for (; it != procResources.end(); it++) {
        //cout << it->first << " " << it->second << endl;
        if (it->first == r and it->second == n) {
            held = *it;
            break;
        }
    }
    if (it == procResources.end())
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
    //Process does not need to be currently running
    //cout << "Destroying: " << proc << endl; 
    if (proc < 0 or proc > PCB_SIZE - 1)
        return false;

    int running = scheduler();

    if (!PCB[running].findChild(proc) and proc != running)
        return false;
    
    Process& dest_proc = PCB[proc];

    if (dest_proc.getState() == OPEN)
        return false;
    //cout << dest_proc.childrenSize() << endl;
    int child_list_size = dest_proc.childrenSize();
    for (int i = 0; i < child_list_size; i++) {
        int child = dest_proc.frontChildren();
        dest_proc.popChildren();
        if (destroy(child) == false)
            return false;
    }
    //cout << endl;
    //remove from parent list
    PCB[dest_proc.getParent()].removeChild(proc);
    //remove from RL or waiting
    //cout << "State = " << dest_proc.getState() << endl;
    if (dest_proc.getState()) {
        //cout << "Prio: " << dest_proc.getPriority() << endl;
        //cout << "Removing: " << RL[dest_proc.getPriority()].front() << endl;
        //have to delete correct proc
        //RL[dest_proc.getPriority()].pop_front();
        for (auto it = RL[dest_proc.getPriority()].begin(); it != RL[dest_proc.getPriority()].end(); it++) {
            if (*it == proc) {
                RL[dest_proc.getPriority()].erase(it);
                break;
            }
        }
    }
    else {
        //check all resources
        //cout << "Remove from waiting" << endl;
        int waiting_size = dest_proc.waitingSize();
        //cout << "Size: " << waiting_size << endl;
        for (int i = 0; i < waiting_size; i++) {
            int resource_waiting = dest_proc.frontWaiting();
            dest_proc.popWaiting();
            if (!RCB[resource_waiting].removeFromWaitlist(proc))
                return false;
        }
    }
    //release all resources
    unordered_map<int,int> procResources = dest_proc.getResources();
    for (auto it = procResources.begin(); it != procResources.end(); it++) {
        if (!release(it->first, it->second, proc))
            return false;
    }
    dest_proc.open();
    //cout << "Done destroying" << endl;
    return true;
}

//can only be requested by current thread
bool request(int r, int n) {
    if (r < 0 or r > RCB_SIZE - 1 or n > RCB[r].getInventory())
        return false;
    
    int running = scheduler();
    if (running == 0)
        return false;
    
    Process& curr_proc = PCB[running];
    unordered_map<int,int> procResources = curr_proc.getResources();
    auto it = procResources.begin();
    for (; it != procResources.end(); it++) {
        //cout << it->first << " " << it->second << endl;
        if (it->first == r and it->second == n) {
            return false;
        }
    }

    int rState = RCB[r].getState();
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
        PCB[running].pushWaiting(r);
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
    //cout << "Starting OS" << endl;
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
            //printAllChildren();
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
                    and release(stoi(words[1]), stoi(words[2]), scheduler())) {
            cout << scheduler() << " ";
        } else if (words.size() == 2 and words[0] == "de"
                    and isnumber(words[1]) and destroy(stoi(words[1]))) {
            //printRL();
            //printAllChildren();
            cout << scheduler() << " ";
        } else {
            cout << ERROR << " ";
        }
        //printAllResources();
        getline(std::cin, line);
        if (!cin)
            break;
    }
    return 0;
}


