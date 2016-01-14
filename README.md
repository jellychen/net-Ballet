# Linux 网络框架

@(关键词)[性能|扩展|速度]

-------------------------

- **类似微软的Com设计**
- **基于Epoll ET模式，实现了ReadyList**
- **基于小根堆实现毫秒定时器，速度很快**
- **tcp server，  tcp client，   http server**
- **支持多进程模式，(server socket fd可以在多个进程wait)**
- **支持Task类似的多线程框架，eventfd多线程切换**
- **支持将c++数据序列化成std::string或者二进制buffer**
- **支持类似于弱引用的ComSink的函数代理回调，和C#的delegate类似**

---------------------------------

> 后面会支持
- **异步DNS查询**
- **完善的Udp支持**
- **ICMP和SMTP协议的支持**
- **实现Monitor支持Gateway和平滑重启**
- **支持消息队列和RPC**

### 联系方式
``` java
QQ: 994299094  Mail: 994299094@qq.com
```