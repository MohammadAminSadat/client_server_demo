add_library(server_src server_src.cpp)
target_include_directories(server_src PUBLIC inc)
target_link_libraries(server_src PUBLIC session)