#include <iostream>
using namespace std;

#include "singleton.h"
using namespace starfarming::utility;

class A
{
public:
	friend class Singleton<A>;
	void show()
	{
		cout << "hello, I am A!" << endl;
	}
	// 这样就可以隐藏构造函数的同时，可以创建A类
private:
	A(){}
	~A(){}
};


int main()
{
	// 饿汉模式，提前创建，线程安全
	Singleton<A>::getInstance()->show();// 模板类
	return 0;
}