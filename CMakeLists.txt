cmake_minimum_required(VERSION 3.3)
project(kpush)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER "-I/Applications/XAMPP/xamppfiles/include -I/usr/local/include")
set(LIB "-L/Applications/XAMPP/xamppfiles/lib -L/usr/local/lib -lssl -lcrypto")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${HEADER} ${LIB}")

add_custom_target(makefile COMMAND make && ./kpush WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(SOURCE_FILES main.c apns_conn.c config.c)
add_executable(kpush ${SOURCE_FILES})