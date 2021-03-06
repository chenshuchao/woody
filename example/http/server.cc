#define LOGGING_LEVEL WARNING
#include <bytree/logging.hpp>
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
  //muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
  App app;
  int port = 5011;
  string name("simple_server");
  HTTPServer server(port, name);
  server.Handle("/", &app);
  server.Start();
}

