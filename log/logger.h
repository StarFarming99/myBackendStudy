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

	// ������־������־���ڵ��ļ�����־�������ڵ��кţ�����ʹ�ò�������������
	static Logger* instance();
	void open(const string& filename); // ���ļ�
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
	static const char* s_level[LEVEL_COUNT]; // ���ֺ�ʵ��level��ӳ���ϵ
	static Logger* m_instance; // ʵ�ֵ���ģʽ
	Level m_level;
	int m_max = 0;
	int m_len = 0;
};

}
}