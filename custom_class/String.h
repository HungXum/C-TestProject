/*自定义string类*/

/* 
# Copyright (c) 2019 HungXum. All rights reserved. 
*/

#ifndef CUSTOM_CLASS_STRING_H_
#define CUSTOM_CLASS_STRING_H_

#include <assert.h>
#include <stdio.h>

#include <cstring>
#include <iostream>
#include <vector>

const size_t STRING_BUFFER_SIZE = 128;

class String {
    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        assert(str.data_ != nullptr);
        os << str.data_;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, String& str) {
        char chTmp[STRING_BUFFER_SIZE];
        is >> chTmp;
        // 释放掉传递的参数原来指向的内存,由我们下面自定义的构造函数可知,无论如何都会有new空间的,所以必须注意需要释放原来的空间
        if (str.data_) {
            delete []str.data_;
            str.data_ = nullptr;
        }

        str.size_ = strlen(chTmp);
        str.data_ = new char[strlen(chTmp) + 1];
        strncpy(str.data_, chTmp, str.size_);
        str.data_[strlen(chTmp)] = '\0';

        return is;
    }

 public :
    // explicit 修饰只有一个参数的构造函数,表示显示构造函数,不被隐式调用,
    // 在调用operator+时最为明显,比如str1 + "hello";
    // 在显式构造函数中会报错,在隐式构造函数则直接调用构造函数,不说明默认是隐式构造函数.
    explicit String(char *str = nullptr) : size_(0), data_(nullptr) {
        std::cout << "String(char *str = nullptr)\n";
        if (str) {
            size_ = strlen(str);
            data_ = new char[size_ + 1];
            strncpy(data_, str, size_);
        } else {
            data_ = new char[1];
            data_[0] = '\0';
        }
    }

    ~String() {
        std::cout << "~String()\n";
        if (data_) {
            this->size_ = 0;
            delete []data_;
            this->data_ = nullptr;
        }
    }

    String(const String &str) {
        std::cout << "String(const String &str)\n";
        if (this != &str) {
            size_t size = str.size_;
            char *temp = new char[size + 1];
            strncpy(temp, str.data_, size);

            this->data_ = temp;
            this->size_ = size;
        }
    }

    String& operator= (const String &rhs) {
        std::cout << "String& operator= (const String &rhs)\n";
        if (this == &rhs)
            return *this;

        size_t size = rhs.size_;
        char *temp = new char[size + 1];
        strncpy(temp, rhs.data_, size);

        if (data_) {
            delete[]data_;
        }

        this->data_ = temp;
        this->size_ = size;
        return *this;
    }

    // 重载string类的operator+,明白了为什么std::string执行相加时必须左边是string对象而不能是宏或者字符数组或者常量字符串,因为二元运算符+必须是成员函数,则说明调用的左边必须是string对象.
    String operator +(const String &rhs) {
        // 调用String operator +(const char *rhs);代理
        return operator+(rhs.data_);
    }

    // 重载右侧是字符串时操作
    String operator +(const char *rhs) {
        size_t length = size_ + strlen(rhs);
        // 原来使用的是变长数组,但是Google的代码规范不推荐使用变长的数组,推荐使用vector
        // char chTmp[length + 1];
        // strncpy(chTmp, data_, size_);
        // strncpy(chTmp + size_, rhs.data_, rhs.size_);
        // chTmp[length] = '\0';
        std::vector<char> chTemp(length + 1);
        std::memcpy(&chTemp[0], data_, size_);
        std::memcpy(&chTemp[size_], rhs, strlen(rhs));
        chTemp[length] = '\0';

        return String(chTemp.data());
    }

    char& operator[] (size_t index) {
        assert(index >= 0 && index < size_);
        return data_[index];
    }

    size_t length() {
        std::cout << "size_t length()\n";
        return size_;
    }

    char *data() {
        std::cout << "char *data()\n";
        return data_;
    }

 private :
    char  *data_;
    size_t size_;
};
#endif  // CUSTOM_CLASS_STRING_H_
