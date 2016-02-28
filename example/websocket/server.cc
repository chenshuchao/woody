#include <muduo/base/Logging.h>

#include <fstream>
#include <boost/pointer_cast.hpp>
#include "woody/websocket/websocket_server.h"

using namespace std;
using namespace woody;

class SimpleWebsocketServer : public WebsocketServer {
 public:
  SimpleWebsocketServer(int port,
                        const string& name)
      : WebsocketServer(port, name) {
  }
  void OnRequest(const WebsocketHandlerPtr& handler,
                     const HTTPRequest& req,
                     HTTPResponse& resp) {
    LOG_INFO << "WebsocketHandler HandleRequest";
    if(req.IsUpgrade()) {
      LOG_INFO << "WebsocketHandler upgrade";
      handler->HandleUpgradeRequest(req, resp);
      return;
    }
    // handle unupgrade request.
    LOG_INFO << "req methond: "<< req.GetMethod();
    if (req.GetUrl() == "/") {
      ifstream ifs("/home/shuchao/Documents/github/woody/example/websocket/index.html");
      string content((std::istreambuf_iterator<char>(ifs)),
                     (std::istreambuf_iterator<char>()));
      resp.SetStatus(200, "OK")
          .AddHeader("Content-Type", "text/html")
          .AddBody(content);
      resp.End();
      return;
    }
  }
  virtual void OnWebsocketTextMessage(const WebsocketHandlerPtr& handler,
                                      const TextMessage& message) {
      LOG_DEBUG << "websocket message: " << message.GetData();
      handler->SendTextMessage(message);
  }
};

int main() {
  muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
  SimpleWebsocketServer server(5011, "simple_server");
  server.Start();
}

