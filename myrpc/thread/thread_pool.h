#pragma once

#include <list>
#include <thread/thread.h>
#include <thread/mutex.h>
#include <thread/auto_lock.h>
#include <thread/condition.h>
#include <thread/task.h>

namespace yazi {
namespace thread {

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    void create(int threads);

    Thread* get();
    void put(Thread * thread);
    bool empty();
    void assign(Task* task);

private:
    int m_threads;
    std::list<Thread *> m_pool;
    Mutex m_mutex;
    Condition m_cond;
};

}}
