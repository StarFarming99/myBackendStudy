#include <thread/thread_pool.h>
#include <thread/worker_thread.h>
using namespace yazi::thread;

#include <utility/logger.h>
using namespace yazi::utility;

ThreadPool::ThreadPool() : m_threads(0)
{
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::create(int threads)
{
    AutoLock lock(&m_mutex);
    m_threads = threads;
    for (int i = 0; i < threads; i++)
    {
        Thread* thread = new WorkerThread();
        log_debug("create thread %x", thread);
        m_pool.push_back(thread);
        thread->start();
    }
}

Thread* ThreadPool::get()
{
    AutoLock lock(&m_mutex);
    while (m_pool.size() == 0)
        m_cond.wait(&m_mutex);
    Thread * thread = m_pool.front();
    m_pool.pop_front();
    return thread;
}

void ThreadPool::put(Thread *thread)
{
    AutoLock lock(&m_mutex);
    m_pool.push_back(thread);
    m_cond.signal();
}

bool ThreadPool::empty()
{
    AutoLock lock(&m_mutex);
    return m_pool.size() == 0;
}

void ThreadPool::assign(Task *task)
{
    if (task == nullptr)
    {
        log_error("assign a null task to thread pool");
        return;
    }

    log_debug("assign a task: %x to thread pool", task);
    Thread* thread = get();
    if (thread != nullptr)
    {
        thread->set_task(task);
    }
    else
    {
        log_error("thread is null, assign a task failed");
    }
}
