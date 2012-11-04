set(src_SRCS 
  PeacAliEnQuery.cpp
)
set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/alien
  ${ALIEN_INCLUDE_DIR}
)
include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/alien
  ${ALIEN_LIBRARIES}
) 
link_directories( ${LINK_DIRECTORIES})

String(REPLACE ".cpp" ".h" HDRS "${src_SRCS}")

add_executable(alien-query alien-query.cpp ${src_SRCS})
target_link_libraries(alien-query ${ALIEN_LIB})

set(INS_TARTGETS alien-query)
install(TARGETS ${INS_TARTGETS}
RUNTIME DESTINATION bin COMPONENT RUNTIME
LIBRARY DESTINATION lib COMPONENT RUNTIME)
