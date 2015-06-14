#include "stdafx.h"
#include "threadpool.h"
#include <cmath>

ThreadPool::ThreadPool(size_t numThreads)
{
	if (!numThreads) {
		return;
	}

    for (size_t i = 0; i < numThreads; ++i)
    {
        worker_ptr worker(new Worker(tasks));
        worker->run();
        workers.insert(workers.end(),worker);
    }
}

ThreadPool::~ThreadPool()
{
    waitForDoneStop();
}

void ThreadPool::addTask(task_ptr task)
{
    tasks.push(task);
}

void ThreadPool::forceStop()
{
    for (std::list<worker_ptr>::const_iterator it = workers.begin(); it != workers.end(); ++it) {
        (*it)->forceStop();
    }

	tasks.setForceStopFlag(true);
    workers.clear();
}

void ThreadPool::start(size_t numThreads) 
{	
	if (!numThreads) {
		return;
	}

	if (!workers.empty()) {
		std::cout << "Pool is running" << std::endl;
		return;
	}

	for (size_t i = 0; i < numThreads; ++i)
	{
		worker_ptr worker(new Worker(tasks));
		worker->run();
		workers.insert(workers.end(),worker);
	}

	tasks.setForceStopFlag(false);
}

void ThreadPool::waitForDoneStop()
{
    workers.clear();
}

void ThreadPool::setTaskQueueMaxSize(size_t maxSize)
{
    tasks.setTaskQueueMaxSize(maxSize);
}

void ThreadPool::setWorkThreadCount(size_t numThreads)
{
    int count = numThreads - workers.size();

    if (!count) {
        return;
    }
    else if (count < 0) {
        count = abs(count);
        for (size_t i = 0; i < count; i++) {
            workers.erase(workers.begin());
        }
    }
    else if (count > 0) {
        count = abs(count);
        for (size_t i = 0; i < count; i++) {
            worker_ptr worker(new Worker(tasks));
            worker->run();
            workers.insert(workers.end(),worker);
        }
    }
}
