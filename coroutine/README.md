# 协程框架
a tiny c++ coroutine framework



#### 什么是协程？

一个可以暂停并恢复的函数。



#### 实现：

Linux的ucontext接口



### ucontext 

getcontext：保存上下文，将当前运⾏到的寄存器的信息保存到 ucontext_t 结构体中。 

setcontext：恢复上下文，将 ucontext_t 结构体变量中的上下文信息重新恢复到 cpu 中并执行。 

makecontext：修改上下文，给 ucontext_t 上下⽂指定⼀个程序入口函数，让程序从该入口函数开始执行。 

swapcontext：切换上下文，保存当前上下文，并将下⼀个要执行的上下文恢复到 cpu 中。



#### 协程的状态：

1. RT_READY: 就绪态
2. RT_RUNNING: 运行态
3. RT_SUSPEND: 挂起态

#### 协程的方法：

1. run 纯虚函数，需要子类来实现协程的具体业务逻辑

2. resume：继续执行挂起的协程
3. yield：当前协程暂停，自己挂起，让调度器调度另一个协程继续运行。



### 协程类的设计

```c++
class Routine
{
public:
    enum Status
    {
        RT_READY = 0;
        RT_RUNNING,
        RT_SESPEND
    };
    
    Routine();
    virtual ~Routine();
    
    virtual void run() = 0;
    
    void resume();
    void yield();
    int status();
    
protected:
    static void func(void *ptr);
    
private:
    ucontext_t m_ctx;
    int m_status;
    char* m_stack;
    int m_stack_size; // 1024 * 1024
};
```





### 调度器设计

```c++
class Schedule
{
    friend class Routine;
public:
    Schedule();
    ~Schedule();
    
    void create(int stack_size = 1024 * 1024);
    void append(Routine *c);
    bool empty() const;
    int size() const;
    
    void resume();
private:
    ucontext_t m_main;
    char* m_stack;
    int m_stack_size;
    std::list<Routine*> m_queue;
};
```

#### 调度器的方法：

1. create: 初始化写成调度器
2. append：添加一个新协程到队列
3. resume: 唤起队列里的某个协程来执行







## 概念：

**有栈协程：**

无论存放什么，直接开辟一个空间存放，开辟的字节空间将会直接被当成栈内存来使用，这时候和普通函数的逻辑时没有区别的。

自定义栈，隐藏暂停，内存浪费/爆栈，深递归快

**无栈协程：**

有结构体来存放参数，局部变量等等东西。

不同的协程有不同的结构体。

定制结构体，显示暂停，内存紧凑，深递归慢
