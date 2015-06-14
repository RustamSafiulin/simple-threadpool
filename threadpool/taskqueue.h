#pragma once

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <queue>
#include "task.h"

struct taskqueue_not_empty
{
    std::priority_queue<task_ptr> &tasks;

    taskqueue_not_empty(std::priority_queue<task_ptr> &tasks_)
        : tasks(tasks_)
    {}

    bool operator()() const
    {
        return !tasks.empty();
    }
};

class TaskQueue : boost::noncopyable
{
public:
    TaskQueue();
    ~TaskQueue();
    void	push(task_ptr task);
    void	setTaskQueueMaxSize(size_t size);
	void	setForceStopFlag(bool flag);
    bool    waitTryPop(task_ptr &task);
    bool    isEmpty();
    size_t  getTaskQueueSize();


private:
    std::priority_queue <task_ptr> tasks;
    size_t                         taskQueueMaxSize;
    boost::mutex                   mx;
    boost::condition_variable      cv;
	bool						   forceStopFlag;
};
