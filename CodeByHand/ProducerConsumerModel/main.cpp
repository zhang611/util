#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
const int MAX_SIZE = 10;
std::queue<int> q;


void produce (int id) {
    for (int i = 0; i < 30; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {return q.size() < MAX_SIZE;});
        q.push(i);
        std::cout << "produce: " << id << " push " << i << std::endl;
        lock.unlock();
        cv.notify_all();
    }
}

void consumer(int id) {
    for (int i = 0; i < 30; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {return q.size();});
        auto t = q.front();
        q.pop();
        std::cout << "consumer: " << id << " pop " << t << std::endl;
        lock.unlock();
        cv.notify_all();
    }
}


int main() {
    std::thread p1(produce, 1);
    std::thread p2(produce, 2);
    std::thread c1(consumer, 1);
    std::thread c2(consumer, 2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    return 0;

}