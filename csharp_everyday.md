# C#每日一题

## override 一般需要和virtual搭配使用，类似c++虚函数override

## async用法简述
异步对可能会被屏蔽的活动（如 Web 访问，文件使用）至关重要

## internal 修饰符作用
访问级别：仅限于当前程序集(DLL or EXE)，不限制命名空间

## event 和 delegate
`delegate`是用来声明“函数指针”的关键词。是将**函数名**作为**函数的参数**来使用，将**一个函数的声明的函数名**作为一个相当于**数据类型**的存在

```csharp
public void PrintA() => Console.WriteLine("A");
public void PrintB() => Console.WriteLine("B");

public delegate void DelegateMethod();
// 注意类型和返回值必须一致
var trigger1 = new DelegateMethod(PrintA);
var trigger2 = new DelegateMethod(PrintB);

var trigger3 = trigger1 + trigger2;
trigger3();
```
```
A
B
```
`event`是专门用于修饰`delegate`变量的修饰词。即，可有如下两种方式声明一个`delegate`变量
```csharp
public event DelegateMethod trigger4;
public DelegateMethod trigger5;
```
event修饰过的delegate有如下限定：
> trigger4只能定义在类中，不能定义在类外部。trigger5则可以。   
假设trigger4定义在类A中，只有A内部的函数能激发trigger4，A外部的指令没有权限激发。而trigger5没有这个限制。   
假设trigger4定义在类A中，A外部的指令无法给trigger4赋值=，只能注册+=或注销-=函数指针。而trigger5没有这个限制。   
 

## 简述static的用法和作用
静态类
> 静态类不能实例化，不能使用 new。没有非静态的成员变量，不能被继承，没有普通构造函数
在声明一个类时使用static关键字，具有两个方面的意义：   
首先，它防止程序员写代码来实例化该静态类；   
其次，它防止在类的内部声明任何实例字段或方法。

静态成员
> 类似c++的静态成员。（略）   
类加载的时候，所有的静态成员就会被创建在“静态存储区”里面，一旦创建直到程序退出，才会被回收。   
不能用this/base关键字，但可以被静态方法调用
静态方法只能被重载，而不能被重写，因为静态方法不属于类的实例成员。   
虽然字段不能声明为 static const，但 const 字段的行为在本质上是静态的。这样的字段属于类，不属于类的实例。

静态方法
> 静态方法可以访问静态成员；不可以直接访问实例成员.可以在实例函数调用的情况下，实例成员做为参数传给静态方法；   
静态方法也不能直接调用实例方法，可以间接调用，首先要创建一个类的实例，然后通过这一特定对象来调用静态方法。

静态构造函数
> 静态类可以有静态构造函数，静态构造函数不可继承   
静态构造函数可以用于静态类，也可用于非静态类；   
静态构造函数无访问修饰符、无参数，只有一个 static 标志；
静态构造函数不可被直接调用，当创建类实例或引用任何静态成员之前，静态构造函数被自动执行，并且只执行一次。
```csharp
         public static int i =0;
         public Program()
         {
             i = 1;
             Console.Write("实例构造方法被调用");
         }
         static Program()
         {
             i = 2;
             Console.Write("静态构造函数被执行");
         }
         static void Main(string[] args)
         {
             Console.Write(Program.i);//结果为2，首先，类被加载，所有的静态成员被创建在静态存储区，i=0,接着调用了类的成员，这时候静态构造函数就会被调用，i=2
             Program p = new Program();
             Console.Write(Program.i);//结果为1，实力化后，调用了实例构造函数，i=1，因为静态构造函数只执行一次，所以不会再执行。
         }
```

## 静态成员的存储
静态全局变量
> 该变量在全局数据区分配内存。   
初始化：如果不显式初始化，那么将被隐式初始化为0。   

静态局部变量
> 该变量在全局数据区分配内存。
初始化：如果不显式初始化，那么将被隐式初始化为0。
其作用域为局部作用域,但它始终驻留在全局数据区，直到程序运行结束。


静态数据成员
> 内存分配：在程序的全局数据区分配。   
静态数据成员因为程序一开始运行就必需存在，所以其初始化的最佳位置在类的内部实现。 　　
被public,protected,private 关键字的影响
因为其空间在全局数据区分配，属于所有本类的对象共享，所以，它不属于特定的类对象，在没产生类对象时其作用域就可见，即在没有产生类的实例时，我们就可以操作它。
可以被继承，但只有一份

静态数据成员，主要用在类的所有实例都拥有的属性上。
比如，对于一个存款类，帐号相对于每个实例都是不同的，但每个实例的利息是相同的。所以，应该把利息设为存款类的静态数据成员。
这有两个好处，第一，不管定义多少个存款类对象，利息数据成员都共享分配在全局区的内存，所以节省存贮空间。
第二，一旦利息需要改变时，只要改变一次，则所有存款类对象的利息全改变过来了，因为它们实际上是共用一个东西。  

静态成员函数
> 静态成员函数与类相联系，不与类的对象相联系   
静态成员函数不能访问非静态数据成员
作用： 主要用于对静态数据成员的操作
调用形式： 类对象名.静态成员函数名（）

 

　　static静态变量的实例与分析，代码如下：
　static静态变量的实例与分析，代码如下：

```csharp
class Program
    {
        static int i = getNum();
        int j = getNum();

        static int num = 1;

        static int getNum()
        {
            return num;
        }

        static void Main(string[] args)
        {
            Console.WriteLine("i={0}", i);
            Console.WriteLine("j={0}", new Program().j);
            Console.Read();
        }

    }
```
最后的结果为：
```
 　　i＝0 j＝1
```
注意：   
当类第一次被加载时，会对类中的静态变量先按顺序进行分配内存空间，当全部分配完内存空间之后，在对静态变量按顺序赋值。   
首先分为两部分 寄存器和内存（包括缓存）   
内存分为两部分 代码和数据   
数据分为两部分 静态存储区和运行时存储   
运行时存储分为 堆栈 和 堆   
静态存储分为 全局静态存储 和 常量