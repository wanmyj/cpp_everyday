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

