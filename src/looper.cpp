#include "looper.h"

#include <Arduino.h>

Looper::Looper() {}

void Looper::RunTask(TaskWithParams &task_with_param)
{
    delay(1); // feed watch dog every task
    unsigned long start_time, cost;
    start_time = millis();
    task_with_param.func(task_with_param.params);
    cost = millis() - start_time;
    if (cost > 150)
    {
        // record long time task
        log_d("func() cost %dms func=%p(%s), params=%p", cost, task_with_param.func, task_with_param.func_name_for_debug, task_with_param.params);
    }
}

void Looper::LoopForever()
{
    TaskWithParams task_with_param = {.func = NULL, .func_name_for_debug = "", .params = NULL};
    while (1)
    {
        // run all high priority queue items first, using while
        while (xQueueReceive(message_queue_high_priority_, &task_with_param, 0) == pdPASS)
        {
            RunTask(task_with_param);
        }
        // then run normal priority queue items, using if
        if (xQueueReceive(message_queue_normal_priority_, &task_with_param, 0) == pdPASS)
        {
            RunTask(task_with_param);
        }
    }
}

bool Looper::PostTask(TaskFunc func, void *params, const char *func_name_for_debug, bool is_high_priority)
{
    TaskWithParams task_with_param = {.func = func, .func_name_for_debug = func_name_for_debug, .params = params};
    if (is_high_priority)
    {
        return xQueueSend(message_queue_high_priority_, &task_with_param, 0) == pdPASS;
    }
    else
    {
        return xQueueSend(message_queue_normal_priority_, &task_with_param, 0) == pdPASS;
    }
}

TaskRunner::TaskRunner(Looper *looper) : looper_(looper) {}

bool TaskRunner::PostTask(TaskFunc func, void *params, const char *func_name_for_debug)
{
    return looper_->PostTask(func, params, func_name_for_debug, false);
}

bool TaskRunner::PostHighPriorityTask(TaskFunc func, void *params, const char *func_name_for_debug)
{
    return looper_->PostTask(func, params, func_name_for_debug, true);
}
