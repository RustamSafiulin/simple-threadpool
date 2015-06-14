#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

class Task;
typedef boost::function<void()> fn_type;
typedef boost::shared_ptr<Task> task_ptr;

enum PRIORITY {
    LOW_PRIO = 0,
    MEDIUM_PRIO = 1,
    HIGH_PRIO = 2
};

class Task
{
public:
	Task(fn_type fn,size_t priority = MEDIUM_PRIO);
    size_t  getPriority() const;
    fn_type getFn() const;
    bool operator <(const Task &task) const;

private:
    fn_type fn;
    size_t  priority;
};
