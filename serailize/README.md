# serialize

a tiny c++ serialize library

支持多种数据类型的序列化，不依赖包

#### 基本类型序列化、反序化

| 字段   | 字段长度 | 底层编码格式                        |
| ------ | -------- | ----------------------------------- |
| bool   | 2        | Type(1) + Value(1)                  |
| char   | 2        | Type(1) + Value(1)                  |
| int32  | 5        | Type(1) + Value(4)                  |
| int64  | 9        | Type(1) + Value(8)                  |
| float  | 5        | Type(1) + Value(4)                  |
| double | 9        | Type(1) + Value(8)                  |
| string | 可变长   | Type(1) + Length(5) + Value(length) |

#### 复合类型序列化 + 反序列化

| 字段      | 字段长度 | 底层编码格式                                             |
| --------- | -------- | -------------------------------------------------------- |
| vector<T> | 可变长   | Type(1) + Length(5) + Value(T + T + T...)                |
| list<T>   | 可变长   | Type(1) + Length(5) + Value(T + T + T...)                |
| map<K, V> | 可变长   | Type(1) + Length(5) + Value((K, V) + (K, V) + (K, V)...) |
| set<T>    | 可变长   | Type(1) + Length(5) + Value(T + T + T...)                |

#### 自定义对象类型编码

| 字段     | 字段长度 | 底层编码                                         |
| -------- | -------- | ------------------------------------------------ |
| 自定义类 | 可变长度 | Type(1) + Value(Member1 + Member2 + Member3 ...) |



解决了不同机器大小端转换的问题：只需要考虑多字节数字的转换

转换算法：

```c++
#include <algorithm>

int value = 0x12345678;
char * first = (char *)&value;
char * last = first + sizeof(int);
std::reverse(first, last);
```

