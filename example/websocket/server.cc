#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <fstream>

#include "woody/base_server.h"
#include "woody/websocket/websocket_handler.h"

using namespace std;
using namespace woody;

class SimpleWebsocketHandler : public WebsocketHandler {
 public:
  SimpleWebsocketHandler(const string& name,
                         const muduo::net::TcpConnectionPtr& conn)
    : WebsocketHandler(name, conn) {
  }
  ~SimpleWebsocketHandler() { }

  void HandleRequest(const HTTPRequest& req) {
    LOG_INFO << "WebsocketHandler HandleRequest";
    if(req.IsUpgrade()) {
      LOG_INFO << "WebsocketHandler upgrade";
      HandleUpgradeRequest(req);
      return;
    }
    // handle unupgrade request.
    LOG_INFO << "req methond: "<< req.GetMethod();
    HTTPResponse resp;
    if (req.GetUrl() == "/") {
      ifstream ifs("/home/shuchao/Documents/github/woody/example/websocket/index.html");
      string content((std::istreambuf_iterator<char>(ifs)),
                          (std::istreambuf_iterator<char>()));
      resp.SetStatus(200, "OK")
          .AddHeader("Content-Type", "text/html")
          .AddBody(content);
      SendResponse(resp);
      return;
    }
  }

  void HandleWebsocket(const WebsocketMessage& message) {
    if (message.GetType() == WebsocketMessage::kTextMessage) {
      SendWebsocketMessage(message);
    }
  }
};

class SimpleWebsocketHandlerFactory : public BaseHandlerFactory {
 public:
  SimpleWebsocketHandler* CreateHandler(const string& name,
                const muduo::net::TcpConnectionPtr& conn) {
    return new SimpleWebsocketHandler(name, conn);
  }
};

int main() {
  muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
  muduo::net::EventLoop loop;

  BaseServer server(loop,
                    5011,
                    new SimpleWebsocketHandlerFactory(),
                    "simple_server");
  server.Start();
  loop.loop();
}

