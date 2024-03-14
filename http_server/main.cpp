#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include <iostream>
#include <string>

#include "../ini_parser/ini_parser.h"
#include "http_connection.h"

namespace {

const std::string configPath =
    "/home/garden/GraduateWork/SearchEngine/configs/config.ini";
const std::string PortSection = "SearchEngine.PortSection";

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
    IniParser iniParser(configPath);
    unsigned short port = 8081;
    try {
      port = static_cast<unsigned short>(iniParser.getValue<int>(PortSection));
    } catch (std::exception &ex) {
      std::cout << ex.what();
    }
    auto const address = net::ip::make_address("0.0.0.0");

    net::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};
    tcp::socket socket{ioc};
    httpServer(acceptor, socket);

    std::cout << "Open browser and connect to http://localhost:" << port
              << " to see the web server operating" << std::endl;

    ioc.run();
  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
