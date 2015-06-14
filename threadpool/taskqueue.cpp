#include "stdafx.h"
#include "taskqueue.h"

TaskQueue::TaskQueue()
    : taskQueueMaxSize(40),forceStopFlag(false)
{

}

TaskQueue::~TaskQueue()
{
}

void TaskQueue::setForceStopFlag(bool flag)
{
	forceStopFlag = flag;
}

void TaskQueue::setTaskQueueMaxSize(size_t size)
{
    boost::mutex::scoped_lock locker(mx);
    taskQueueMaxSize = size;
}

void TaskQueue::push(task_ptr task)
{
    boost::unique_lock<boost::mutex> locker(mx);

	if (forceStopFlag) {
		return;
	}

    while (tasks.size() == taskQueueMaxSize) {
		std::cout << "Wait tasks" << std::endl;
        cv.wait(locker);
    }

    tasks.push(task);
    cv.notify_all();
}

bool TaskQueue::waitTryPop(task_ptr &task)
{
    boost::unique_lock<boost::mutex> locker(mx);

    if (!cv.timed_wait(locker,boost::posix_time::milliseconds(1000),taskqueue_not_empty(tasks)))
    {
        return false;
    }

    task = tasks.top();
    tasks.pop();
    cv.notify_all();
    return true;
}

bool TaskQueue::isEmpty()
{
    boost::mutex::scoped_lock locker(mx);
    return tasks.empty();
}

size_t TaskQueue::getTaskQueueSize()
{
    boost::mutex::scoped_lock locker(mx);
    return tasks.size();
}
