#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool
{
public:
    // CONSTRUCTOR & DESTRUCTOR

    ThreadPool(unsigned int _threadCount = std::thread::hardware_concurrency());
    ThreadPool(const ThreadPool& _copy) = delete;
    ~ThreadPool();


    // OPERATORS

    ThreadPool& operator=(const ThreadPool& _copy) = delete;


    // PUBLIC METHODS

    // Add a job to the queue of jobs.
    void Submit(std::function<void()> _job);

private:
    // PROPERTIES

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;

    std::mutex jobsQueueMutex;
    std::condition_variable spinLockCondition;

    std::atomic<bool> isThreadPoolActive;


    // WORKERS

    void WorkerLoop();
};
