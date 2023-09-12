#pragma once

#include <string.h>
#include <string>
#include <map>
using std::string;

#include <utility/value.h>

namespace yazi {
namespace utility {

typedef std::map<string, Value> Section;

class IniFile
{
public:
    IniFile();
    IniFile(const string & filename);
    ~IniFile();

    bool load(const string & filename);
    void save(const string & filename);
    void show();
    void clear();

    Value & get(const string & section, const string & key);
    void set(const string & section, const string & key, const Value & value);

    bool has(const string & section);
    bool has(const string & section, const string & key);

    void remove(const string & section);
    void remove(const string & section, const string & key);

    Section & operator [] (const string & section)
    {
        return m_sections[section];
    }

    string str();

private:
    string trim(string s);

private:
    string m_filename;
    std::map<string, Section> m_sections;
};

}}
