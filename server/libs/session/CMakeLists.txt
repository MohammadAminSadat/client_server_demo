add_library(session session.cpp)

target_include_directories(session PUBLIC inc)

target_link_libraries(session PUBLIC asio_config)