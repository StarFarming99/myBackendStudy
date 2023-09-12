#include <thread/task_dispatcher.h>
using namespace yazi::thread;

#include <utility/logger.h>
#include <utility/singleton.h>
using namespace yazi::utility;

TaskDispatcher::TaskDispatcher()
{
}

TaskDispatcher::~TaskDispatcher()
{
}

void TaskDispatcher::init(int threads)
{
    Singleton<ThreadPool>::instance()->create(threads);
    start();
}

void TaskDispatcher::assign(Task* task)
{
    log_debug("task dispatcher assign task: %x", task);
    m_mutex.lock();
    m_tasks.push_back(task);
    m_mutex.unlock();
    m_cond.signal();
}

void TaskDispatcher::handle(Task* task)
{
    // log_debug("task dispatcher handle task");
    ThreadPool * pool = Singleton<ThreadPool>::instance();
    if (!pool->empty())
    {
        pool->assign(task);
    }
    else
    {
        AutoLock lock(&m_mutex);
        m_tasks.push_front(task);
        // log_debug("all threads are busy!");
    }
}

bool TaskDispatcher::empty()
{
    AutoLock lock(&m_mutex);
    return m_tasks.size() == 0;
}

void TaskDispatcher::run()
{
    sigset_t mask;
    if (0 != sigfillset(&mask))
    {
        log_error("thread manager sigfillset failed!");
        return;
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
    {
        log_error("thread manager pthread_sigmask failed!");
        return;
    }
    while (true)
    {
        // log_debug("task list: %d", m_tasks.size());
        m_mutex.lock();
        while (m_tasks.empty())
            m_cond.wait(&m_mutex);
        Task* task = m_tasks.front();
        m_tasks.pop_front();
        m_mutex.unlock();
        handle(task);
    }
}
