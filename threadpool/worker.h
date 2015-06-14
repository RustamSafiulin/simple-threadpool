#pragma once

#include <boost/thread.hpp>
#include "taskqueue.h"

class Worker : public boost::noncopyable
{
public:
    Worker(TaskQueue &tasks_);
    ~Worker();
    void run();
    void forceStop();

private:
    void processThread();
    bool          done;
    boost::thread mThread;
    TaskQueue     &tasks;
};
