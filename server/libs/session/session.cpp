#include "session.h"

void Session::do_read() {
  auto self = shared_from_this();
  // Read until newline
  asio::async_read_until(
      socket_, buffer_, '\n',
      [this, self](std::error_code ec, std::size_t length) {
        if (!ec) {
          std::string msg(asio::buffers_begin(buffer_.data()),
                          asio::buffers_begin(buffer_.data()) + length);
          buffer_.consume(length);
          msg.erase(msg.find_last_not_of("\r\n") + 1); // trim
          std::cout << "[client] " << msg << "\n";

          // Echo back with a prefix
          std::string reply = "SERVER ECHO: " + msg + "\n";
          do_write(reply);
        } else {
          std::cout << "Client disconnected.\n";
        }
      });
}

void Session::do_write(const std::string &msg) {
  auto self = shared_from_this();
  asio::async_write(socket_, asio::buffer(msg),
                    [this, self](std::error_code ec, std::size_t /*len*/) {
                      if (!ec)
                        do_read(); // wait for next message
                    });
}