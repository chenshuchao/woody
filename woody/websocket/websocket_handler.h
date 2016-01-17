#ifndef WOODY_WEBSOCKET_WEBSOCKETHANDLER_H
#define WOODY_WEBSOCKET_WEBSOCKETHANDLER_H

#include <vector>

#include "woody/websocket/websocket_codec.h"
#include "woody/http/http_handler.h"

namespace woody {
static const int kHTTP = 0;
static const int kWebsocket = 1;
//static const int kVersions[] = {9, 10, 11, 12, 13}
static const int kVersion = 13; // only support 13
static const std::string kWebsocketGUID("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

class WebsocketHandler : public HTTPHandler {
 public:
  WebsocketHandler(const std::string& name,
                   const muduo::net::TcpConnectionPtr& conn);
  virtual ~WebsocketHandler() { }
 
  int GetProtocol() { return protocol_; }
  void SetProtocol(int x) { protocol_ = x; }

  void AddSubProtocol(std::string protocol)
  { sub_protocols_.push_back(protocol); }
  
  virtual void OnData(muduo::net::Buffer* buf);
  virtual void HandleRequest(const HTTPRequest& req);
  virtual void HandleWebsocket(const WebsocketMessage&) = 0;
  void HandleUpgradeRequest(const HTTPRequest& req);
  
  virtual void OnMessageComplete(const WebsocketMessage&);

  bool SendWebsocketMessage(const WebsocketMessage& message);

 private:
  // TODO enum
  int protocol_;
  std::vector<std::string> sub_protocols_;
  WebsocketCodec ws_codec_;
};
}

#endif
