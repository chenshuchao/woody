# woody

woody 是一个提供基本的 http 和 websocket 服务的 C++ 库，底层使用 [muduo](https://github.com/chenshuo/muduo) 网络库。

## 使用
###http

1. 实现接口 HTTPApplication
2. 设置路由后启动即可

```
#include <woody/http/http_server.h>
#include <woody/http/http_application.h>
#include <woody/http/http_handler.h>

using namespace std;
using namespace woody;

class App : public HTTPApplication {
 public:
  virtual void HandleRequest(const HTTPHandlerPtr& handler,
                             const HTTPRequest& req,
                             HTTPResponse& resp) {
    resp.SetStatus(200, "OK")
        .AddHeader("header_name", "header_value")
        .AddBody("I am body");
    resp.End();
  }
};
int main()
{
  App app;
  int port = 5011;
  string name("simple_server");
  HTTPServer server(port, name);
  server.Handle("/", &app);
  server.Start();
}
```

###websocket
//TODO


## 依赖库
[boost](http://www.boost.org/) (version >= 1.59.0)

