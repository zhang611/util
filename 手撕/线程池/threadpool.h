#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

template <typename T>
class BlockingQueue {
public:
    BlockingQueue(bool nonblock = false) : nonblock_(nonblock) {}
    // 入队操作
    void Push(const T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
        not_empty_.notify_one();
    }

    // 出队操作
    bool Pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        // 条件变量等待，直到队列不为空或者非阻塞模式
        // this 指针是一个指向当前对象的指针，用于在 lambda 表达式中访问当前对象的成员
        not_empty_.wait(lock, [this] { return !queue_.empty() || nonblock_; });
        if (queue_.empty())
            return false;

        value = queue_.front();
        queue_.pop();
        return true;
    }

    // 解除阻塞在当前队列的线程
    void Cancel() {
        std::lock_guard<std::mutex> lock(mutex_);
        nonblock_ = true;
        not_empty_.notify_all();
    }
private:
    bool nonblock_;
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable not_empty_;   // 队列状态，空就休眠
};


// 使用两个队列实现生产者消费者模型,基于workflow的线程池
template <typename T>
class BlockingQueuePro {
public:
    BlockingQueuePro(bool nonblock = false) : nonblock_(nonblock) {}
    // 入队操作
    void Push(const T &value) {
        std::lock_guard<std::mutex> lock(producer_mutex_);
        producer_queue_.push(value);
        not_empty_.notify_one();
    }

    // 出队操作
    bool Pop(T& value) {
        std::unique_lock<std::mutex> lock(consumer_mutex_);
        if (consumer_queue_.empty() && SwapQueue_() == 0) {
            return false;
        }
        value = consumer_queue_.front();
        consumer_queue_.pop();
        return true;
    }

    // 解除阻塞在当前队列的线程
    void Cancel() {
        std::lock_guard<std::mutex> lock(producer_mutex_);
        nonblock_ = true;
        not_empty_.notify_all();
    }

private:

    // 当消费者队列为空时，交换生产者和消费者队列
    int SwapQueue_() {
        std::unique_lock<std::mutex> lock(producer_mutex_);
        not_empty_.wait(lock, [this] { return !producer_queue_.empty() || nonblock_; });
        std::swap(producer_queue_, consumer_queue_);
        return consumer_queue_.size();
    }

    bool nonblock_;
    std::queue<T> producer_queue_;
    std::queue<T> consumer_queue_;
    std::mutex producer_mutex_;
    std::mutex consumer_mutex_;
    std::condition_variable not_empty_;
};

class ThreadPool {
public:
    // 构造函数，初始化线程池
    // 根据线程数量创造线程
    explicit ThreadPool(int num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] { Worker(); });
        }
    }

    // 析构函数，停止线程池
    ~ThreadPool() {
        task_queue_.Cancel();
        for (auto &worker : workers_) {
            if (worker.joinable())
                worker.join();
        }
    }

    // 生产者发布任务到线程池，可以提交任意可调用对象
    // F 表示一个可调用对象(函数、lambda 表达式、函数对象等)，Args 表示函数对象的参数类型
    // Args... 是一个可变参数模板，表示可以接受任意数量的参数
    template<typename F, typename... Args>
    void Post(F &&f, Args &&...args) {
        // 把 f 和 args...绑定成一个可调用对象，并放入任务队列
        // 函数名+参数集成起来, 变成无参数的可调用对象，符合线程池任务队列存储的类型
        // 完美转发，确保 f 和 args 保持原始的 左值/右值 属性
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        task_queue_.Push(task);
    }

private:
    // 工作线程入口函数
    // 线程不断从任务队列 task_queue_ 获取任务并执行
    void Worker() {
        while (true) {
            std::function<void()> task;
            if (!task_queue_.Pop(task))
                break;
            task();
        }
    }

    BlockingQueue<std::function<void()>> task_queue_;   // 任务队列
    std::vector<std::thread> workers_;                  // 工作线程
};