#ifndef WOODY_HTTP_HTTPRESPONSE_H
#define WOODY_HTTP_HTTPRESPONSE_H

#include <string>
#include <map>
#include <vector>

namespace woody {
class HTTPResponse {
 public:
  HTTPResponse() { }
  HTTPResponse& SetStatus(int code, const std::string& message);
  HTTPResponse& AddHeader(const std::string& name, const std::string& value);
  HTTPResponse& AddBody(const std::string& body);

  int GetStatusCode() const { return status_code_; }
  std::string GetStatusMessage() const { return status_message_; }
  std::string GetHeaderValue(const std::string& name) const;
  void GetAllHeaders(std::vector<std::string>& name, std::vector<std::string>& value) const;
  std::string GetBody() const { return body_; }

 private:
  // TODO
  int status_code_;
  std::string status_message_;
  std::map<std::string, std::string> headers_map_;
  std::string body_;
};
}

#endif
