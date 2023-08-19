#include <routine/schedule.h>
using namespace yazi::routine;

Schedule::Schedule() : m_stack(nullptr)
{
}

Schedule::~Schedule()
{
    for (auto it : m_queue)
    {
        delete it;
    }
    m_queue.clear();
    if (m_stack != nullptr)
    {
        delete [] m_stack;
        m_stack_size = 0;
    }
}

void Schedule::create(int stack_size)
{
    m_stack_size = stack_size;
    m_stack = new char[stack_size];
    std::memset(m_stack, 0, stack_size);
}

void Schedule::append(Routine * c)
{
    m_queue.push_back(c);
}

bool Schedule::empty() const
{
    return m_queue.empty();
}

int Schedule::size() const
{
    return m_queue.size();
}

void Schedule::resume()
{
    if (m_queue.empty())
    {
        return;
    }
    auto c = m_queue.front();
    m_queue.pop_front();
    c->resume();
}