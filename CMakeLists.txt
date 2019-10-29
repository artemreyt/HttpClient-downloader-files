cmake_minimum_required(VERSION 3.1)
project(HttpDownloader)

include_directories(includes)
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(main src/main.cpp src/HttpClient.cpp)

