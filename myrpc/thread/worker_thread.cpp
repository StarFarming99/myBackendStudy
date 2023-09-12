#include <thread/worker_thread.h>
#include <thread/task.h>
#include <thread/thread_pool.h>
using namespace yazi::thread;

#include <utility/logger.h>
#include <utility/singleton.h>
using namespace yazi::utility;

WorkerThread::WorkerThread() : Thread()
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::cleanup(void* ptr)
{
    log_info("worker thread cleanup handler: %x", ptr);
}

void WorkerThread::run()
{
    sigset_t mask;
    if (0 != sigfillset(&mask))
    {
        log_error("worker thread sigfillset faile!");
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
    {
        log_error("worker thread pthread_sigmask failed");
    }
    pthread_cleanup_push(cleanup, this);

    while (true)
    {
        m_mutex.lock();
        while (m_task == nullptr)
            m_cond.wait(&m_mutex);
        m_mutex.unlock();

        int rc = 0;
        int old_state = 0;
        rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);

        log_debug("worker thread run: thread=%x, task=%x", this, m_task);
        m_task->run();
        m_task->destroy();
        m_task = nullptr;

        Singleton<ThreadPool>::instance()->put(this);

        rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);
        pthread_testcancel(); // cancel-point
    }
    pthread_cleanup_pop(1);
}
