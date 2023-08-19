#include <iostream>

#include <routine/schedule.h>
using namespace yazi::routine;

#include <test/a_routine.h>
#include <test/b_routine.h>

int main()
{
    Logger::instance()->open("./main.log");
    Logger::instance()->console(false);

    auto s = Singleton<Schedule>::instance();
    s->create();

    int * num1 = new int;
    *num1 = 5;
    Routine * a = new ARoutine(num1);
    s->append(a);

    int * num2 = new int;
    *num2 = 10;
    Routine * b = new BRoutine(num2);
    s->append(b);

    while (!s->empty())
    {
        std::cout << "main run" << std::endl;
        s->resume();
    }
    return 0;
}
