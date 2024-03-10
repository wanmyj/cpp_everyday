## 静态绑定和动态绑定
绑定：把一个方法与类对象关联起来的方法叫绑定。

|静态绑定|编译器|程序运行前就已经知道方法是属于哪个类的|
| --- | --- | --- |
|动态绑定|运行期|在程序运行过程中，根据具体的实例对象才能具体确定是调用了哪个方法|
## 引用是否能实现动态绑定，为什么引用可以实现
可以。引用必须要完成初始化，只要是虚函数就可以调用。本质上引用也是用常指针实现的。
```cpp
    Son s;
    Base& b = s; // 基类类型引用绑定已经存在的Son对象，引用必须初始化
    s.fun(); //son::fun()
    b.fun(); //son :: fun()
```
## 静态链接库、动态链接库的区别和特点

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

## std::ref用法
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

## 编译的四个过程
预处理：\
* 删除注释
* 处理所有#开头的指令，包括展开头文件，宏替换，条件编译
* 添加行号等各种标识
* 生成预处理i文件

编译：(C -> 汇编)\
源代码语义语法分析，目标代码分析和生成(实例化模板)，代码优化
生成汇编代码
汇总符号
生成.s文件

汇编(汇编 -> 二进制)：
根据不同平台，翻译成二进制机器码
合并section生成符号表
生成.o文件

链接：
合并obj文件的section，再次生成符号表
符号地址重定位
生成可执行文件

## 字符串宏和合并操作
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

