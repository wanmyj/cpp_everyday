# C++每日一题-2

- [C++每日一题-2](#c每日一题-2)
  - [1. set自定义比较函数的方法](#1-set自定义比较函数的方法)
  - [2. except使用](#2-except使用)
  - [3. 嵌套类的访问权限问题](#3-嵌套类的访问权限问题)
  - [4. void \*p 可以 p+1 吗](#4-void-p-可以-p1-吗)
  - [5. A a = func() 调用哪个构造函数](#5-a-a--func-调用哪个构造函数)
  - [6. 在C++中，必须使用初始化列表来初始化成员变量的情况](#6-在c中必须使用初始化列表来初始化成员变量的情况)
  - [7. c++11的智能指针是否是线程安全的](#7-c11的智能指针是否是线程安全的)
  - [8. unique\_ptr能否做到零开销](#8-unique_ptr能否做到零开销)
  - [9. make\_shared和普通shared\_ptr的区别](#9-make_shared和普通shared_ptr的区别)
  - [10. 空类的大小，空vector的大小](#10-空类的大小空vector的大小)
  - [11. 静态绑定和动态绑定](#11-静态绑定和动态绑定)
  - [12. 引用可以被取地址吗？空指针能否用来解引用，给函数引用传参？](#12-引用可以被取地址吗空指针能否用来解引用给函数引用传参)
  - [13. 引用是否能实现动态绑定，为什么引用可以实现](#13-引用是否能实现动态绑定为什么引用可以实现)
  - [14. 静态链接库、动态链接库的区别和特点](#14-静态链接库动态链接库的区别和特点)
  - [15. std::ref用法](#15-stdref用法)
  - [16. 编译的四个过程](#16-编译的四个过程)
  - [17. 字符串宏和合并操作](#17-字符串宏和合并操作)
  - [18. volatile 的作用](#18-volatile-的作用)

## 1. set自定义比较函数的方法
* lambda 函数法
```cpp
// 3 2 1 0
auto cmp = [](int a, int b) { return a>b; };
std::set<int, decltype(cmp)> s(cmp);
// c++20 std::set<int, decltype(cmp)> s;
```
* 普通函数法
```cpp
bool cmp(int a, int b) {return a>b;}

std::set<int, decltype(cmp)*> s1(cmp);
std::set<int, decltype(&cmp)> s2(&cmp);
```
* 结构体()重载函数法
```cpp
struct cmp {
    bool operator() (int a, int b) const { return a>b; }
};
std::set<int, cmp> s;
```

RTTI(Run Time Type Identification)即通过运行时类型识别，`decltype`属于编译时类型推导，类似于`auto`，自身特点是“以一个普通表达式作为参数，返回该表达式的类型”。

## 2. except使用
throw 使用 `std::runtime_error` \
catch 使用 `std::exception& e`\
print 使用 `e.what()` e.g. `std::cerr << e.what() << '\n';`
```cpp
try {
    if (true) throw std::runtime_error("error");
}
catch(std::exception& e) {
    std::cerr << e.what() << '\n';
}
```

## 3. 嵌套类的访问权限问题
嵌套类是外围类的隐式友元，外围类对嵌套类的访问没有特权。

## 4. void *p 可以 p+1 吗
不行,GNU会有warning，sizeof(void)返回1

## 5. A a = func() 调用哪个构造函数
普通拷贝构造函数。注：右值拷贝构造函数会自动屏蔽拷贝构造函数

## 6. 在C++中，必须使用初始化列表来初始化成员变量的情况
* const成员变量
* 引用成员变量
* 没有默认构造函数的成员变量
* 继承的没有默认构造函数的基类

## 7. c++11的智能指针是否是线程安全的
- `std::unique_ptr` 由于独占所有权语义，通常不需要在多个线程间共享它。

- `std::shared_ptr` 在其内部使用原子操作来管理对底层资源的引用计数，这意味着从多个线程同时复制和销毁 `std::shared_ptr` 对象是安全的。然而，被 `std::shared_ptr` 管理的对象本身并不保证是线程安全的。你需要额外的同步机制(如互斥锁)来保护共享的对象，以防止数据竞争。

- `std::weak_ptr` 是一种配合 `std::shared_ptr` 使用的智能指针，它不会增加所指向对象的引用计数。创建和销毁 `std::weak_ptr` 也是线程安全的，但当你尝试从 `std::weak_ptr` 获得一个 `std::shared_ptr` 时，你需要确保相关的 `std::shared_ptr` 对象在这个操作过程中是有效的。


## 8. unique_ptr能否做到零开销
std::unique_ptr 设计上旨在实现零额外开销，而且在大多数使用场景中确实能够达到这一点

## 9. make_shared和普通shared_ptr的区别
建一个shared_ptr需要两次不连续内存分配和创建
make_shared只需要一次连续的分配，不需要显式new，效率更好，异常安全。坏处就是释放pointer本身的空间的时候受weak_ptr的影响

## 10. 空类的大小，空vector的大小
空类：一个字节。但当其作为基类时候，就没有了。\
空vector：在64位系统上，一个空的 std::vector 可能会有3个指针大小（一个用于数据开始，一个用于结束，一个用于末尾的容量），sizeof(empty_vector) 将会是24字节。

## 11. 静态绑定和动态绑定
绑定：把一个方法与类对象关联起来的方法叫绑定。

|静态绑定|编译器|程序运行前就已经知道方法是属于哪个类的|
| --- | --- | --- |
|动态绑定|运行期|在程序运行过程中，根据具体的实例对象才能具体确定是调用了哪个方法|

## 12. 引用可以被取地址吗？空指针能否用来解引用，给函数引用传参？
引用可以被取地址\
空指针能解引用，编译是可以通过的，编译器可能会出现warning\
这是一个未定义行为，运行期一旦使用这个引用的对象，肯定是会出问题的。但使用取地址运算符，不会有段错误。

## 13. 引用是否能实现动态绑定，为什么引用可以实现
可以。引用必须要完成初始化，只要是虚函数就可以调用。本质上引用也是用常指针实现的。
```cpp
    Son s;
    Base& b = s; // 基类类型引用绑定已经存在的Son对象，引用必须初始化
    s.fun(); //son::fun()
    b.fun(); //son :: fun()
```

## 14. 静态链接库、动态链接库的区别和特点

| 特性 | 静态链接库 | 动态链接库 |
| --- | --- | --- |
| 后缀 | .a (Unix/Linux), .lib (Windows) | .so (Unix/Linux), .dll (Windows) |
| 链接时期 | 编译时期 | 程序运行时 |
| 代码整合 | 将代码复制成程序的一部分 | 只引用需要的代码 |
| 二进制文件体积 | 较大 | 较小 |
| 移植性 | 方便（无需额外的库） | 不方便（依赖外部库） |
| 更新库时的处理 | 需要重新编译程序 | 通常无需重新编译程序 |
| 运行时开销 | 无额外开销 | 需要在运行时链接代码，增加运行时间 |
| 内存空间 | 存在多份拷贝 | 只有一份拷贝 |

## 15. std::ref用法
C++11 中还要引入一个 `std::ref` 主要是考虑函数式编程（如 `std::bind` 多线程的 `std::thread`）在使用时，是对参数直接拷贝，而不是引用。

例1：
```cpp
void f(int& n1, int& n2, const int& n3)
{
    std::cout << "In function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    ++n1; // increments the copy of n1 stored in the function object
    ++n2; // increments the main()'s n2
    // ++n3; // compile error
}

int main()
{
    int n1 = 1, n2 = 2, n3 = 3;
    std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::cref(n3));
    n1 = 10;
    n2 = 11;
    n3 = 12;
    std::cout << "Before function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    bound_f();
    std::cout << "After function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
}
```
输出：
```
Before function: 10 11 12
In function: 1 11 12
After function: 10 12 12
```

例2：
```cpp
void threadFunc(std::string &str, int a)
{
    str = "change by threadFunc";
    a = 13;
}

int main()
{
    std::string str("main");
    int a = 9;
    std::thread th(threadFunc, std::ref(str), a);

    th.join();

    std::cout<<"str = " << str << std::endl;
    std::cout<<"a = " << a << std::endl;

    return 0;
}
```
输出：

```
str = change by threadFunc
a = 9
```
可以看到，和 std::bind 类似，多线程的 std::thread 也是必须显式通过 std::ref 来绑定引用进行传参，否则，形参的引用声明是无效的。

## 16. 编译的四个过程
预处理：
* 删除注释
* 处理所有#开头的指令，包括展开头文件，宏替换，条件编译
* 添加行号等各种标识
* 生成预处理i文件

编译：(C -> 汇编)
* 源代码语义语法分析，目标代码分析和生成(实例化模板)，代码优化
* 生成汇编代码
* 汇总符号
* 生成.s文件

汇编(汇编 -> 二进制)：
* 根据不同平台，翻译成二进制机器码
* 合并section生成符号表
* 生成.o文件

链接：
* 合并obj文件的section，再次生成符号表
* 符号地址重定位
* 生成可执行文件

## 17. 字符串宏和合并操作
```cpp
#define strer( x ) printf( #x "\n" )
strer(test)
```
1. 一般情况是直接对 `test` 加引号 `"test"`
2. 如果包含转义字符，会自动添加`\`标志来完成转义
3. 前后的空格，包括中间的空格，都会被忽略
4. 参数中的宏默认不会再被展开，如必须要展开需要中间的过度宏 `#define strerx(s) strer(s)`

字符串合并宏
```cpp
struct command commands[] {
    {"quit", quit_command },
    {"help", help_command },
    ...
}

#define COMMAND(NAME) { #NAME, NAME ## _command}
struct command commands[] {
    COMMAND(quit),
    COMMAND(help),
    ...
}
```



## 18. volatile 的作用
volatile 关键字用于告诉编译器，每次访问该变量时都必须从内存中读取，而不是使用寄存器中的缓存值。
它通常用于以下情况：
1. **硬件寄存器**：在嵌入式系统中，硬件寄存器的值可能会被外部设备改变
2. **多线程编程**：在多线程环境中，一个线程可能会修改一个变量