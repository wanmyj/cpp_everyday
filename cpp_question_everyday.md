
# C++每日一题

- [C++每日一题](#c每日一题)
  - [1. C++中能否使用空对象指针调用成员函数](#1-c中能否使用空对象指针调用成员函数)
  - [2. delete delete[]](#2-delete-delete)
  - [3. `shared_ptr`, `unique_ptr`, `weak_ptr` 这三个智能指针的用法和区别](#3-shared_ptr-unique_ptr-weak_ptr-这三个智能指针的用法和区别)
  - [4. 类的内存分布](#4-类的内存分布)
  - [5. 虚基类](#5-虚基类)
  - [6. 重载重写重定向](#6-重载重写重定向)
  - [7. C++ 中 const. static. extern. ref. volatile. explicit 的意思](#7-c-中-const-static-extern-ref-volatile-explicit-的意思)
  - [8. 'const int p' int const p . int * const p的区别](#8-const-int-p-int-const-p--int--const-p的区别)
  - [9 C/C++ 混合编程](#9-cc-混合编程)
  - [10. c++ 的 struct 和 class 的区别是什么，如果不考虑这个区别，能否混用（从内存结构分析），如果可以混用，struct的意义是什么](#10-c-的-struct-和-class-的区别是什么如果不考虑这个区别能否混用从内存结构分析如果可以混用struct的意义是什么)
  - [11. C++11如何创建单例类](#11-c11如何创建单例类)
  - [12. C++11 STL 中 unordered_map 和 map 的区别 (底层实现, 优缺点，适用情况)](#12-c11-stl-中-unordered_map-和-map-的区别-底层实现-优缺点适用情况)
  - [13. C++的内存管理，话题太大，不具体出题了，](#13-c的内存管理话题太大不具体出题了)
  - [14. STL 源码中的 hash 表的实现](#14-stl-源码中的-hash-表的实现)
  - [15. 引用](#15-引用)
  - [16. C++ 中的重载和重写(覆盖)的区别](#16-c-中的重载和重写覆盖的区别)
  - [17. the difference between `constexpr` and `const`](#17-the-difference-between-constexpr-and-const)
  - [18.  虚函数（virtual）可以是内联函数（inline）吗？如果一般不可以，在什么时候可以？如果一般可以，在什么情况下不可以？](#18--虚函数virtual可以是内联函数inline吗如果一般不可以在什么时候可以如果一般可以在什么情况下不可以)
  - [19.  如何定义一个只能在堆上（栈上）生成对象的类？](#19--如何定义一个只能在堆上栈上生成对象的类)
  - [20. C++11 右值引用. 移动语义和完美转发](#20-c11-右值引用-移动语义和完美转发)
  - [21. C++11下条件变量之虚假唤醒](#21-c11下条件变量之虚假唤醒)
  - [22. 有了malloc/free为什么还要new/delete？](#22-有了mallocfree为什么还要newdelete)
  - [23. C++ multiple inheritance ambiguous member](#23-c-multiple-inheritance-ambiguous-member)
  - [24. 强转类型标识符 `dynamic_cast`, `static_cast`, `reinterpret_cast`, `const_cast`, `safe_cast` 使用区别](#24-强转类型标识符-dynamic_cast-static_cast-reinterpret_cast-const_cast-safe_cast-使用区别)
  - [25. What happens to global and static variables in a shared library when it is dynamically linked?](#25-global-and-static-variables-in-a-shared-library)
  - [26. RVO](#26-返回值优化)
  - [27. -强枚举类型](#27-强枚举类型)

## 1. C++中能否使用空对象指针调用成员函数
> Yes, 只要不涉及到成员变量。因为此时`this`指针指向`nullptr`  
> 详见 [this](https://blog.csdn.net/chenzrcd/article/details/60472616)

## 2. delete delete[]
The `delete` operator deallocates memory and **calls the destructor** for a single object created with `new`.  
The `delete []` operator deallocates memory and calls destructors for an array of objects created with `new []`.  
Using `delete` on a pointer returned by `new []` or `delete []` on a pointer returned by `new` results in undefined behavior.


## 3. `shared_ptr`, `unique_ptr`, `weak_ptr` 这三个智能指针的用法和区别
weak_ptr就是shared_ptr的不计数版本，对对象的生命周期没有控制权
```cpp
// C++ program to illustrate the use of unique_ptr 
#include <iostream>
#include <memory>
using namespace std;

class A {
public:
    void show()
    {
        cout << ">:show()" << endl;
    }
};

int main()
{
    unique_ptr<A> p1(new A);
    p1->show();

    // returns the memory address of p1
    cout << p1.get() << endl;

    // transfers ownership to p2
    unique_ptr<A> p2 = move(p1);
    p2->show();
    cout << p1.get() << endl;
    cout << p2.get() << endl;

    // transfers ownership to p3
    unique_ptr<A> p3 = move(p2);
    p3->show();
    cout << p1.get() << endl;
    cout << p2.get() << endl;
    cout << p3.get() << endl;

    return 0;
}
```
Output:
```
>:show()
0x1c4ac20
>:show()
0          // NULL
0x1c4ac20
>:show()
0          // NULL
0          // NULL
0x1c4ac20
```
The below code returns a resource and if we don’t explicitly capture the return value, the resource will be cleaned up.  
If we do, then we have exclusive ownership of that resource.
In this way, we can think of `unique_ptr` as safer and better replacement of `auto_ptr`.
```cpp
unique_ptr<A> fun()
{
    unique_ptr<A> ptr(new A);

    /* ...
       ... */

    return ptr;
}
```
Use `shared_ptr` if you want to share ownership of a resource. Many `shared_ptr` can point to a single resource.  
`shared_ptr` maintains **reference count** for this propose.
when all `shared_ptr`’s pointing to resource goes out of scope the resource is destroyed.
```cpp
// C++ program to demonstrate shared_ptr
#include <iostream>
#include <memory>
using namespace std;

class A {
public:
    void show()
    {
       cout << ">:show()" << endl;
    }
};

int main()
{
    shared_ptr<A> p1(new A);
    cout << p1.get() << endl;
    p1->show();
    shared_ptr<A> p2(p1);
    p2->show();
    cout << p1.get() << endl;
    cout << p2.get() << endl;
    // Returns the number of shared_ptr objects
    // referring to the same managed object.
    cout << p1.use_count() << endl;
    cout << p2.use_count() << endl;
    // Relinquishes ownership of p1 on the object
    // and pointer becomes NULL
    p1.reset();
    cout << p1.get() << endl;
    cout << p2.use_count() << endl;
    cout << p2.get() << endl;
    return 0;
}
```
Output:
```
0x1c41c20
>:show()
>:show()
0x1c41c20
0x1c41c20
2
2
0          // NULL
1
0x1c41c20
```
## 4. 类的内存分布
> 对于常规类：   
静态数据成员虽然属于类，但不占用具体类对象的内存。   
成员函数不占用具体类对象内存空间，成员函数存在代码区。   
数据成员的访问级别并不影响其在内存的排布和大小，均是按照声明的顺序在内存中有序排布，并适当对齐。   

>有虚函数的类：   
类内存的起始处存储的是虚指针vptr，与虚函数数量无关

>单一继承且无虚函数：   
每个派生类中起始位置都是Base class subobject。

>单一继承且有虚函数：   
基类虚指针，基类的object，派生类的object

>多重继承：   
派生类C中依其继承的基类的顺序，存放了各个基类subobject及各自的vptr，然后才是Class C自己的数据成员

> 菱形继承：   
D中依次存放基类B subobject和基类C subobject。其中B和C中均存放一份class A subobject。

> 虚拟继承：   
class B中有两个虚指针：第一个指向B自己的虚表，第二个指向虚基类A的虚表。   
而且，从布局上看，class B的部分要放在前面，虚基类A的部分放在后面。   
在class B中虚基类A的成分相对内存起始处的偏移offset等于class B的大小（8字节）  
- Class如果内含一个或多个virtual base subobjects，将被分割成两部分：一个不变区域和一个共享区域。不变区域中的数据，不管后继如何衍化，总有固定的offset（从object的开头算起），所以这一部分可以直接存取。而共享区域所表现的就是virtual base class subobject。这部分数据的位置会因为每次的派生操作而发生变化，所以它们只可以被间接存取。

> 看class D的内存布局：直接的基类B和C按照声明的继承顺序，在D的内存中顺序安放。紧接着是D的data member。然后是共享区域virtual base class A。

## 5. 虚基类 
> 虚基类是用关键字virtual声明继承的父类，即便该基类在多条链路上被一个子类继承，但是该子类中只包含一个该虚基类的备份，虚基类主要用来解决继承中的二义性问题，这就是是虚基类的作用所在。

> 正是由于虚基类的这个作用，所以在每个子类的构造函数中必须显示的调用该虚基类的构造函数，不管该虚基类是不是直接的父类。

> 其次，虚基类的构造函数的调用早于其他非虚基类的构造函数的调用。

```
CBase constructor! 
CDerive1 constructor! 
CDerive2 constructor! 
CDerive12 constructor! 
 CDerive12:a = 400
 from CDerive1 : aa = 400
 from CDerive1 : a = 400
 from CDerive2 : a = 400
CDerive12 deconstructor! 
CDerive2 deconstructor! 
CDerive1 deconstructor! 
CBase deconstructor! 
```
非虚基类
```
CBase constructor! 
CDerive1 constructor! 
CBase constructor! 
CDerive2 constructor! 
CDerive12 constructor! 

CBase constructor! 
CDerive1 constructor! 
CDerive12_2 constructor! 
 CDerive12_2:a = 200
 from CDerive1 : a = 100
 from CDerive2 : a = 200
CDerive12_2 deconstructor! 
CDerive1 deconstructor! 
CBase deconstructor! 

CDerive12 deconstructor! 
CDerive2 deconstructor! 
CBase deconstructor! 
CDerive1 deconstructor! 
CBase deconstructor! 
```
## 6. 重载重写重定向

| 概念   | 作用域关系       | 函数名   | 参数      | 返回值     | virtual关键字     | 访问修饰符 |
| ------ | ------------ | ---------- | --------- | --------- | ---------------- | ---------- |
| 重载   | 相同的作用域     | 相同     | 不同      | 都可以   | -                 | -          |
| 重写   | 不同（派生/基类） | 相同     | 相同     | 相同    | 必须有， 不能有static| 可以不同   |
| 重定义 | 不同（派生/基类） | 相同     | 都可以    | 都可以 |    见下         | -          |

>重定义：基类指针返回的函数是基类函数   
如果基类函数没有virtual关键字，则必然产生会重定义   
如果基类函数  有virtual关键字，但参数不同，则也产生会重定义
 
## 7. C++ 中 const. static. extern. ref. volatile. explicit 的意思
> [ss](https://www.geek-share.com/detail/2643706224.html)

## 8. 'const int p' int const p . int * const p的区别
> 就是常量指针和指针常量。从右向左读 `int * const p`, "p is a const pointer to int"   
注意：两个成员函数如果只是常量性不同，是可以被重载的

## 9 C/C++ 混合编程
 extern "C"

```cpp
#ifdef __cplusplus
extern "C" {
#endif
void display();
#ifdef __cplusplus
}
#endif
```

## 10. c++ 的 struct 和 class 的区别是什么，如果不考虑这个区别，能否混用（从内存结构分析），如果可以混用，struct的意义是什么

> [Structure of a C++ Object in Memory Vs a Struct](https://stackoverflow.com/questions/422830/structure-of-a-c-object-in-memory-vs-a-struct)  
> For POD ("Plain Old Data"), there is no diff


## 11. C++11如何创建单例类

在单线程环境下
**构造**，**析构**，**拷贝**，**赋值**四个构造函数全部private\
搞一个static的函数，有个static变量，并return这个变量的地址

```cpp
// 懒汉式实现 (指系统运行中，实例并不存在, 要考虑线程安全)
class CSingleton
{
private:
    CSingleton(){ cout << "单例对象创建！" << endl; };
    CSingleton(const CSingleton &);
    CSingleton& operator=(const CSingleton &);
    ~CSingleton(){ cout << "单例对象销毁！" << endl; };


public:
    static CSingleton * getInstance()
    {
        static CSingleton myInstance;
        return &myInstance;
    }

};  

// 饿汉式实现(指系统一运行，就初始化创建实例, 线程安全)
class CSingleton
{
private:
    CSingleton(){ cout << "单例对象创建！" << endl; };
    CSingleton(const CSingleton &);
    CSingleton& operator=(const CSingleton &);
    ~CSingleton(){ cout << "单例对象销毁！" << endl; };

    static CSingleton *myInstance；

public:
    static CSingleton * getInstance()  // 这里的实现是非线程安全的
    {    
        if（myInstance == nullptr ）{
            myInstance = new CSingleton();
        }
        return myInstance ;
    }
}; 

CSingleton *CSingleton::myInstance = nullptr; // 静态对象初始化
```
> 在多线程环境下，饿汉式实现显然是不安全的，有可能造成多个线程同时创建多个不同实例，  
并且除了最后一个实例被引用，其他实例都被丢弃而引起内存泄漏。

解决办法：1. 加锁判断 2. std::call_once

> 说明一下为什么将构造函数等定义为私有的（private:）类型:  
构造函数定义为 private 的，是为了防止其他地方使用 new 创建 Singleton 的对象  
析构函数定义为 private 的，是为了防止其他地方使用 delete 删除 Singleton 的对象  
拷贝构造函数定义为 private 的，是为了防止通过拷贝构造函数创建新的 Singleton 对象  
赋值运算操作符定义为 private 的，是为了防止通过赋值操作创建新的 Singleton 对象  

## 12. C++11 STL 中 unordered_map 和 map 的区别 (底层实现, 优缺点，适用情况)

底层实现不同：
> unordered_map 底层实现是一个哈希表，元素无序
map 底层实现是红黑树，其内部所有的元素都是有序的，因此对 map 的所有操作，其实都是对红黑树的操作

优缺点：
> unordered_map：查找效率高；但是建立哈希表比较耗费时间
map：内部元素有序，查找和删除操作都是 logn 的时间复杂度；但是维护红黑树的存储结构需要占用一定的内存空间

适用情况：
> 对于要求内部元素有序的使用 map，对于要求查找效率的用 unordered_map

## 13. C++的内存管理，话题太大，不具体出题了，
> [答案](https://blog.csdn.net/caogenwangbaoqiang/article/details/79788368)   
代码段（text）：就是C程序编译后的机器指令，也就是我们常见的汇编代码。   
数据段（data）：\
    &ensp; &ensp; 数据区：⽤来存放显式初始化的全局变量或者静态（全局）变量，常量数据.\
    &ensp; &ensp; BSS段（Block Started by Symbol): 存储未初始化的全局变量或者静态（全局）变量。编译器给处理成0；   
堆段（heap): 动态内存分配的区域，也就是malloc申请的内存区，使⽤free()函数来释放内存\
    &ensp; &ensp; 堆区：new\
    &ensp; &ensp; 自由存储区：malloc\
栈段(stack)：存放函数调⽤相关的参数、局部变量的值，以及在任务切换的上下⽂信息。栈区是由操作系统分配和管理的区域。   

## 14. STL 源码中的 hash 表的实现
> hashtable 是采用开链法来完成的，（vector + list）

> 底层键值序列采用 vector 实现，vector 的大小取的是质数，且相邻质数的大小约为 2 倍关系，当创建 hashtable 时，会自动选取一个接近所创建大小的质数作为当前 hashtable 的大小；  

> 对应键的值序列采用单向 list 实现；  
当 hashtable 的键 vector 的大小重新分配的时候，原键的值 list 也会重新分配，因为 vector 重建了相当于键增加了，那么原来的值对应的键可能就不同于原来分配的键，这样就需要重新确定值的键。

## 15. 引用
> 什么是“引用”？  
申明和使用“引用”要注意哪些问题？  
将“引用”作为函数参数有哪些特点？  
在什么时候需要使用“常引用”？  
将“引用”作为函数返回值类型的格式. 好处和需要遵守的规则?

## 16. C++ 中的重载和重写(覆盖)的区别（重定义）
重写，是指派生类中存在重新定义的函数。  
其函数名，参数列表，**返回值类型**，所有都必须同基类中被重写的函数**一致**。只有函数体不同（花括号内），派生类调用时会调用派生类的重写函数，不会调用被重写函数。重写的基类中被重写的函数必须有virtual修饰。

函数重载是指在同一作用域内，可以有一组具有相同函数名，**不同参数列表**的函数，这组函数被称为重载函数。函数重载不考虑返回值类型，这是为了保持解析操作符或函数调用时，独立于上下文

还有重定义函数，派生类方法与基类同名但参数列表不同时，或者虽然相同但基类没写virtual。（基类写virtual，派生类方法仅有返回值不同，这时候编译报错）   
基类指针调用时，都会使用基类的方法，而派生类指针会调用派生类方法

## 17. the difference between `constexpr` and `const`
Both `const` and `constexpr` can be applied to variables and functions. Even though they are similar to each other, in fact they are very different concepts.

Both `const` and `constexpr` mean that their values can't be changed after their initialization. So for example:

```cpp
const int x1=10;
constexpr int x2=10;

x1=20; // ERROR. Variable 'x1' can't be changed.
x2=20; // ERROR. Variable 'x2' can't be changed.
The principal difference between const and constexpr is the time when their initialization values are known (evaluated).
While the values of const variables can be evaluated at both compile time and runtime, constexpr are always evaluated at compile time. For example:

int temp=rand(); // temp is generated by the the random generator at runtime.
const int x1=10; // OK - known at compile time.
const int x2=temp; // OK - known only at runtime.
constexpr int x3=10; // OK - known at compile time.
constexpr int x4=temp; // ERROR. Compiler can't figure out the value of 'temp' variable at compile time so `constexpr` can't be applied here.
```
The key advantage to know if the value is known at compile time or runtime is the fact that compile time constants can be used whenever compile time constants are needed. For instance, C++ doesn't allow you to specify C-arrays with the variable lengths.

```cpp
int temp=rand(); // temp is generated by the the random generator at runtime.

int array1[10]; // OK.
int array2[temp]; // ERROR.
So it means that:

const int size1=10; // OK - value known at compile time.
const int size2=temp; // OK - value known only at runtime.
constexpr int size3=10; // OK - value known at compile time.

int array3[size1]; // OK - size is known at compile time.
int array4[size2]; // ERROR - size is known only at runtime time.
int array5[size3]; // OK - size is known at compile time.
```
So `const` variables can define both compile time constants like size1 that can be used to specify array sizes and runtime constants like size2 that are known only at runtime and can't be used to define array sizes. On the other hand `constexpr` always define compile time constants that can specify array sizes.

Both `const` and `constexpr` can be applied to functions too. A `const` function must be a member function (method, operator) where application of `const` keyword means that the method can't change the values of their member (non-static) fields. For example.
```cpp
class test
{
   int x;

   void function1()
   {
      x=100; // OK.
   }

   void function2() const
   {
      x=100; // ERROR. The const methods can't change the values of object fields.
   }
};
```
A `constexpr` is a different concept. It marks a function (member or non-member) as the function that can be evaluated at compile time if compile time constants are passed as their arguments. For example you can write this.

```cpp
constexpr int func_constexpr(int X, int Y)
{
    return(X*Y);
}

int func(int X, int Y)
{
    return(X*Y);
}
int array1[func_constexpr(10,20)]; // OK - func_constexpr() can be evaluated at compile time.
int array2[func(10,20)]; // ERROR - func() is not a constexpr function.

int array3[func_constexpr(10,rand())]; // ERROR - even though func_constexpr() is the 'constexpr' function, the expression 'constexpr(10,rand())' can't be evaluated at compile time.
```

By the way the constexpr functions are the regular C++ functions that can be called even if non-constant arguments are passed. But in that case you are getting the non-constexpr values.

```cpp
int value1=func_constexpr(10,rand()); // OK. value1 is non-constexpr value that is evaluated in runtime.
constexpr int value2=func_constexpr(10,rand()); // ERROR. value2 is constexpr and the expression func_constexpr(10,rand()) can't be evaluated at compile time.
```
The `constexpr` can be also applied to the member functions (methods), operators and even constructors. For instance.

```cpp
class test2
{
    static constexpr int function(int value)
    {
        return(value+1);
    }

    void f()
    {
        int x[function(10)];


    }
};
```
A more 'crazy' sample.

```cpp
class test3
{
    public:

    int value;

    // constexpr const method - can't chanage the values of object fields and can be evaluated at compile time.
    constexpr int getvalue() const
    {
        return(value);
    }

    constexpr test3(int Value)
        : value(Value)
    {
    }
};

constexpr test3 x(100); // OK. Constructor is constexpr.

int array[x.getvalue()]; // OK. x.getvalue() is constexpr and can be evaluated at compile time.
```
## 18.  虚函数（virtual）可以是内联函数（inline）吗？如果一般不可以，在什么时候可以？如果一般可以，在什么情况下不可以？
> 虚函数可以是内联函数，内联是可以修饰虚函数的，但是当虚函数表现多态性的时候不能内联。  
内联是在编译器建议编译器内联，而虚函数的多态性在运行期，编译器无法知道运行期调用哪个代码，因此虚函数表现为多态性时（运行期）不可以内联。  

> `inline virtual` 唯一可以内联的时候是：编译器知道所调用的对象是哪个类（如 Base::who()），这只有在编译器具有实际对象而不是对象的指针或引用时才会发生。

## 19.  如何定义一个只能在堆上（栈上）生成对象的类？
只能在堆上 方法：
> 将析构函数设置为私有

原因：
>C++ 是静态绑定语言，编译器管理栈上对象的生命周期，编译器在为类对象分配栈空间时，会先检查类的析构函数的访问性。若析构函数不可访问，则不能在栈上创建对象。

只能在栈上 方法：
> 将 new 和 delete 重载为私有

原因：
> 在堆上生成对象，使用 new 关键词操作，其过程分为两阶段：第一阶段，使用 new 在堆上寻找可用内存，分配给对象；第二阶段，调用构造函数生成对象。将 new 操作设置为私有，那么第一阶段就无法完成，就不能够在堆上生成对象。

## 20. C++11 右值引用. 移动语义和完美转发
C++中所有的值都必然属于左值. 右值二者之一。左值是指表达式结束后依然存在的持久化对象，右值是指表达式结束时就不再存在的临时对象。所有的具名变量或者对象都是左值，而右值不具名。很难得到左值和右值的真正定义，但是有一个可以区分左值和右值的便捷方法：看能不能对表达式取地址，如果能，则为左值，否则为右值。

```cpp
int i=0;// i是左值， 0是右值

class A {
  public:
    int a;
};
A getTemp()
{
    return A();
}
A a = getTemp();   // a是左值  getTemp()的返回值是右值（临时变量）
```
c++11中的右值引用使用的符号是`&&`，如

```cpp
int a = 10;
int& refA = a; // refA是a的别名， 修改refA就是修改a, a是左值，左移是左值引用
int& b = 1; //编译错误! 1是右值，不能够使用左值引用
int&& a = 1; //实质上就是将不具名(匿名)变量取了个别名
int b = 1;
int && c = b; //编译错误！ 不能将一个左值复制给一个右值引用
class A {
  public:
    int a;
};
A getTemp()
{
    return A();
}
A && a = getTemp();   //getTemp()的返回值是右值（临时变量）
```
`getTemp()` 返回的右值本来在表达式语句结束后，其生命也就该终结了（因为是临时变量）
而通过右值引用，该右值又重获新生，其生命期将与右值引用类型变量a的生命期一样
只要a还活着，该右值临时变量将会一直存活下去。实际上就是给那个临时变量取了个名字。

注意：这里a的类型是右值引用类型(`int &&`)，但是如果从左值和右值的角度区分它，它实际上是个左值。因为可以对它取地址，而且它还有名字，是一个已经命名的右值。

所以，左值引用只能绑定左值，右值引用只能绑定右值，如果绑定的不对，编译就会失败。但是，常量左值引用却是个奇葩，它可以算是一个“万能”的引用类型，它可以绑定非常量左值. 常量左值. 右值，而且在绑定右值的时候，常量左值引用还可以像右值引用一样将右值的生命期延长，缺点是，只能读不能改。
```cpp
const int & a = 1; //常量左值引用绑定 右值， 不会报错

class A {
  public:
    int a;
};
A getTemp()
{
    return A();
}
const A & a = getTemp();   //不会报错 而 A& a 会报错
```
要实现移动语义就必须增加两个函数：移动构造函数和移动赋值构造函数。
```cpp
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

class MyString
{
public:
    static size_t CCtor; //统计调用拷贝构造函数的次数
    static size_t MCtor; //统计调用移动构造函数的次数
    static size_t CAsgn; //统计调用拷贝赋值函数的次数
    static size_t MAsgn; //统计调用移动赋值函数的次数

public:
    // 构造函数
   MyString(const char* cstr=0){
       if (cstr) {
          m_data = new char[strlen(cstr)+1];
          strcpy(m_data, cstr);
       } else {
          m_data = new char[1];
          *m_data = '\0';
       }
   }

   // 拷贝构造函数
   MyString(const MyString& str) {
       CCtor ++;
       m_data = new char[ strlen(str.m_data) + 1 ];
       strcpy(m_data, str.m_data);
   }
   // 移动构造函数
   MyString(MyString&& str) noexcept
       :m_data(str.m_data) {
       MCtor ++;
       str.m_data = nullptr; //不再指向之前的资源了
   }

   // 拷贝赋值函数 =号重载
   MyString& operator=(const MyString& str){
       CAsgn ++;
       if (this == &str) // 避免自我赋值!!
          return *this;

       delete[] m_data;
       m_data = new char[ strlen(str.m_data) + 1 ];
       strcpy(m_data, str.m_data);
       return *this;
   }

   // 移动赋值函数 =号重载
   MyString& operator=(MyString&& str) noexcept{
       MAsgn ++;
       if (this == &str) // 避免自我赋值!!
          return *this;

       delete[] m_data;
       m_data = str.m_data;
       str.m_data = nullptr; //不再指向之前的资源了
       return *this;
   }

   ~MyString() {
       delete[] m_data;
   }

   char* get_c_str() const { return m_data; }
private:
   char* m_data;
};
size_t MyString::CCtor = 0;
size_t MyString::MCtor = 0;
size_t MyString::CAsgn = 0;
size_t MyString::MAsgn = 0;
int main()
{
    vector<MyString> vecStr;
    vecStr.reserve(1000); //先分配好1000个空间
    for(int i=0;i<1000;i++){
        vecStr.push_back(MyString("hello"));
    }
    cout << "CCtor = " << MyString::CCtor << endl;
    cout << "MCtor = " << MyString::MCtor << endl;
    cout << "CAsgn = " << MyString::CAsgn << endl;
    cout << "MAsgn = " << MyString::MAsgn << endl;
}
/* 结果
CCtor = 0
MCtor = 1000
CAsgn = 0
MAsgn = 0
*/
```
对于一个左值，肯定是调用拷贝构造函数了，但是有些左值是局部变量，生命周期也很短，能不能也移动而不是拷贝呢？C++11为了解决这个问题，提供了`std::move()`方法来将左值转换为右值，从而方便应用移动语义。
```cpp
MyString str1("hello"); //调用构造函数
MyString str2("world"); //调用构造函数
MyString str3(str1); //调用拷贝构造函数
MyString str4(std::move(str1)); // 调用移动构造函数. 
//    cout << str1.get_c_str() << endl; // 此时str1的内部指针已经失效了！不要使用
//注意：虽然str1中的m_dat已经称为了空，但是str1这个对象还活着，知道出了它的作用域才会析构！而不是move完了立刻析构
MyString str5;
str5 = str2; //调用拷贝赋值函数
MyString str6;
str6 = std::move(str2); // str2的内容也失效了，不要再使用
```
移动语义，`emplace_back`减少内存拷贝和移动
我们之前使用`vector`一般都喜欢用`push_back()`，由上文可知容易发生无谓的拷贝，解决办法是为自己的类增加移动拷贝和赋值函数，但其实还有更简单的办法！就是使用`emplace_back()`替换`push_back()`
```cpp
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

class A {
public:
    A(int i){
//        cout << "A()" << endl;
        str = to_string(i);
    }
    ~A(){}
    A(const A& other): str(other.str){
        cout << "A&" << endl;
    }

public:
    string str;
};

int main()
{
    vector<A> vec;
    vec.reserve(10);
    for(int i=0;i<10;i++){
        vec.push_back(A(i)); //调用了10次拷贝构造函数
//        vec.emplace_back(i);  //一次拷贝构造函数都没有调用过
    }
    for(int i=0;i<10;i++)
        cout << vec[i].str << endl;
}
```
对于`map`和`set`，可以使用emplace()。基本上emplace_back()对应push_back(), emplace()对应insert()。

移动语义对swap()函数的影响也很大，之前实现swap可能需要三次内存拷贝，而有了移动语义后，就可以实现高性能的交换函数了。

## 21. C++11下条件变量之虚假唤醒
https://blog.csdn.net/pi9nc/article/details/37043123

## 22. 有了malloc/free为什么还要new/delete？
> malloc与free是C++/C语言的标准库函数，new/delete是C++的运算符。它们都可用于申请动态内存和释放内存。

> 对于非内部数据类型的对象而言，光用maloc/free无法满足动态对象的要求。对象在创建的同时要自动执行构造函数，对象在消亡之前要自动执行析构函数。由于malloc/free是库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加于malloc/free。

> 因此C++语言需要一个能完成动态内存分配和初始化工作的运算符new，以及一个能完成清理与释放内存工作的运算符delete。

## 23. C++ multiple inheritance ambiguous member
```cpp
a.base1::start();

a.base2::start();
```
or if you want to use one specifically

```cpp
class derived:public base1,public base2
{
public:
    using base1::start;
};
```

## 24. 强转类型标识符 `dynamic_cast`, `static_cast`, `reinterpret_cast`, `const_cast`, `safe_cast` 使用区别
> [answer](https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used)
dynamic_cast 只能够用在指向类的指针或者引用上(或者void*)。会做类型检查，检查不通过会返回空指针，或者对引用来说会抛异常。\
static_cast能够完成指向**相关类**的指针上的转换。upcast和downcast都能够支持，但不同的是，并不会做类型检查\
reinterpret_cast能够完成任意指针类型向任意指针类型的转换，reinterpret_cast能做但static_cast不能做的转换大多都是一些基于重新解释二进制的底层操作，因此会导致代码限定于特定的平台进而导致差移植性\
const_cast可以用来设置或者移除指针所指向对象的const。例如，要把一个const指针传入一个接受非const指针的函数里。在移除const之后假如真的向目标进行写操作将导致UB。


## 25. global and static variables in a shared library
> [answer](https://stackoverflow.com/questions/19373061/what-happens-to-global-and-static-variables-in-a-shared-library-when-it-is-dynam)

## 26. 返回值优化
返回值优化分为RVO和NRVO。   
```
T f()
{
    return T(constructor arguments);
}
T t = f();
```

Theoretically, there could be 3 objects of type T created here:

* the object constructed inside f in the return statement (which happens to be a temporary because it does not have a name),
* the temporary object returned by f, copied from the one above,
* the named object t, copied from the one above.

The RVO lets the compiler remove the two temporaries by directly initializing t with the constructor arguments passed inside the body of f.

But for the RVO to be applied, the returned object has to be constructed on a **return statement**. Therefore this object does not have a name.

The NRVO (Named-RVO) goes one step further: it can remove the intermediary objects even if the returned object has a name and is therefore not constructed on the return statement. So this object can be constructed before the return statement, like in the following example:
```
T f()
{
    T result(....);
    return result;
}
```
But, like with the RVO, the function still needs to return a unique object (which is the case on the above example), so that the compiler can determine which object inside of f it has to construct at the memory location of t (outside of f).

## 27. 强枚举类型
```
enum class - enumerator names are local to the enum and their values do not implicitly convert to other types (like another enum or int)

Plain enum - where enumerator names are in the same scope as the enum and their values implicitly convert to integers and other types

enum class Color1 { red, green, blue };    //this will compile
enum class Color2 { red, green, blue };

enum Color1 { red, green, blue };    //this will not compile 
enum Color2 { red, green, blue };

Color1 a = Color2::red; // error Color2::red is not a color1
```

## 28. 运算符重载
形式可以表现为类的成员函数，也可以表现为类的友元函数。   
前置++重载时没有参数，而后置++重载时有参数。不会使用其参数，仅仅是区分用。
和`. , :` 相关的运算符不可以重载，还有逻辑与或运算符，四个case不能重载。

## 28. RAII实现两种智能指针的过程
> 构造函数获得资源，析构函数释放资源。\
实现指针的直接引用，-> 运算, bool运算 \
类内包含指针的话，就少不了三个函数，拷贝构造，拷贝赋值，析构函数。\
unique智能指针还需要move，移动语义构造函数\
根据C++的规则，我们提供了转移构造而没有提供拷贝构造，那拷贝构造就自动被禁用。\
\
在拷贝里面实现搬移的操作，把原指针的对象赋值给新指针，原指针就不能再用了，对应 std::unique_ptr\
原指针和新指针都指向那一个对象，在智能指针里添加一个引用计数，引用计数为0后删除该对象，对应 std::shared_ptr \
实现一个share_count 类，用来处理引用计数增加减少的操作。在share_ptr里，放入share_count的指针

## volatile
volatile int i = 10; 
volatile  告诉编译器不要对象优化。
volatile 关键字声明的变量，每次访问时都必须从内存中取出值（没有被 volatile 修饰的变量，可能由于编译器的优化，从 CPU 寄存器中取值）
const 和 指针 都可以是 volatile

## move的意义是什么
把左值当右值使用，避免拷贝\
对于一个左值，肯定是调用拷贝构造函数了，但是有些左值是局部变量，生命周期也很短，能不能也移动而不是拷贝呢？C++11为了解决这个问题，提供了`std::move()`方法来将左值转换为右值，从而方便应用移动语义。\
局部变量的左值，当右值使用，走到右值引用的(构造)函数里\
```cpp
MyString str1("hello"); //调用构造函数
MyString str4(std::move(str1)); // 调用移动构造函数. 
```

## using用法总结