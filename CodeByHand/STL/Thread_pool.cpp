#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

class Task {
   public:
    template <typename FUNC_T, typename... ARGS>
    Task(FUNC_T func, ARGS... args) {
        this->func = std::bind(func, std::forward<ARGS>(args)...);
    }
    void run() {
        this->func();
        return;
    }

   private:
    std::function<void()> func;
};

class ThreadPool {
   public:
    ThreadPool(int n = 10) : starting(false), thread_size(n), threads(n) {
        this->start();
    }
    void start() {
        if (starting == true) return;
        for (int i = 0; i < thread_size; ++i) {
            threads[i] = new std::thread(&ThreadPool::worker, this);
        }
        starting = true;
        return;
    }
    void worker() {
        auto id = std::this_thread::get_id();
        running[id] = true;
        while (running[id]) {
            Task* t = get_task();
            t->run();
            delete t;
        }
        return;
    }
    void stop() {
        if (starting == false) return;

        for (int i = 0; i < thread_size; ++i) {
            this->add_task(&ThreadPool::stop_running, this);
        }
        for (int i = 0; i < thread_size; ++i) {
            threads[i]->join();
        }
        for (int i = 0; i < thread_size; ++i) {
            delete threads[i];
            threads[i] = nullptr;
        }
        starting = false;
        return;
    }

    template <typename FUNC_T, typename... ARGS>
    void add_task(FUNC_T func, ARGS... args) {
        std::unique_lock<std::mutex> lock(m_mutex);
        tasks.push(new Task(func, std::forward<ARGS>(args)...));
        m_cond.notify_one();
        lock.unlock();
        return;
    }

    ~ThreadPool() {
        this->stop();
        while (!tasks.empty()) {
            delete tasks.front();
            tasks.pop();
        }
    }

   private:
    bool starting;
    int thread_size;
    std::vector<std::thread*> threads;
    std::queue<Task*> tasks;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::unordered_map<decltype(std::this_thread::get_id()), bool> running;

    void stop_running() {
        auto id = std::this_thread::get_id();
        running[id] = false;
        return;
    }

    Task* get_task() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (tasks.empty()) {
            m_cond.wait(lock);
        }
        Task* t = tasks.front();
        tasks.pop();
        lock.unlock();
        return t;
    }
};