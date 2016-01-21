#ifndef WOODY_WEBSOCKET_WEBSOCKETHANDLER_H
#define WOODY_WEBSOCKET_WEBSOCKETHANDLER_H

#include <vector>

#include <boost/enable_shared_from_this.hpp>

#include "woody/websocket/websocket_codec.h"
#include "woody/http/http_handler.h"

namespace woody {
static const int kHTTP = 0;
static const int kWebsocket = 1;
//static const int kVersions[] = {9, 10, 11, 12, 13}
static const int kVersion = 13; // only support 13
static const std::string kWebsocketGUID("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

class WebsocketHandler : public BaseHandler,
                         public boost::enable_shared_from_this<WebsocketHandler> {
 public:
  typedef boost::shared_ptr<WebsocketHandler> WebsocketHandlerPtr;
  typedef boost::function<void (const WebsocketHandlerPtr&, const HTTPRequest&)>
      RequestCompleteCallback;
  typedef boost::function<void (const WebsocketHandlerPtr&, const WebsocketMessage&)> MessageCompleteCallback;
  typedef boost::function<void (const WebsocketHandlerPtr&)> ErrorCallback;
  
  WebsocketHandler(const std::string& name,
                   const muduo::net::TcpConnectionPtr& conn);
  virtual ~WebsocketHandler() { }
 
  int GetProtocol() { return protocol_; }
  void SetProtocol(int x) { protocol_ = x; }

  void AddSubProtocol(std::string protocol)
  { sub_protocols_.push_back(protocol); }
  
  virtual void OnData(muduo::net::Buffer* buf);

  virtual void HandleUpgradeRequest(const HTTPRequest& req);

  void SendResponse(HTTPResponse& resp) {
    http_handler_.SendResponse(resp);
  }

  bool SendWebsocketMessage(const WebsocketMessage& message);
 
  virtual void HandleError();
 
  virtual void OnMessageComplete(const WebsocketMessage&);

  void OnRequestComplete(const HTTPRequest& req);

  void SetRequestCompleteCallback(const RequestCompleteCallback& cb) {
    request_complete_callback_ = cb;
  }
  void SetMessageCompleteCallback(const MessageCompleteCallback& cb) {
    message_complete_callback_ = cb;
  }
  void SetErrorCallback(const ErrorCallback& cb) {
    error_callback_ = cb;
  }



 private:
  // TODO enum
  int protocol_;
  std::vector<std::string> sub_protocols_;
  WebsocketCodec ws_codec_;
  HTTPHandler http_handler_;
  MessageCompleteCallback message_complete_callback_;
  RequestCompleteCallback request_complete_callback_;
  ErrorCallback error_callback_;
};
typedef boost::shared_ptr<WebsocketHandler> WebsocketHandlerPtr;
}

#endif
