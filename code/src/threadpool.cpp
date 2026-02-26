#include "threadpool.h"

ThreadPool::ThreadPool(unsigned int _threadCount)
	: isThreadPoolActive(true)
{
    if (_threadCount == 0)
        _threadCount = 1;

    // Reserve space in the vector for the given number of threads (workers).
    workers.reserve(_threadCount);
    for (unsigned int i = 0; i < _threadCount; ++i)
        workers.emplace_back(&ThreadPool::WorkerLoop, this);
}

ThreadPool::~ThreadPool()
{
    // Flag the threadpool as inactive so the threads stop.
    isThreadPoolActive = false;

    // Notify all threads, including the "sleeping" ones.
    spinLockCondition.notify_all();

    // Wait for every thread to finish.
    for (int i = 0; i < workers.size(); i++)
    {
        if (workers[i].joinable())
            workers[i].join();
    }
}

void ThreadPool::Submit(std::function<void()> _job)
{
    // Lock the jobs queue in a local scope.
    {
        std::lock_guard<std::mutex> lock(jobsQueueMutex);
        jobs.push(std::move(_job));
    }

    // Wakes up one of the workers waiting for a job.
    spinLockCondition.notify_one();
}

void ThreadPool::WorkerLoop()
{
    while (isThreadPoolActive)
    {
        std::function<void()> job;
        // Lock the jobs queue in a local scope.
        {
            // Acquire the jobs queue and spin lock the thread as long as we don't have a job to accomplish.
            std::unique_lock<std::mutex> lock(jobsQueueMutex);
            spinLockCondition.wait(lock, [this]()
                {
                    return !jobs.empty() || !isThreadPoolActive;
                });

            // If the pool is not active anymore, we exit the thread.
            if (!isThreadPoolActive)
                return;

            // Fetch the next job in queue.
            job = std::move(jobs.front());
            jobs.pop();
        }

        // Execute the job we fetched.
        job();
    }
}
