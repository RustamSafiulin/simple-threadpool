#include "stdafx.h"
#include "worker.h"
#include "taskqueue.h"
#include <iostream>

Worker::Worker(TaskQueue &tasks_)
    : tasks(tasks_),done(false)
{
}

Worker::~Worker()
{
	done = true;
	mThread.join();
}

void Worker::forceStop()
{
    done = true;
    mThread.interrupt();
}

void Worker::run()
{
    try {
        mThread = boost::thread(&Worker::processThread,this);
    }
    catch(...)
    {
        done = true;
    }
}

void Worker::processThread()
{
    while (!done) {

        task_ptr task;
        fn_type  fn;

        try {
            boost::this_thread::interruption_point();

			while (!tasks.isEmpty()) {
				boost::this_thread::interruption_point();
				if (tasks.waitTryPop(task)) {
					fn = task->getFn();
					fn();
				}
				else {
					boost::this_thread::yield();
				}
			}
        }
        catch (boost::thread_interrupted &) {
            std::cerr << "Thread: " << boost::this_thread::get_id() << " - interrupted" << std::endl;
            break;
        }
    }
}
