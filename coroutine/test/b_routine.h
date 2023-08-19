#pragma once

#include <iostream>
#include <routine/routine.h>
using namespace yazi::routine;

class BRoutine : public Routine
{
public:
    BRoutine(int * num) : Routine(), m_num(num) {}
    ~BRoutine()
    {
        if (m_num != nullptr)
        {
            delete m_num;
            m_num = nullptr;
        }
    }

    void run() override
    {
        for (int i = 0; i < *m_num; i++)
        {
            std::cout << "b run: num=" << i << std::endl;
            yield();
        }
    }

private:
    int * m_num;
};