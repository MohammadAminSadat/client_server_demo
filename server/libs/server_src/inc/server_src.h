#pragma once
#include <asio.hpp>
#include <iostream>
#include <memory>

#include "session.h"

using asio::ip::tcp;

class Server {
public:
  Server(asio::io_context &io, unsigned short port);

private:
  void do_accept();

  tcp::acceptor acceptor_;
};