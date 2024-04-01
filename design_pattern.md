## 设计模式七大原则
1) 单一职责原则：一个类应该只负责一项职责。
2) 接口隔离原则：客户端不应该依赖它不需要的接口
3) 依赖倒转(倒置)原则：程序要依赖于抽象接口，不要依赖于具体实现
4) 里氏替换原则：父类中凡是已经实现好的方法，实际上是在设定规范和契约
5) 开闭原则：模块和函数应该 对扩展开放( 对提供方)，对 修改关闭( 对使用方)。
6) 迪米特法则：一个对象应该对其他对象保持最少的了解
7) 合成复用原则：尽量使用合成/聚合的方式，而不是使用继承

工厂 代理 建造者 适配器 装饰器，策略、门面、观察者、单例

## 工厂
简单工厂会打破开闭原则，可以用工厂模式，但是会产生大量的类
你可以避免创建者和具体产品之间的紧密耦合。

    单一职责原则。
    开闭原则。 
    应用工厂方法模式需要引入许多新的子类， 代码可能会因此变得更复杂。 最好的情况是将该模式引入创建者类的现有层次结构中。

简单工厂会有复杂的 switch分支运算符， 用于选择各种需要实例化的产品类。 \
用工厂模式就可以了

    产品基类：
    各种产品类：
    工厂基类：依赖产品基类，接口返回一个产品基类的对象
    各种工厂类：继承工厂基类，接口返回各种产品的对象
    客户端：new一个具体的工厂类给工厂基类的指针，调用接口获得具体的产品。

## 策略和简单工厂组合
    产品基类：
    各种产品类：
    context类：产品基类的聚合（成员变量），通过switch各种产品类，接口直接返回这个产品类的输出
    客户端：new context类，传入产品参数，通过context获得产品的结果
简化了单元测试，但是还是有switch，需要用工厂模式的反射才能具体消除

## 代理
    共同接口类subject：定义了`realsub`和proxy的公共接口\
    realsub: 实现`subject`接口\
    proxy：`realsub`是它的成员变量，它对`subject`接口的实现直接是调用`realsub`的接口\
    客户端：使用proxy的接口即可

应用：网页打开本身很快，但打开图片很慢，就是一种代理。或者智能指针也是代理，在访问对象时还要附加内务处理比如改变count

## 建造者
    BuilderBase: 是一个纯虚接口类，需要BuilderDerived来实现\
    BuilderDerived: 实现各种形态，但不在构造函数里做东西\
    Director: BuilderBase作为成员变量，每个形态都要做的事情，包装到一个函数func里，由它来统一完成\
    客户端：new 各种形态的BuilderDerived，交给Director初始化传参，执行func函数来build完成

[1.什么是设计模式](https://github.com/colinlet/PHP-Interview-QA/blob/master/docs/09.%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F/QA.md)

2.如何理解框架
3.主要设计模式
4.怎样选择设计模式
5.单例模式
6.抽象工厂模式
7.工厂方法模式
8.适配器模式
9.观察者模式
10.策略模式
11.OOP 思想
12.抽象类和接口
13.控制反转
14.依赖注入
依赖注入是 将 所依赖的 传递给 将使用的从属对象（即客户端）。该服务是将会变成客户端的状态的一部分。 传递服务给客户端，而非允许客户端来建立或寻找服务