#pragma once
#include<string>
#include<fstream>
#include<exception>
using namespace std;
namespace starfarming 
{
namespace utility 
{

#define debug(format, ...) \
	Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define info(format, ...) \
	Logger::instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define warn(format, ...) \
	Logger::instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);

#define error(format, ...) \
	Logger::instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);

class Logger
{
public:

	enum Level
	{
		DEBUG = 0,
		INFO,
		WARN,
		ERROR,
		FATAL,
		LEVEL_COUNT
	};

	// 传入日志级别，日志所在的文件，日志函数坐在的行号，内容使用不定参数来传递
	static Logger* instance();
	void open(const string& filename); // 打开文件
	void close();
	void log(Level level, const char* file, int line, const char* format, ...);
	void level(Level level)
	{
		m_level = level;
	}
	void setMaxLength(int number)
	{
		m_max = number;
	}
private:
	Logger();
	~Logger();
	void rotate();

private:
	string m_filename;
	ofstream m_fout;
	static const char* s_level[LEVEL_COUNT]; // 数字和实际level的映射关系
	static Logger* m_instance; // 实现单例模式
	Level m_level;
	int m_max = 0;
	int m_len = 0;
};

}
}