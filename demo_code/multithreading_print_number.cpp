#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

std::mutex mtx;
std::condition_variable cv;
bool isOddprev = false;
int max_count = 100;
int num = 1;
void printOdd() {
    while (num < max_count) {
        std::unique_lock<std::mutex> lock(mtx);
        // if pred is false, the function call will block
        cv.wait(lock, []{return !isOddprev;});
        if (num % 2 != 0) {
            cout << "odd: " <<num << endl;
            num++;
            isOddprev = true;
            cv.notify_one();
        }
    }

}
void printEven() {
    while (num < max_count) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{return isOddprev;});
        if (num % 2 == 0) {
            cout << "even: "<< num << endl;
            num++;
            isOddprev = false;
            cv.notify_one();
        }
    }
}

void printnums(int maxnum) {
/*
编写一个多线程程序，打印 1 到指定的最大数字 max_count。 
其中一个线程负责打印奇数，另一个线程负责打印偶数，
需要保证两个线程交替打印，并且不能出现重复的数字。
*/
    max_count = maxnum;
    std::thread oddThread(printOdd);
    std::thread evenThread(printEven);

    oddThread.join();
    evenThread.join();
}