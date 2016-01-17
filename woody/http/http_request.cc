#include "woody/http/http_request.h"

#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace woody;

HTTPRequest::HTTPRequest()
    : is_header_finished_(false),
      is_body_finished_(false),
      is_upgrade_(false) {
}

void HTTPRequest::AddHeader(const string name, const string& value) {
  headers_map_.insert(pair<string, string>(name, value));
}

string HTTPRequest::GetHeader(const string& name) const {
  // TODO return boolean
  map<string, string>::const_iterator it;
  it = headers_map_.find(name);
  if (it != headers_map_.end()) {
    return it->second;
  }
  return "";
}

void HTTPRequest::SetUrl(const std::string& url) {
  url_ = url;
  return;
}

void HTTPRequest::SetMethod(const string& method) {
  //todo check method
  string temp(method);
  //method_ = boost::to_upper_copy<string>(temp);
  cout << method_ << endl;
  method_ = temp;
  return;
}

