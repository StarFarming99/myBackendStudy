#include <thread/task.h>
#include <thread/auto_lock.h>

using namespace yazi::thread;

Task::Task() : m_data(nullptr)
{
}

Task::Task(void* data) : m_data(data)
{
}

Task::~Task()
{
}

void* Task::get_data()
{
    AutoLock lock(&m_mutex);
    return m_data;
}

void Task::set_data(void *data)
{
    AutoLock lock(&m_mutex);
    m_data = data;
}
