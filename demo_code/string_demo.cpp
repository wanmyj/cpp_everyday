#include <iostream>
#include <cstring>
using namespace std;

class my_string {
private:
    unsigned int size = 0;
    char* buf = nullptr;
public:
    my_string() : size(0), buf(nullptr) {
        cout<<"empty constructor called"<<endl;
    }
    // 传0试一下，编译器会报错
    my_string(const char* in) : size(strlen(in)), buf(new char[size+1]) {
        cout<<"constructor const char* called"<<endl;
        strcpy(buf, in);
    }
    my_string(nullptr_t) = delete;
    my_string(const my_string& rhs) : size(rhs.size), buf(new char[size+1]) {
        cout<<"copy constructor called"<<endl;
        strcpy(buf, rhs.buf);
    }
    my_string& operator=(const my_string& rhs) {
        cout<<"copy assignment operator called"<<endl;
        if (&rhs != this) {
            __cleanup__();
            size = rhs.size;
            buf = new char[size+1];
            strcpy(buf, rhs.buf);
        }
        return *this;
    }
    my_string(my_string&& rhs) : size(rhs.size), buf(rhs.buf) {
        cout<<"move constructor called"<<endl;
        rhs.size = 0;
        rhs.buf = nullptr;
    }
    my_string& operator=(my_string&& rhs) {
        cout<<"move assignment operator called"<<endl;
        if (&rhs != this) {
            __cleanup__();
            size = rhs.size;
            buf = rhs.buf;
            rhs.size = 0;
            rhs.buf = nullptr;
        }
        return *this;
    }
    operator bool() const noexcept {
        return buf != nullptr;
    }
    ~my_string() {
        __cleanup__();
    }
private:
    void __cleanup__() {
        if (buf != nullptr) {
            delete [] buf;
        }
        size = 0;
    }
};

int main() {
    my_string str1("Hello");
    my_string str2(str1);
    my_string str3;
    str3 = str1;
    my_string str4(std::move(str1));
    my_string str5;
    str5 = std::move(str1);
    if (str1)
        cout<<"str1 is not empty"<<endl;
    else
        cout<<"str1 is empty"<<endl;
    return 0;
}