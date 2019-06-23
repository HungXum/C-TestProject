/*自定义智能指针*/

#include <iostream>
using namespace std;

template<typename T>
class smartpointor {
public:
    smartpointor(T *_p = nullptr): _data(_p){
        cout << "smartpointor(T *_p = nullptr)\n";
        if(_p)
            _count = 1;
        else
            _count = 0;
    }

    ~smartpointor() {
        cout << "~smartpointor()\n";
        _count--;
        if(_count <= 0){
            delete _data;
            cout << "delete _data\n";
        }
    }

    smartpointor(const smartpointor& sptr) {
        cout << "smartpointor(const smartpointor& sptr)\n";
        if(this != &sptr) {
            this->_data = sptr._data;
            this->_count = sptr._count;
            (this->_count)++;
        }    
    }

    smartpointor& operator= (const smartpointor& sptr) {
        cout << "smartpointor& operator= (const smartpointor& sptr)\n";
        if(this->_data == sptr._data)
            return *this;

        if(this->_data) {
            this->_count--;
            if(this->_count == 0)
                delete _data;
        }

        this->_data = sptr._data;
        this->_count = sptr._count;
        this->_count++;
        return *this;
    }

    T& operator* (){
        cout << "T& operator* ()\n";
        return *(this->_data);
    }

    T* operator->() {
        cout << "T* operator->()\n";
        return this->_data;
    }

    T *get() const{
        cout << "T *get() const\n";
        return _data;
    }

    size_t getCount() const {
        cout << "size_t getCount() const\n";
        return _count;
    }
private:
    T     *_data;
    size_t _count;
};