#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include <iostream>
#include <string>

#include "http_connection.h"

namespace {

void httpServer(tcp::acceptor &acceptor, tcp::socket &socket) {
  acceptor.async_accept(socket, [&](beast::error_code ec) {
    if (!ec)
      std::make_shared<httpsrvr::HttpConnection>(std::move(socket))->start();
    httpServer(acceptor, socket);
  });
}

} // namespace

int main(int argc, char *argv[]) {
  try {

    auto const address = net::ip::make_address("0.0.0.0");
    unsigned short port = 8081;

    net::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};
    tcp::socket socket{ioc};
    httpServer(acceptor, socket);

    std::cout << "Open browser and connect to http://localhost:8081 to see the "
                 "web server operating"
              << std::endl;

    ioc.run();
  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
