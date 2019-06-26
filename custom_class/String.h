/*自定义string类*/
#ifndef STRING_H
#define STRING_H

#include <cstring>
#include <iostream>
#include <assert.h>
using namespace std;

const size_t STRING_BUFFER_SIZE = 128;

class String {
    friend ostream& operator<<(ostream& os, const String& str) {
        assert(str.m_data != nullptr);
        os << str.m_data;
        return os;
    }
    friend istream& operator>>(istream& is, String& str) {
        char chTmp[STRING_BUFFER_SIZE];
        is >> chTmp;
        //释放掉传递的参数原来指向的内存,由我们下面自定义的构造函数可知,无论如何都会有new空间的,所以必须注意需要释放原来的空间
        if(str.m_data) {
            delete []str.m_data;
            str.m_data = nullptr;
        }

        str.m_size = strlen(chTmp);
        str.m_data = new char[strlen(chTmp) + 1];
        strcpy(str.m_data, chTmp);
        str.m_data[strlen(chTmp)] = '\0';

        return is;
    }
public:
    //explicit 修饰只有一个参数的构造函数,表示显示构造函数,不被隐式调用,在调用operator+时最为明显,比如str1 + "hello";在显式构造函数中会报错,在隐式构造函数则直接调用构造函数,不说明默认是隐式构造函数.
    explicit String(char *str = nullptr) : m_size(0), m_data(nullptr){
        cout << "String(char *str = nullptr)\n";
        if(str) {
            m_size = std::strlen(str);
            m_data = new char[m_size + 1];
            strcpy(m_data, str);
        } else {
            m_data = new char[1];
            m_data[0] = '\0';
        }
    }

    ~String() {
        cout << "~String()\n";
        if(m_data) {
            this->m_size = 0;
            delete []m_data;
            this->m_data = nullptr;
        }
    }

    String(const String &string) {
        cout << "String(const String &string)\n";
        if(this != &string) {
            size_t size = string.m_size;
            char *temp = new char[size + 1];
            strcpy(temp, string.m_data);

            this->m_data = temp;
            this->m_size = size;
        }
    }

    String& operator= (const String &rhs) {
        cout << "String& operator= (const String &rhs)\n";
        if(this == &rhs)
            return *this;

        size_t size = rhs.m_size;
        char *temp = new char[size + 1];
        strcpy(temp, rhs.m_data);

        if(m_data) {
            delete[]m_data;
        }

        this->m_data = temp;
        this->m_size = size;
        return *this;
    }

    //重载string类的operator+,明白了为什么std::string执行相加时必须左边是string对象而不能是宏或者字符数组或者常量字符串,因为二元运算符+必须是成员函数,则说明调用的左边必须是string对象.
    String operator +(const String &rhs) {
        size_t length = m_size + rhs.m_size;
        //用局部变量数组可以保证在拷贝到strTmp后,自动释放掉
        char chTmp[length + 1];
        strncpy(chTmp, m_data, m_size);
        strncpy(chTmp + m_size, rhs.m_data, rhs.m_size);
        chTmp[length] = '\0';

        return String(chTmp);
    }

    //重载右侧是字符串时操作
    String operator +(const char *rhs) {
        size_t length = m_size + strlen(rhs);
        char chTmp[length + 1];
        strncpy(chTmp, m_data, m_size);
        strncpy(chTmp + m_size, rhs, strlen(rhs));
        chTmp[length] = '\0';

        return String(chTmp);
    }

    char& operator[] (size_t index) {
        assert( index >= 0 && index < m_size);
        return m_data[index];
    }

    size_t length() {
        cout << "size_t length()\n";
        return m_size;
    }

    char *data() {
        cout << "char *data()\n";
        return m_data;
    }
private:
    char *m_data;
    size_t m_size;
};
#endif