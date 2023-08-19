#pragma once

#include <list>
#include <routine/routine.h>

namespace yazi {
namespace routine {

class Schedule
{
    friend class Routine;
public:
    Schedule();
    ~Schedule();

    void create(int stack_size = 1024 * 1024);
    void append(Routine * c);
    bool empty() const;
    int size() const;

    void resume();

private:
    ucontext_t m_main;
    char * m_stack;
    int m_stack_size;
    std::list<Routine *> m_queue;
};

}
}