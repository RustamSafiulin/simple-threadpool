#include "stdafx.h"
#include "task.h"

Task::Task(fn_type fn_, size_t priority_)
    : fn(fn_),priority(priority_)
{
}

size_t Task::getPriority() const
{
    return priority;
}

bool Task::operator <(const Task &task) const
{
    return getPriority() < task.getPriority();
}

fn_type Task::getFn() const
{
    return fn;
}
