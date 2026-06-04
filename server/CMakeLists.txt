cmake_minimum_required(VERSION 3.16)
project(asio_demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# --- Asio interface library ---
# Everything that needs Asio links to this and gets all settings for free
add_library(asio_config INTERFACE)
target_include_directories(asio_config INTERFACE ${CMAKE_SOURCE_DIR}/external/asio)
target_compile_definitions(asio_config INTERFACE
    ASIO_STANDALONE
    ASIO_NO_DEPRECATED
    _WIN32_WINNT=0x0601
)
target_link_libraries(asio_config INTERFACE
    Threads::Threads
    ws2_32
    mswsock
)

add_subdirectory(libs)

add_executable(server main.cpp)
target_link_libraries(server PRIVATE server_src)