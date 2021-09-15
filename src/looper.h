#ifndef __MAIN_LOOPER_H
#define __MAIN_LOOPER_H

#include <Arduino.h>

typedef void (*TaskFunc)(void *);

struct TaskWithParams
{
    TaskFunc func;
    const char *func_name_for_debug;
    void *params;
};

class Looper
{
public:
    Looper();
    void LoopForever();
    bool PostTask(TaskFunc func, void *params, const char *func_name_for_debug, bool is_high_priority);

private:
    const size_t MESSAGE_QUEUE_LENGTH = 32;
    QueueHandle_t message_queue_normal_priority_ = xQueueCreate(MESSAGE_QUEUE_LENGTH, sizeof(TaskWithParams));
    QueueHandle_t message_queue_high_priority_ = xQueueCreate(MESSAGE_QUEUE_LENGTH, sizeof(TaskWithParams));
    void RunTask(TaskWithParams &task_with_param);
};

class TaskRunner
{
public:
    TaskRunner(Looper *looper);
    bool PostTask(TaskFunc func, void *params, const char *func_name_for_debug);
    bool PostHighPriorityTask(TaskFunc func, void *params, const char *func_name_for_debug);

private:
    Looper *looper_;
};

#endif //__SYSTEMINIT_H