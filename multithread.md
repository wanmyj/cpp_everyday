## c++线程同步方式
* mutex
* conditional_variable
* future/promise


## linux线程是如何进行切换的
1. **保存当前线程的状态**：
2. **选择新线程**：
3. **加载新线程的状态**：
4. **执行新线程**：
## 如何证明一个数据结构是线程安全的
无状态一定安全\
单状态由一个线程安全的对象维护，也是安全的\
多状态需要讨论

## 并行编程中多进程和多线程，什么情况下多进程能解决的多线程无法解决
### 使用线程的情况：
1. **共享内存空间**：线程间通信成本低，上下文切换速度快。
2. **轻量级任务管理**：资源消耗少
3. **响应度提高**：
4. **利用多核处理器**：
### 使用进程的情况：
1. **隔离性和安全性**：
2. **资源分配**：
3. **稳定性考虑**：不会有死锁，一个崩溃不至于导致全盘崩溃
4. **充分利用分布式系统**：


## 惊群现象
当共享资源（如socket、文件等）可用时，多个等待该资源的线程或进程被同时唤醒竞争资源，但最终只有一个能够获得，造成其他被唤醒者无效唤醒，浪费了CPU资源。

## 程序什么时候应该使用线程，什么时候单线程效率高
使用线程：任务需要并行处理，或者io频繁，或者多核计算
选单线程：当任务顺序依赖、上下文切换开销大于并行收益时

## 锁的种类
1. **互斥锁**（Mutex）：通过`pthread_mutex_t`类型和相关函数实现，保证同一时间只有一个线程访问临界区。
2. **读写锁**（Read-Write Lock）：使用`pthread_rwlock_t`类型实现，允许多个读操作同时进行，但写操作是互斥的。
3. **自旋锁**（Spinlock）：用于轻量级同步，当线程尝试获取锁时，它在一个循环中不断检查锁是否可用，适用于短时间等待。
4. **条件变量**（Condition Variable）：通常与互斥锁配合使用，通过`pthread_cond_t`类型实现，允许线程在某种条件下挂起，直到另一个线程通知这个条件已经满足。

## condition variable使用方式
在 `cv.wait(lck);` 执行时，以下几件事情会发生：

1. 当前线程会释放通过 `unique_lock` 获得的互斥锁 `mtx` 并进入等待状态。
2. 当 `cv.notify_one()` 或 `cv.notify_all()` 被调用时，等待的线程之一（或全部，）会被唤醒。
3. 一旦当前线程被唤醒（不管是因为通知还是因为虚假唤醒），它将再次尝试获取互斥锁。如果获取成功，`wait` 函数返回，并持有锁。

```cpp
#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

std::mutex mtx; // 全局互斥锁.
std::condition_variable cv; // 全局条件变量.
bool ready = false; // 全局标志位.

void do_print_id(int id)
{
    std::unique_lock <std::mutex> lck(mtx);
    while (!ready) // 如果标志位不为 true, 则等待...
        cv.wait(lck); // 当前线程被阻塞, 当全局标志位变为 true 之后,
    // 线程被唤醒, 继续往下执行打印线程编号id.
    std::cout << "thread " << id << '\n';
}

void go()
{
    std::unique_lock <std::mutex> lck(mtx);
    ready = true; // 设置全局标志位为 true.
    cv.notify_all(); // 唤醒所有线程.
}

int main()
{
    std::thread threads[10];
    // spawn 10 threads:
    for (int i = 0; i < 10; ++i)
        threads[i] = std::thread(do_print_id, i);

    std::cout << "10 threads ready to race...\n";
    go(); // go!

  for (auto & th:threads)
        th.join();

    return 0;
}
```
执行结果如下：
```
10 threads ready to race...
thread 1
thread 0
thread 2
thread 3
thread 4
thread 5
thread 6
thread 7
thread 8
thread 9
复制代码
```
## shared_future
它的get()函数可以被多线程多次调用。

## promise, future, and async
获取子线程的返回值
```cpp
#include <future>
using namespace std;
int factorial(int N) {
    int res = 1;
    for (int i = N; i> 1; i--)
        res *= i;
    return res;
}
int main() {
    int x;
    // std::future<int> fu = std::async(factorial, 4);
    // std::future<int> fu = std::async(std::launch::deferred | std::launch::async, factorial, 4);
    // async 不一定就会新建新线程，新建线程必须加上std::launch::async 参数
    std::future<int> fu = std::async(std::launch::async, factorial, 4);
    // 如果加上std::launch::deferred 参数，则不会新建·线程，会在get的时候在当前线程运行factorial
    // std::future<int> fu = std::async(std::launch::deferred, factorial, 4);
    x = fu.get();
    // fu.get(); error, crush ,不能二次使用get()
}
```
给子线程递送一个值
```cpp
int factorial(std::future<int>& f) {
    int res = 1;
    int N = f.get();
    return N;
}
int main() {
    std::promise<int> p; // promise 不能被赋值和拷贝，只能用move
    std::future<int> f = p.get_future();
    std::future<int> fu = std::async(std::launch::async, factorial, std::ref(f));
    // do something else
    p.set_value(4);
    int x = fu.get();
    return 0;
}
```
## join和detach
使用join函数，原线程会等待新线程执行结束，再去销毁线程对象。\
如果使用detach函数，新线程和老线程分离后，如果老线程执行完毕，由于新线程有自己的栈，新线程的局部变量不会被立即销毁。新线程如果使用了和老线程创建和共享的变量，则可能产生异常。这些资源用**共享指针**是个不错的办法。

## 死锁例子
线程A,B都需要用两个互斥量M，N。\
A先占有M, 这时候B先占有了N。会发生AB两个线程都被卡住了。

## lock()函数，try_lock()
锁住互斥量。使用try_lock()时，注意如果当前线程二次调用try_lock(same_mtx)，会产生未定义行为。

## unique_lock vs lockguard
lock_guard 对象并不负责管理 Mutex 对象的生命周期，lock_guard 对象只是简化了 Mutex 对象的上锁和解锁操作\
unique_lock 可以随时加锁解锁，在condition_variable时候只能用他