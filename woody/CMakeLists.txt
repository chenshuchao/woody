ADD_SUBDIRECTORY(base)
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(http)
ADD_SUBDIRECTORY(websocket)

SET(woody_SRCS
  base_server.cc
  tcp_server.cc
  )

ADD_LIBRARY(woody ${woody_SRCS})
TARGET_LINK_LIBRARIES(woody woody_websocket woody_http woody_base)
