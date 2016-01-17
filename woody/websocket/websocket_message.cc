#include "woody/websocket/websocket_message.h"

#include "woody/base/base_util.h"
#include "woody/base/endian_util.h"

using namespace std;
using namespace woody;

void WebsocketMessage::Append(string& data) {
  data_.append(data);
}

void WebsocketMessage::CleanUp() {
  data_.clear();
}

bool WebsocketMessage::SingleFrame(WebsocketFrame& frame) const {
  int fin = 1;
  int rsv1 = 0;
  int rsv2 = 0;
  int rsv3 = 0;
  int opcode = -1;
  int mask = 0;  // Server must not send any masked frame to client.
  switch (type_) {
    case kNoneMessage: {
      return false;
    }
    case kTextMessage: {
      opcode = OPCODE_TEXT;
      break;
    }
    case kBinaryMessage: {
      opcode = OPCODE_BINARY;
      break;
    }
    case kCloseMessage: {
      opcode = OPCODE_CLOSE;
      break;
    }
    case kPingMessage: {
      opcode = OPCODE_PING;
      break;
    }
    case kPongMessage: {
      opcode = OPCODE_PONG;
      break;
    }
    default: {
      assert(false);
    }
  }
  uint32_t masking_key = 0;
  string body = data_;
  if (mask && !body.empty()) {
    unsigned char bytes[4];
    for (int i = 0; i < 4; i ++) {
      bytes[i] = GetRandomByte();
    }
    masking_key = NetworkToHost32(*(uint32_t*)&bytes);
    body = WebsocketFrame::Mask(body, masking_key);
  }
  uint64_t payload_length = body.size();

  frame.Reset(fin, rsv1, rsv2, rsv3, opcode,
              mask, payload_length,
              masking_key,
              body);
  return true;
}

