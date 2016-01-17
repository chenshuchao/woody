#ifndef WOODY_HTTP_HTTPHANDLER_H
#define WOODY_HTTP_HTTPHANDLER_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <muduo/net/TcpConnection.h>

#include "woody/base_handler.h"
#include "woody/http/http_codec.h"

namespace woody {
class HTTPHandler : public BaseHandler,
                    public boost::enable_shared_from_this<HTTPHandler> {
 public:
  HTTPHandler(const std::string& name,
              const muduo::net::TcpConnectionPtr&);
  virtual ~HTTPHandler() { }

  virtual void OnData(muduo::net::Buffer* buf);
  virtual void OnMessageBegin(HTTPCodec::StreamID id);
  virtual void OnMessageComplete(HTTPCodec::StreamID id, HTTPRequest& request);
  
  virtual void HandleRequest(const HTTPRequest& info) = 0;
  void FormatResponse(HTTPResponse& resp); 
  void SendResponse(HTTPResponse& response);

 private:
  HTTPCodec codec_;
};
}

#endif
