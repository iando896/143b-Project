#include <string.h>
#include <iostream>
#include <vector>
#include "Process.hpp"
using namespace std;

#define PCB_SIZE 16
#define RCB_SIZE 4
#define READY_LIST_SIZE 3


class Resource {
private:
    int state;
    vector<int> waitlist;
public:
    Resource(int st) {
        state = st;
    }

};

Process* PCB = new Process[PCB_SIZE];
vector<vector<int>> ready_list (READY_LIST_SIZE, vector<int>());

int init () {
// Erase all previous contents of the data structures PCB, RCB, RL
// Create a single running process at PCB[0] with priority 0
// Enter the process into the RL at the lowest-priority level 0

    PCB[0] = Process(0,0);
    return 0;
}

int main(int argc, char * argv[]) {
    cout << "Hello world" << endl;
    init();
    //delete PCB;
    return 0;
}


