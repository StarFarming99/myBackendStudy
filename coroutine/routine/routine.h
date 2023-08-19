#pragma once

#include <ucontext.h>
#include <cstdint>
#include <cstring>
#include <cassert>

#include <utility/logger.h>
#include <utility/singleton.h>
using namespace yazi::utility;

namespace yazi {
namespace routine {

class Schedule;

class Routine
{
public:
    enum Status
    {
        RT_READY = 0,
        RT_RUNNING,
        RT_SUSPEND
    };
    Routine();
    virtual ~Routine();

    virtual void run() = 0;

    void resume();
    void yield();
    int status();

protected:
    static void func(void * ptr);

private:
    ucontext_t m_ctx;
    int m_status;
    char * m_stack;
    int m_stack_size;
};

}
}