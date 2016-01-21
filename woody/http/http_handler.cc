#include "woody/http/http_handler.h"

#include <muduo/base/Logging.h>

#include "woody/base/string_util.h"

using namespace std;
using namespace woody;

HTTPHandler::HTTPHandler(const string& name,
                         const muduo::net::TcpConnectionPtr& conn)
    : BaseHandler(name, conn) {
  codec_.SetMessageBeginCallback(
      boost::bind(&HTTPHandler::OnMessageBegin, this, _1));
  codec_.SetMessageCompleteCallback(
      boost::bind(&HTTPHandler::OnMessageComplete, this, _1, _2));
  codec_.SetErrorCallback(
      boost::bind(&HTTPHandler::HandleError, this));
}

void HTTPHandler::OnData(muduo::net::Buffer* buf) {
  // http protocol
  LOG_INFO << "HTTPHandler::OnData [" << GetName()
           << "] - protocol: HTTP.";
  while (buf->readableBytes() > 0) {
    string data = convert_to_std(buf->retrieveAllAsString());
    size_t bytes_parsed = codec_.OnData(data);
    if (!bytes_parsed) {
      // It means we need to wait for more data.
      break; 
    }
    string unparsed_str(data, bytes_parsed);
    LOG_DEBUG << "unparsed bytes" << unparsed_str.size()
              << "unparsed_str : " << unparsed_str;
    buf->prepend(unparsed_str.c_str(), unparsed_str.size());
    LOG_DEBUG << "readable bytes: " << buf->readableBytes();
  }
}

void HTTPHandler::OnMessageBegin(HTTPCodec::StreamID id) { }

void HTTPHandler::OnMessageComplete(HTTPCodec::StreamID id, HTTPRequest& info) {
  LOG_INFO << "HTTPHandler::OnMessageComplete []";
  request_complete_callback_(info);
}

void HTTPHandler::FormatResponse(HTTPResponse& resp) {
  resp.AddHeader("Content-Length", int_to_string(resp.GetBody().size()));
}

void HTTPHandler::SendResponse(HTTPResponse& resp) {
  FormatResponse(resp);
  string msg = codec_.ConvertResponseToString(resp);
  LOG_INFO << "HTTPHandler::Send [" << GetName()
           << "] - response:\n" << msg;
  Send(msg);
}

