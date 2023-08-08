#include <iostream>
using namespace std;

#include "logger.h"
using namespace starfarming::utility;

int main()
{

    Logger::instance()->open("./test.log");
    Logger::instance()->level(Logger::INFO); // 低于info会被丢弃
    Logger::instance()->setMaxLength(1024); // 设置日志的最大长度
    // __FILE__ :替换为当前的源文件名
    //Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, "hello world");
    //Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, "name is %s, age is %d", "jack", 18);
    try
    {
        debug("test wtf lalala %d", 1119);
        info("test wtf lalala %d", 1024);
        warn("test wtf lalala %d", 1999);
        error("test wtf lalala %d", 0712);
    }
    catch (const std::logic_error& e)
    {
        cout << e.what() << endl;
        return 1;
    }
    


    return 0;
}