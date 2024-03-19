#pragma once

#include "searcher/searcher.h"
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

namespace httpsrvr {

/**
 * @brief HTTP-соединение.
 * @details Принимает запросы, обрабатывает их
 * и отправляет ответы.
 */
class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
  /**
   * @brief Конструктор.
   * @param socket TCP-сокет.
   * @param searcherConnection Данные для подключения к БД.
   */
  HttpConnection(tcp::socket socket,
                 const SearcherConnection &searcherConnection);

  /**
   * @brief Запуск HTTP-соединения.
   */
  void start();

private:
  //! Серверный сокет.
  tcp::socket socket_;
  //! Данные для соединения с SQL БД.
  SearcherConnection searcherConnection_;
  //! Буффер для чтения данных.
  beast::flat_buffer buffer_{8192};
  //! HTTP-запрос.
  http::request<http::dynamic_body> request_;
  //! HTTP-ответ.
  http::response<http::dynamic_body> response_;
  //! Таймер для закрытия сокета.
  net::steady_timer deadline_{socket_.get_executor(), std::chrono::seconds(60)};

  /**
   * @brief Чтение запроса.
   * @details Чтение запроса из сокета и запись в буфер.
   */
  void readRequest();
  /**
   * @brief Обработка запроса.
   */
  void handleRequest();
  /**
   * @brief Создание запроса GET.
   */
  void createResponseGet();
  /**
   * @brief Создание запроса POST.
   */
  void createResponsePost();
  /**
   * @brief Запись ответа.
   */
  void writeResponse();
  /**
   * @brief Проверка необходимости закрытия сокета.
   */
  void checkDeadline();
};

} // namespace httpsrvr
