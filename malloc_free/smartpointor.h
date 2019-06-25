/*自定义智能指针*/
#ifndef SMARTPOINTOR_H
#define SMARTPOINTOR_H

#include <iostream>
using namespace std;

template<typename T>
class smartpointor {

    //输入输出运算符必须是非成员函数---<<C++Primer>>,因为它们是istream 或 ostream的成员,同时也不能被拷贝
    friend ostream& operator << (ostream& os, const smartpointor<T>& sptr) {
        cout << "friend ostream& operator << (ostream& os, const smartpointor<T>& sptr)" << endl;
        os << *sptr << endl;
        return os;
    }

    // friend smartpointor<T> operator +(const smartpointor<T>& lhs, const smartpointor<T>& rhs) {
    //     cout << "friend smartpointor<T> operator +(const smartpointor<T>& lhs, const smartpointor<T>& rhs)" << endl;
    //     smartpointor<T> sum(new T(*lhs._data + *rhs._data));
    //     return sum;
    // }

    //友元函数必须是有两个参数,二元运算符成员函数只能有形参
    // friend T operator +(const smartpointor<T>& sptr1, const smartpointor<T>& sptr2) {
    //     cout << "friend T operator +(const smartpointor<T>& sptr1, const smartpointor<T>& sptr2)" << endl;
    //     if(sptr1._data != nullptr) {
    //         if(sptr2._data == nullptr) {
    //             return *sptr1._data;
    //         } else {
    //             return *sptr1._data + *sptr2._data;
    //         }
    //     } else if(sptr1._data == nullptr) {
    //         if(sptr2._data == nullptr){
    //             return 0;
    //         } else {
    //             return *sptr2._data;
    //         }
    //     }
    //     return 0;
    // }

public:
    smartpointor(T *_p = nullptr): _data(_p){
        cout << "smartpointor(T *_p = nullptr)\n";
        if(_p)
            _count = new size_t(1);
        else
            _count = new size_t(0);
    }

    ~smartpointor() {
        cout << "~smartpointor()\n";
        (*_count)--;
        if((*_count) <= 0){
            delete _data;
            delete _count;
            cout << "delete _data and _count\n";
        }
    }

    smartpointor(const smartpointor& sptr) {
        cout << "smartpointor(const smartpointor& sptr)\n";
        if(this != &sptr) {
            this->_data = sptr._data;
            this->_count = sptr._count;
            (*(this->_count))++;
        }    
    }

    smartpointor& operator= (const smartpointor& sptr) {
        cout << "smartpointor& operator= (const smartpointor& sptr)\n";
        if(this->_data == sptr._data)
            return *this;

        if(this->_data) {
            (*(this->_count))--;
            if((*(this->_count)) == 0) {
                delete _data;
                delete _count;
            }
        }

        this->_data = sptr._data;
        this->_count = sptr._count;
        (*(this->_count))++;
        return *this;
    }

    //重载解引用运算符
    T& operator* () {
        cout << "T& operator* ()\n";
        return *(this->_data);
    }

    //重载箭头运算符
    smartpointor<T>* operator->() {
        cout << "T* operator->()\n";
        return this;
    }

    //重载 +运算符,二元运算符成员函数只能有右侧参数,默认左侧参数为this,如果两个形参编译报错
    smartpointor<T> operator +(const smartpointor<T>& rhs) {
        cout << "smartpointor<T> operator +(const smartpointor<T>& rhs)" << endl;
        smartpointor<T> sum(new T(*_data + *rhs._data));
        return sum;
    }

    T* get() const{
        cout << "T *get() const\n";
        return _data;
    }

    size_t getCount() const {
        cout << "size_t getCount() const\n";
        return *_count;
    }
private:
    T      *_data;
    size_t *_count;
};

// template<typename T>
// //输入输出流必须重载为友元函数,如果不在类内定义友元运算符函数,会一直报警告
// ostream& operator << (ostream& os, const smartpointor<T>& sptr) {
//     os << *sptr << endl;
//     return os;
// }

// template<typename T>
// T operator +(const smartpointor<T>& sptr1, const smartpointor<T>& sptr2) {
//     if(sptr1._data != nullptr) {
//         if(sptr2._data == nullptr) {
//             return *sptr1._data;
//         } else {
//             return *sptr1._data + *sptr2._data;
//         }
//     } else if(sptr1._data == nullptr) {
//         if(sptr2._data == nullptr){
//             return 0;
//         } else {
//             return *sptr2._data;
//         }
//     }
//     return 0;
// }



/************************************************
tips:关于重载运算符
1.重载运算符函数的参数数量与该运算符作用的运算对象数量一样多.一元运算符有一个参数,二元运算符有两个.如果一个运算符函数是一个成员函数,则它的第一个(左侧)运算对象绑定到隐式的this指针上,因此,成员运算符函数的(显式)参数数量比运算符的运算对象总数少一个.
2.输入输出运算符必须是'非成员函数'
3.下标运算符[],必须是'成员函数',vector中就是重载了operator[]
*************************************************/
#endif