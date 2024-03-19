#include <iostream>
#include <cstring>
class String {
    unsigned m_length;
    char* m_pData;
public:
    //通用构造函数
    String(const char * in = "") : m_length(strlen(in)), m_pData(new char[m_length+1]) {
        strcpy(m_pData, in);
    }
    // 拷贝构造函数
    String(const String& in) : m_length(in.m_length), m_pData(new char[m_length+1]) {
        strcpy(m_pData, in.m_pData);
    }
    // 赋值符号函数  !!！一定判断是否是自身,一定要delete，而且要用[]
    String& operator=(const String& rhs) {
        if (&rhs != this) {
            m_length = rhs.m_length;
            delete[] m_pData;
            m_pData = new char[m_length+1];
            strcpy(m_pData, rhs.m_pData);
        }
        return *this;
    }
    // 移动拷贝构造函数 ！！！一定要加noexcept,
    String(String&& in) noexcept : m_length(in.m_length), m_pData(in.m_pData) {
        in.m_pData = nullptr;
        in.m_length = 0;
    }
    // 移动赋值符号函数 !!！一定要加noexcept,判断是否是自身,一定要delete，而且要用[]
    String& operator=(String&& rhs) noexcept {
        if (&rhs != this) {
            delete [] m_pData;
            m_length = rhs.m_length;
            m_pData = rhs.m_pData;
            rhs.m_pData = nullptr;
            rhs.m_length = 0;
        }
        return *this;
    }
    // 实现if(ptr)
    explicit operator bool() const noexcept {
        return m_pData != nullptr;
    }
    //析构函数
    ~String() {
        if (!m_pData)
            delete[] m_pData;
    }
};

class Solution {
public:
    bool divisorGame(int n) {
        int memo[n+1];
        memo[0] = false;
        memo[1] = false;
        dfs(n, memo);
    }
    void dfs(int n, int * memo) {

    }
};

int main() {
    String s1("Hello");
    String s2(s1); // 调用拷贝构造函数
    String s3(std::move(s1)); // 调用移动构造函数
    return 0;
}