// #include "malloc_free/memory_manager.h"
// #include "malloc_free/smartpointor.h"
#include "malloc_free/String.h"

// void smartpointorTest();
void stringTest();
int main()
{
    // malloc_memory();
    // access_free_data();
    // malloc_mem_control_block();
    // smartpointorTest();
    stringTest();
    
    return 0;
}

// void smartpointorTest() {
//     smartpointor<int> sptr(new int(1024));
//     //调用的是构造函数,没有调用拷贝赋值运算符
//     smartpointor<int> sptr1 = sptr;
//     smartpointor<int> sptr3(sptr);
//     //先调用smartpointor的构造函数,再调用拷贝赋值运算符
//     smartpointor<int> sptr2;
//     sptr2 = sptr;

//     smartpointor<int> sptrAdd = sptr1 + sptr2;
//     cout << "sptr1 + sptr2 = " << *sptrAdd << endl;
//     cout << *sptr << " " << sptr->getCount() << endl;
// }

void stringTest(){
    String str("hello world");
    String str1;
    str1 = str;
    String str2(str);

    //打印:先打印出data(),再打印length();输出栈???
    //char *data()
    //size_t length()
    cout << "length = " << str1.length() << ", date = " << str1.data() << endl;
    cout << "str[" << 1 << "] = " << str[1] << endl;

    String strAdd = str1 + str2;
    String strAdd1 = str1 + " and univers";
    cout << "strAdd = " << strAdd << "; strAdd1 = " << strAdd1 << endl;

    cout << "input" << "strAdd" << endl;
    cin >> strAdd;
    cout << "strAdd = " << strAdd << endl;
}

void memcheckTest() {
    int *ptr = (int*)malloc(sizeof(int) * 4);
    // ptr[4] = 1;
    // cout << ptr[5] << endl;
    free(ptr);
}