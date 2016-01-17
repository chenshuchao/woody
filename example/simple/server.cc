#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include "woody/base_server.h"
#include "woody/http/http_handler.h"

using namespace std;
using namespace woody;

class SimpleHTTPHandler : public HTTPHandler
{
 public:
  SimpleHTTPHandler(const string& name,
                    const muduo::net::TcpConnectionPtr& conn)
    : HTTPHandler(name, conn)
  {
  }
  ~SimpleHTTPHandler() {}

  void Handle(HTTPRequest& req)
  {
    LOG_INFO << "req methond: "<< req.GetMethod();
    HTTPResponse resp;
    resp.SetStatus(200, "OK")
        .AddHeader("hello", "world")
        .AddHeader("Content-Type", "text/plain")
        .AddBody("I am body");
    SendResponse(resp);
  }
};

class SimpleHTTPHandlerFactory : public BaseHandlerFactory
{
 public:
  SimpleHTTPHandler* CreateHandler(const string& name,
                const muduo::net::TcpConnectionPtr& conn)
  {
    return new SimpleHTTPHandler(name, conn);
  }
};

int main()
{
  muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
  muduo::net::EventLoop loop;

  BaseServer server(loop,
                    muduo::net::InetAddress(5011),
                    "simple_server",
                    new SimpleHTTPHandlerFactory()) ;
  server.Start();
  loop.loop();
}

