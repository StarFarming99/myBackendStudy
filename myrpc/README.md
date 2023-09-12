# Myrpc
c++ rpc framework

**框架特性** 

1. 操作系统：Linux 

2. 编程语言：C++14 
3. 完全独立：不依赖任何第三⽅库 
4. 高性能：微秒级响应 
5. 高并发：单机百万连接 
6. IO多路复⽤：epoll 
7. 连接池 
8. 线程池


## example

server.cpp

```c++
#include <iostream>
#include <algorithm>
using namespace std;

#include <rpc/Server.h>
using namespace yazi::rpc;

string hello(const string & name)
{
    return "hello, " + name;
}

int sum(int a, int b)
{
    return a + b;
}

class Request : public Serializable
{
public:
    Request() {}
    Request(const string & name) : m_name(name) {}
    ~Request() {}

    const string & name() const
    {
        return m_name;
    }

    SERIALIZE(m_name)

private:
    string m_name;
};

class Response : public Serializable
{
public:
    Response() {}
    Response(const string & name) : m_name(name) {}
    ~Response() {}

    const string & name() const
    {
        return m_name;
    }

    SERIALIZE(m_name)

private:
    string m_name;
};

Response upper(const Request & req)
{
    string name = req.name();
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    return Response(name);
}

int main()
{
    Server * server = Singleton<Server>::instance();
    server->listen("127.0.0.1", 8080);
    server->bind("hello", hello);
    server->bind("sum", sum);
    server->bind("upper", upper);
    server->start();

    return 0;
}

```


client.cpp

```c++
#include <iostream>
using namespace std;

#include <rpc/Client.h>
using namespace yazi::rpc;

class Request : public Serializable
{
public:
    Request() {}
    Request(const string & name) : m_name(name) {}
    ~Request() {}

    const string & name() const
    {
        return m_name;
    }

    SERIALIZE(m_name)

private:
    string m_name;
};

class Response : public Serializable
{
public:
    Response() {}
    Response(const string & name) : m_name(name) {}
    ~Response() {}

    const string & name() const
    {
        return m_name;
    }

    SERIALIZE(m_name)

private:
    string m_name;
};

int main()
{
    Client client;
    client.connect("127.0.0.1", 8080);

    auto reply = client.call<string>("hello", "kitty");
    std::cout << reply << std::endl;

    auto total = client.call<int>("sum", 1, 2);
    std::cout << total << std::endl;

    Request req("kitty");
    auto resp = client.call<Response>("upper", req);
    std::cout << resp.name() << std::endl;
    
    return 0;
}
```

整体框架设计图：

![image-20230912152926930](C:\Users\Zihang\Desktop\yazi-rpc\myrpc\image-20230912152926930.png)



C++14可变参数的接受与解包：rpc核心逻辑

```c++
class FunctionHandler
{
public:
    FunctionHandler() {}
    ~FunctionHandler() {}

    template <typename F>
    void bind(const string & name, F func);

    void call(const string & name, DataStream & in, DataStream & out);

private:
    template <typename F>
    void wrap(F func, DataStream & in, DataStream & out);

    template <typename R, typename ...Args>
    void wrap_impl(R(*func)(Args...), DataStream & in, DataStream & out);

    template <typename R, typename ...Args>
    void wrap_impl(std::function<R(Args...)> func, DataStream & in, DataStream & out);

    template<typename R, typename F, typename Tuple>
    typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
    call_impl(F func, Tuple args);

    template<typename R, typename F, typename Tuple>
    typename std::enable_if<!std::is_same<R, void>::value, R>::type
    call_impl(F func, Tuple args);

    template<typename R, typename F, typename Tuple>
    auto invoke(F && func, Tuple && t);

    template<typename R, typename F, typename Tuple, std::size_t... Index>
    auto invoke_impl(F && func, Tuple && t, std::index_sequence<Index...>);

    template<typename Tuple, std::size_t... I>
    Tuple get_args(DataStream & ds, std::index_sequence<I...>);

    template<typename Tuple, std::size_t Id>
    void get_arg(DataStream & ds, Tuple & t);

private:
    std::map<string, std::function<void(DataStream &, DataStream &)>> m_handlers;
};

inline void FunctionHandler::call(const string & name, DataStream & in, DataStream & out)
{
    auto func = m_handlers[name];
    func(in, out);
}

template <typename F>
void FunctionHandler::bind(const string & name, F func)
{
    m_handlers[name] = std::bind(&FunctionHandler::wrap<F>, this, func, std::placeholders::_1, std::placeholders::_2);
}

template <typename F>
void FunctionHandler::wrap(F func, DataStream & in, DataStream & out)
{
    wrap_impl(func, in, out);
}

template <typename R, typename ...Args>
void FunctionHandler::wrap_impl(R(*func)(Args...), DataStream & in, DataStream & out)
{
    wrap_impl(std::function<R(Args...)>(func), in, out);
}

template <typename R, typename ...Args>
void FunctionHandler::wrap_impl(std::function<R(Args...)> func, DataStream & in, DataStream & out)
{
    using args_type = std::tuple<typename std::decay<Args>::type...>;
    constexpr auto size = std::tuple_size<typename std::decay<args_type>::type>::value;
    args_type args = get_args<args_type>(in, std::make_index_sequence<size>{});
    auto ret = call_impl<R>(func, args);
    out << ret;
}

template<typename R, typename F, typename Tuple>
typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
FunctionHandler::call_impl(F func, Tuple args)
{
    invoke<R>(func, args);
    return 0;
}

template<typename R, typename F, typename Tuple>
typename std::enable_if<!std::is_same<R, void>::value, R>::type
FunctionHandler::call_impl(F func, Tuple args)
{
    return invoke<R>(func, args);
}

template<typename R, typename F, typename Tuple>
auto FunctionHandler::invoke(F && func, Tuple && t)
{
    constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
    return invoke_impl<R>(std::forward<F>(func), std::forward<Tuple>(t), std::make_index_sequence<size>{});
}

template<typename R, typename F, typename Tuple, std::size_t... Index>
auto FunctionHandler::invoke_impl(F && func, Tuple && t, std::index_sequence<Index...>)
{
    return func(std::get<Index>(std::forward<Tuple>(t))...);
}

template<typename Tuple, std::size_t... I>
Tuple FunctionHandler::get_args(DataStream & ds, std::index_sequence<I...>)
{
    Tuple t;
    std::initializer_list<int>{((get_arg<Tuple, I>(ds, t)), 0)...};
    return t;
}

template<typename Tuple, std::size_t Id>
void FunctionHandler::get_arg(DataStream & ds, Tuple & t)
{
    ds >> std::get<Id>(t);
}

}
```

