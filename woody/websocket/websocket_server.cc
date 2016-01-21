#include "woody/websocket/websocket_server.h"

#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <muduo/base/Logging.h>

#include "woody/base/string_util.h"

using namespace std;
using namespace woody;

WebsocketServer::WebsocketServer(muduo::net::EventLoop& loop,
                       int port,
                       const string& name)
    : tcp_server_(&loop,
                  muduo::net::InetAddress(port),
                  convert_to_muduo(name),
                  muduo::net::TcpServer::kNoReusePort) {
  tcp_server_.setConnectionCallback(
      boost::bind(&WebsocketServer::OnCreateOrDestroyConnection, this, _1));
  tcp_server_.setMessageCallback(
      boost::bind(&WebsocketServer::OnData, this, _1, _2, _3));
}

void WebsocketServer::Start() {
  LOG_INFO << "WebsocketServer::Start [" << name_ << "].";
  tcp_server_.start();
}

// TODO
void WebsocketServer::Stop() { }

void WebsocketServer::OnCreateOrDestroyConnection(
         const muduo::net::TcpConnectionPtr& conn) {
  if (conn->connected()) {
    OnConnection(conn);
  }
  else {
    OnDisconnection(conn);
  }
}

void WebsocketServer::OnConnection(const muduo::net::TcpConnectionPtr& conn) {
  CreateHandler(conn);
}

void WebsocketServer::OnDisconnection(const muduo::net::TcpConnectionPtr& conn) {
  WebsocketHandlerPtr handler = FindHandler(conn);
  if (!handler) {
    LOG_ERROR << "WebsocketServer::OnDisconnection [" << name_
              << "] connection [" << conn->name()
              << " handler not found.";
  }
  RemoveHandler(handler);
}

void WebsocketServer::OnData(const muduo::net::TcpConnectionPtr& conn,
                        muduo::net::Buffer* buf,
                        muduo::Timestamp) {
  WebsocketHandlerPtr handler = FindHandler(conn);
  if (!handler) {
    LOG_ERROR << "WebsocketServer::OnData [" << name_
              << "] connecton [" << conn->name()
              << " handler not found.";
    return;
  }
  handler->OnData(buf);
}

void WebsocketServer::OnHandlerError(const WebsocketHandlerPtr& handler) {
  tcp_server_.removeAndDestroyConnection(handler->GetConn());
}

void WebsocketServer::CreateHandler(const muduo::net::TcpConnectionPtr& conn) {
  string conn_name = convert_to_std(conn->name());
  WebsocketHandlerPtr handler(new WebsocketHandler(conn_name, conn));
  handler->SetRequestCompleteCallback(
      boost::bind(&WebsocketServer::HandleRequest, this, _1, _2));
  handler->SetMessageCompleteCallback(
      boost::bind(&WebsocketServer::HandleWebsocket, this, _1, _2));
  handler->SetErrorCallback(
      boost::bind(&WebsocketServer::OnHandlerError, this, _1));
  handler_map_.insert(
      pair<string, WebsocketHandlerPtr>(conn_name, handler));
}

void WebsocketServer::RemoveHandler(const WebsocketHandlerPtr& handler) {
  handler_map_.erase(convert_to_std(handler->GetConn()->name()));
}

WebsocketHandlerPtr WebsocketServer::FindHandler(const muduo::net::TcpConnectionPtr& conn) {
  return handler_map_[convert_to_std(conn->name())];
}

bool WebsocketServer::UpdateHandler(WebsocketHandlerPtr old_handler,
                                    WebsocketHandlerPtr new_handler) {
  if (old_handler->GetConn() != new_handler->GetConn()) {
    // TODO
    // error
    return false;
  }
  string conn_name = old_handler->GetName();
  if (handler_map_.find(conn_name) == handler_map_.end()) {
    // TODO
    // error
    return false;
  }
  handler_map_[conn_name] = new_handler;
  return true;
}
