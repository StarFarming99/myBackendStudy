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
	// �����Ϳ������ع��캯����ͬʱ�����Դ���A��
private:
	A(){}
	~A(){}
};


int main()
{
	// ����ģʽ����ǰ�������̰߳�ȫ
	Singleton<A>::getInstance()->show();// ģ����
	return 0;
}