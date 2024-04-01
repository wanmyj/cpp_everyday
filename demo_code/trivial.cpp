#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <condition_variable>    // std::condition_variable
std::shared_ptr<int> global_instance = std::make_shared<int>(0);
constexpr int max_loop = 10000;

std::mutex mtx;
bool ready = false; // 全局标志位.
std::condition_variable cv;
void thread_fcn()
{
    std::unique_lock <std::mutex> lck(mtx);
    while (!ready) // 如果标志位不为 true, 则等待...
        cv.wait(lck); // 当前线程被阻塞, 当全局标志位变为 true 之后,

    // thread-safe reference counting 
    for (int i = 0; i < max_loop; i++) {
        std::shared_ptr<int> temp = global_instance;
        *temp = *temp + 1;
    }
  
    std::cout << "global_instance use count : " << global_instance.use_count() << std::endl;
}

void go()
{
    std::unique_lock <std::mutex> lck(mtx);
    ready = true; // 设置全局标志位为 true.
    cv.notify_all(); // 唤醒所有线程.
}

int main()
{
    *global_instance = 0;
    std::vector<std::thread> threadList;
    for (int i = 0; i < 10; ++i) {
        threadList.push_back(std::thread(thread_fcn));
    }
    go(); // go!

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    for (auto & thread : threadList)  {
        thread.join();
    }
    std::cout << __FUNCTION__ << "-> global_instance : " << *global_instance << std::endl;
    return 0;
}