#include <routine/routine.h>
#include <routine/schedule.h>
using namespace yazi::routine;

Routine::Routine() : m_status(RT_READY), m_stack(nullptr), m_stack_size(0)
{
}

Routine::~Routine()
{
    if (m_stack != nullptr)
    {
        delete [] m_stack;
        m_stack = nullptr;
        m_stack_size = 0;
    }
}

void Routine::resume()
{
    Schedule * s = Singleton<Schedule>::instance();
    switch (m_status)
    {
        case RT_READY:
        {
            log_debug("routine=%x status=%d", this, m_status);
            getcontext(&m_ctx);
            m_ctx.uc_stack.ss_sp = s->m_stack;
            m_ctx.uc_stack.ss_size = s->m_stack_size;
            m_ctx.uc_link = &s->m_main;
            m_status = RT_RUNNING;
            makecontext(&m_ctx, (void (*)(void))Routine::func, 1, this);
            swapcontext(&s->m_main, &m_ctx);
            break;
        }
        case RT_SUSPEND:
            log_debug("routine=%x status=%d", this, m_status);
            std::memcpy(s->m_stack + s->m_stack_size - m_stack_size, m_stack, m_stack_size);
            m_status = RT_RUNNING;
            swapcontext(&s->m_main, &m_ctx);
            break;
        default:
            break;
    }
}

void Routine::yield()
{
    auto s = Singleton<Schedule>::instance();
    char dummy = 0;
	assert(s->m_stack + s->m_stack_size - &dummy <= s->m_stack_size);
    if (m_stack_size < s->m_stack + s->m_stack_size - &dummy)
    {
        delete [] m_stack;
        m_stack_size = s->m_stack + s->m_stack_size - &dummy;
        m_stack = new char[m_stack_size];
    }
    std::memcpy(m_stack, &dummy, m_stack_size);
    m_status = RT_SUSPEND;
    s->append(this);
	swapcontext(&m_ctx, &s->m_main);
}

int Routine::status()
{
    return m_status;
}

void Routine::func(void * ptr)
{
    auto c = (Routine *)ptr;
    c->run();
    delete c;
}