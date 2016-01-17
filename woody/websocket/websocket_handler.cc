#include "woody/websocket/websocket_handler.h"

#include <muduo/base/Logging.h>
#include "woody/base/string_util.h"
#include "woody/base/ssl_util.h"

using namespace std;
using namespace woody;

WebsocketHandler::WebsocketHandler(const std::string& name,
    const muduo::net::TcpConnectionPtr& conn)
    : HTTPHandler(name, conn),
      protocol_(kHTTP) {
  ws_codec_.SetErrorCallback(
      boost::bind(&WebsocketHandler::HandleError, this));
  ws_codec_.SetMessageCallback(
      boost::bind(&WebsocketHandler::OnMessageComplete, this, _1));
}

void WebsocketHandler::OnData(muduo::net::Buffer* buf) {
  if (GetProtocol() == kHTTP) {
    HTTPHandler::OnData(buf);
    return;
  }
  // websocket protocol
  LOG_INFO << "HTTPHandler::OnData [" << GetName()
           << "] - protocol: Websocket.";
  while (buf->readableBytes() > 0) {
    string data = convert_to_std(buf->retrieveAllAsString());
    size_t parsed_bytes = 0;
    // It means codec error or we need to wait for more data.
    if (!ws_codec_.OnData(data, parsed_bytes) ||
        !parsed_bytes) {
      break; 
    }

    string unparsed_str(data, parsed_bytes);
    LOG_DEBUG << "unparsed bytes" << unparsed_str.size()
              << "unparsed_str : " << unparsed_str;
    buf->prepend(unparsed_str.c_str(), unparsed_str.size());
    LOG_DEBUG << "readable bytes: " << buf->readableBytes();
  }
}

// You must override this method if you want to handle unupgrade request.
void WebsocketHandler::HandleRequest(const HTTPRequest& req) {
  LOG_INFO << "WebsocketHandler HandleRequest";
  if(req.IsUpgrade()) {
    LOG_INFO << "WebsocketHandler upgrade";
    HandleUpgradeRequest(req);
    return;
  }
  // handle unupgrade request.
  // override the method and add you code here.
}

void WebsocketHandler::HandleUpgradeRequest(const HTTPRequest& req) {
  if (req.GetMethod() != "GET") {
    // error
  }
  if ((req.GetHeader("Upgrade") != "websocket") ||
       req.GetHeader("Connection") != "upgrade") {
    // error
  }

  string key = req.GetHeader("Sec-WebSocket-Key");
  if (!key.empty()) {
    string ws_key = base64_decode(key);
    if (ws_key.size() != 16) {
      //error
    }
  }

  string version = req.GetHeader("Sec-WebSocket-Version");
  if(string_to_int(version) != kVersion) {
    //error
  }
  
  string protocols = req.GetHeader("Sec-WebSocket-Protocol");
  vector<string> protocol_vec;
  if (!protocols.empty()) {
    split(protocols, ",", protocol_vec);
    for(int i = 0, len = protocol_vec.size(); i < len; i ++) {
      trim(protocol_vec[i]);
      AddSubProtocol(protocol_vec[i]);
    }
  }

  // TODO extension

  string temp = key + kWebsocketGUID;
  string accept_key = base64_encode(sha1(temp));

  HTTPResponse resp;
  resp.AddHeader("Upgrade", "websocket")
      .AddHeader("Connection", "Upgrade")
      .AddHeader("Sec-WebSocket-Version", version)
      .AddHeader("Sec-WebSocket-Accept", accept_key)
      .SetStatus(101, "Switching Protocols");
  SetProtocol(kWebsocket);
  HTTPHandler::SendResponse(resp);
}

void WebsocketHandler::OnMessageComplete(const WebsocketMessage& message) {
  HandleWebsocket(message);
}

bool WebsocketHandler::SendWebsocketMessage(const WebsocketMessage& message) {
  WebsocketFrame frame;
  if (!message.SingleFrame(frame)) {
    LOG_ERROR << "message.SingleFrame error";
    return false;
  }
  string frame_string;
  if(!ws_codec_.ConvertFrameToString(frame, frame_string)) {
    LOG_ERROR << "message.SingleFrame error";
    return false;
  }
  Send(frame_string);
  return true;
}

