#pragma once

#include <asio.hpp>
#include <iostream>
#include <memory>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
  explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}
  void start() { do_read(); }

private:
  tcp::socket socket_;
  asio::streambuf buffer_;
  void do_read();
  void do_write(const std::string &msg);
};