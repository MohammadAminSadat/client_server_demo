#include <asio.hpp>
#include <iostream>
#include "server_src.h"

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io;
        Server server(io, 12345);
        std::cout << "Server listening on port 12345...\n";
        io.run(); // blocks until all async work is done
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}