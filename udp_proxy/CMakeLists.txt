cmake_minimum_required(VERSION 2.6)

project(PROXY)

aux_source_directory(${PROJECT_SOURCE_DIR} SRCS)

# include *h files
include_directories(${PROJECT_SOURCE_DIR}/../include/raknet ${PROJECT_SOURCE_DIR}) 

# link library
# @remark: for we set the lib dll, here we do not need to link to directory
#link_libraries(${PROJECT_SOURCE_DIR}/../Lib/DLL)

set(RAKNET_DLL_LIBS ${PROJECT_SOURCE_DIR}/../Lib/DLL/libRakNetDLL.so)

add_executable(proxy ${SRCS})
target_link_libraries(proxy ${RAKNET_DLL_LIBS})
