#include <iostream>
#include <vector>
#include <shared_mutex>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;


vector<int> v = {1, 2, 3, 4, 5};
shared_mutex m;
void read (int id) {
    shared_lock<shared_mutex> lock(m);
    cout << "Read Thread " << id << " read: ";
    for (int i : v) cout << i << " ";
    cout << endl;
}

void write (int id) {
    unique_lock<shared_mutex> lock(m);
    cout << "Write Thread " << id << " write: ";
    v.pop_back();
    cout << endl;
}

int a = 100;

// int main() {
//     thread t1(read, 1);
//     thread t2(write, 2);
//     thread t3(read, 3);
//     thread t4(read, 4);
//     thread t5(write, 5);
//     thread t6(read, 6);

//     t1.join();
//     t2.join();
//     t3.join();
//     t4.join();
//     t5.join();
//     t6.join();

//     return 0;

// }