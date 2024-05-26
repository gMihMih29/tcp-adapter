# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.10)

project (tcp-adapter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

file(GLOB TCP_ADAPTER_SRC CONFIGURE_DEPENDS
    "src/tcp/*.hpp"
    "src/tcp/*.cpp"
)

set(ECHO_CLIENT_EXAMPLE_SRC
    "src/example/EchoClient.cpp"
)

set(ECHO_SERVER_EXAMPLE_SRC
    "src/example/EchoServer.cpp"
)

add_executable(EchoClientExample ${ECHO_CLIENT_EXAMPLE_SRC})
add_executable(EchoServerExample ${ECHO_SERVER_EXAMPLE_SRC})

add_library(TCP_ADAPTER ${TCP_ADAPTER_SRC})
add_library(${PROJECT_NAME} ${TCP_ADAPTER_SRC})
target_link_libraries(${PROJECT_NAME} TCP_ADAPTER)

target_include_directories(EchoClientExample PRIVATE "include")
target_include_directories(EchoServerExample PRIVATE "include")

target_link_libraries(EchoClientExample ${PROJECT_NAME})
target_link_libraries(EchoServerExample ${PROJECT_NAME})
