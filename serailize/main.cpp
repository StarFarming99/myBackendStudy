#include <iostream>

#include "data_stream.h"
#include "serializable.h"
using namespace yazi::serialize;

class A : public Serializable
{
public:
    A() {}
    A(const string& name, int age) : m_name(name), m_age(age) {}
    ~A() {}

    void show()
    {
        std::cout << m_name << "," << m_age << std::endl;
    }

    SERIALIZE(m_name, m_age)

private:
    string m_name;
    int m_age;
};

int main()
{
    DataStream ds;
    ds << "hello" << "kitty";
    ds.show();

    std::cout << ds.size() << std::endl;

    return 0;
}