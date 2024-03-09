#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <map>
#include <mutex>
#include <stack>
#include <string>
// #include <exception>
#include <memory>
#include <queue>
#include <condition_variable>
#include <atomic>

using namespace std;
struct empty_stack : exception
{
    const char* what() const noexcept {
        return "empty stack!";
    }
};

template<typename T = int>
class threadsafe_stack
{
    stack<T> data;
    mutable mutex m; // 思考，为什么是mutable
public:
    threadsafe_stack(){}
    // 思考，为什么需要移动构造函数
    threadsafe_stack(threadsafe_stack&& other) {
        lock_guard<mutex> lock(other.m);
        data = move(other.data);
    }

    threadsafe_stack& operator=(threadsafe_stack&& other) {
        if (this != other) {
            // 思考，这里为什么要这么做
            lock(m, other.m);
            // 同时给this和other上锁，避免死锁
            lock_guard<mutex> self_lock(m, std::adopt_lock);
            // 告诉lock_guard，互斥量已经被当前线程锁定，构造时不用再上锁，析构时解锁
            lock_guard<mutex> other_lock(other.m, std::adopt_lock);
            data = move(other.data);
        }
    }
    // 思考，为什么需要删除赋值构造函数
    // 线程对象本身不支持拷贝和赋值等操作。
    // 这是因为线程启动后，其内部资源被移交给了操作系统进行管理，如果复制线程对象，将会导致线程的内部资源重复被管理，进而导致程序的错误或崩溃。
    threadsafe_stack(const threadsafe_stack& other) = delete;
    // 思考，为什么需要删除赋值运算符函数
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T value) {
        lock_guard<mutx> lock(m);
        data.push(value);
    }
    shared_ptr<T> pop() {
        lock_guard<mutex> lock(m);
        if (data.empty()) throw empty_stack();
        shared_ptr<T> const res(make_shared<T>(data.top()));
        data.pop();
        return res;
    }
    void pop(T& value) {
        lock_guard<mutex> lock(m);
        if (data.empty()) throw empty_stack();
        value = move(stack.top());
        stack.pop();
    }
    bool empty() const {
        lock_guard<mutex> lock(m);
        return data.empty();
    }
};

