#ifndef MyExcepions_hpp
#define MyExcepions_hpp

#include <string>

using namespace std;

class MyException : public exception
{
public:
    MyException(const string& message) : m_message(message) {}
    const char* what() const noexcept override { return m_message.c_str(); }
private:
    string m_message;
};

class NothingFoundException : public MyException
{
public:
    NothingFoundException(const string& message) : MyException(message) {}
};

class AddException : public MyException
{
public:
    AddException(const string& message) : MyException(message) {}
};

class EmptyTableException : public MyException
{
public:
    EmptyTableException(const string& message) : MyException(message) {}
};

class NoPolyException : public MyException
{
public:
    NoPolyException(const string& message) : MyException(message) {}
};

#endif /* MyExcepions_hpp */
