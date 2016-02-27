#include <muduo/base/Logging.h>

#include "woody/http/http_server.h"
#include "woody/http/http_application.h"
#include "woody/http/http_handler.h"

using namespace std;
using namespace woody;

class App : public HTTPApplication {
 public:
  virtual void HandleRequest(const HTTPHandlerPtr& handler,
                             const HTTPRequest& req,
                             HTTPResponse& resp) {
    resp.SetStatus(200, "OK")
        .AddHeader("lala", "haha")
        .AddBody("I am body");
    resp.End();
  }
};
int main()
{
  muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
  App app;
  HTTPServer server(5011, "simple_server");
  server.Handle("/", &app);
  server.Start();
}

