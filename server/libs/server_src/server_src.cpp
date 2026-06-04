#include "server_src.h"

Server::Server(asio::io_context &io, unsigned short port)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
  do_accept();
}

void Server::do_accept() {
    acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
      if (!ec) {
        std::cout << "New connection from " << socket.remote_endpoint() << "\n";
        std::make_shared<Session>(std::move(socket))->start();
      }
      do_accept(); // keep listening
    });
  }
