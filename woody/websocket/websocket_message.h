#ifndef WOODY_WEBSOCKET_WEBSOCKETMESSAGE_H
#define WOODY_WEBSOCKET_WEBSOCKETMESSAGE_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "woody/websocket/websocket_frame.h"

namespace woody {
class WebsocketMessage {
 public:
  enum MessageType {
    kNoneMessage,
    kTextMessage,
    kBinaryMessage,
    kCloseMessage,
    kPingMessage,
    kPongMessage,
  };
  WebsocketMessage(MessageType type = kNoneMessage)
    : type_(type) {
  }
  //void SetComplete(bool b) { is_complete_ = b; }
  bool IsComplete() const { return is_complete_; }
  void Append(std::string& data);
  void CleanUp();
  MessageType GetType() const { return type_; }
  void SetType(MessageType type) { type_ = type; }

  bool SingleFrame(WebsocketFrame& frame) const;

 private:
  MessageType type_;
  std::string data_;
  bool is_complete_;
};
typedef boost::shared_ptr<WebsocketMessage> WebsocketMessagePtr;

/*
class TextMessage : public WebsocketMessage
{
 public:
  TextMessage() {}
};

class BinaryMessage : public WebsocketMessage
{
};

class CloseControlMessage : public WebsocketMessage
{
};
class PingControlMessage : public WebsocketMessage
{

};

class PongControlMessage : public WebsocketMessage
{

};
*/
}
#endif
