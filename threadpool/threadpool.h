#pragma once

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <list>
#include "worker.h"

typedef boost::shared_ptr <Worker> worker_ptr;

class ThreadPool : public boost::noncopyable
{

public:
    ThreadPool(size_t numThreads = boost::thread::hardware_concurrency());
    ~ThreadPool();
    void waitForDoneStop();
    void forceStop();
	void start(size_t numThreads = boost::thread::hardware_concurrency());
    void setWorkThreadCount(size_t count);
    void addTask(task_ptr task);
    void setTaskQueueMaxSize(size_t maxSize);

private:
    std::list <worker_ptr> workers;
    TaskQueue              tasks;
};
