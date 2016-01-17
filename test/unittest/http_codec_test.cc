#include <string>
#include <gtest/gtest.h>
#include <woody/http/http_codec.h>
#include <woody/http/http_request.h>

using namespace std;
using namespace woody;

string CreateSimpleRequest() {
  string req("GET / HTTP/1.1\nHost: www.baidu.com\n\n");
  return req;
}

  string req = "GET /hello/haha/ HTTP/1.1\r\nHost: 10.211.55.6:5011Connection: keep-alive\r\nCache-Control: max-age=0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.106 Safari/537.36\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: zh-CN,zh;q=0.8,en;q=0.6,st;q=0.4\r\n\r\n";

bool is_message_begin = false;
void OnMessageBegin(HTTPCodec::StreamID id) {
  is_message_begin = true;
}

TEST(OnDataTest, simple) {
  HTTPCodec codec;
  HTTPCodec::MessageBeginCallback f_begin = boost::bind(OnMessageBegin, _1);
  codec.SetMessageBeginCallback(f_begin);
  string req = CreateSimpleRequest();
  codec.OnData(req);
  ASSERT_EQ(is_message_begin, true);
}

