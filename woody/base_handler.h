#ifndef WOODY_BASEHANDLER_H
#define WOODY_BASEHANDLER_H

#include <string>
#include <boost/enable_shared_from_this.hpp>
#include <muduo/net/Buffer.h>

namespace woody {
class BaseHandler {
 public:
  typedef boost::shared_ptr<BaseHandler> BaseHandlerPtr;
  typedef boost::function<void (BaseHandlerPtr)> ErrorCallback;
  BaseHandler(const std::string& name, 
              const muduo::net::TcpConnectionPtr& conn)
      : name_(name),
        conn_(conn) {
  }
  virtual ~BaseHandler() { }

  virtual std::string GetName() const { return name_; }
  muduo::net::TcpConnectionPtr GetConn() const { return conn_; }

  virtual void OnData(muduo::net::Buffer* buf) = 0;

  virtual void Send(std::string& message) {
    conn_->send(message.c_str(), message.size());
  }

  /* TODO
  virtual void SetErrorCallback(const ErrorCallback& cb) {
    error_callback_ = cb;
  }
  virtual void HandleError() { 
    if (error_callback_) {
      error_callback_(shared_from_this());
    }
  }
  */

 private:
  std::string name_;
  muduo::net::TcpConnectionPtr conn_;
  //ErrorCallback error_callback_;
};
typedef boost::shared_ptr<BaseHandler> BaseHandlerPtr;

class BaseHandlerFactory {
 public:
  virtual BaseHandler* CreateHandler(const std::string& name,
                                     const muduo::net::TcpConnectionPtr& conn) = 0;
};
typedef boost::shared_ptr<BaseHandlerFactory> BaseHandlerFactoryPtr;
}

#endif
