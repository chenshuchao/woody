#ifndef WOODY_WEBSOCKET_WEBSOCKETCODEC_H
#define WOODY_WEBSOCKET_WEBSOCKETCODEC_H

#include <string>

#include "woody/websocket/websocket_parser.h"
#include "woody/websocket/websocket_message.h"

namespace woody {
class WebsocketCodec {
 public:
  typedef boost::function<void (const WebsocketMessage&)> MessageCallback;
  typedef boost::function<void ()> ErrorCallback;

  WebsocketCodec();

  bool OnData(const std::string& data, size_t& parsed_bytes);
  bool ConvertFrameToString(const WebsocketFrame& frame, std::string& parsed_string);

/*
  void OnTextMessage(const WebsocketMessage& message)
  { text_message_callback_(); }
  void OnBinaryMessage(const BinaryMessage& message);
  void OnCloseMessage(const CloseControlMessage& message);
  void OnPingMessage(const PingControlMessage& message);
  void OnPongMessage(const PongControlMessage& message);
*/

  void SetMessageCallback(const MessageCallback& cb) {
    message_callback_ = cb;
  }
  void SetErrorCallback(const ErrorCallback& cb) {
    error_callback_ = cb;
  }

 private:
  void OnParsedFrame(const WebsocketFrame& frame);
  bool OnContinuationFrame(const WebsocketFrame& frame);
  bool OnTextFrame(const WebsocketFrame& frame);
  bool OnBinaryFrame(const WebsocketFrame& frame);
  bool OnCloseFrame(const WebsocketFrame& frame);
  bool OnPingFrame(const WebsocketFrame& frame);
  bool OnPongFrame(const WebsocketFrame& frame);
  bool HandleFrameBody(const WebsocketFrame& frame, std::string& handled_body);

  void OnMessage(const WebsocketMessage& message) {
    message_callback_(message);
  }

  void OnCodecError(std::string reason);

  WebsocketParser parser_;
  bool is_expected_masking_;
  WebsocketMessage::MessageType cur_message_type_;
  WebsocketMessage message_;
  MessageCallback message_callback_;
  ErrorCallback error_callback_;
};
}

#endif
