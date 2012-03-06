SET(trinichat_SRCS
IRCClient.cpp
IRCClient.h
IRCCmd.cpp
IRCCmd.h
IRCCmde.cpp
IRCFunc.h
IRCIO.cpp
IRCLog.cpp
IRCLog.h
IRCSock.cpp
MCS_OnlinePlayers.cpp
MCS_OnlinePlayers.h
)

include_directories(
  ${ACE_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/src/game/trinichat 
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/include
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/vmap
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_SOURCE_DIR}/src/game
  ${MYSQL_INCLUDE_DIR}
)

add_library(trinichat STATIC ${trinichat_SRCS})
