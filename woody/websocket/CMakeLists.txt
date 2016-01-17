SET(woody_websocket_SRCS
  websocket_codec.cc
  websocket_frame.cc
  websocket_handler.cc
  websocket_message.cc
  websocket_parser.cc
  )

ADD_LIBRARY(woody_websocket ${woody_websocket_SRCS})

