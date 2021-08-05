cmake_minimum_required(VERSION 3.2)

project(uwbLocation)

include_directories(
  /home/node/Desktop/zhangqr-linux/uwbLocalize/include
)

aux_source_directory(src DIR_SRCS)

add_executable(${PROJECT_NAME} 
src/main.cpp 
src/load_config.cpp
src/location.cpp 
src/openDev.cpp
)

target_link_libraries(uwbLocation)
