#pragma once

#include "../../utils/http_utils.h"
#include <regex>
#include <string>
#include <vector>

/**
 * @brief links-геттер.
 * @details После скачивания страницы «Паук» собирает все ссылки с неё и
 * добавляет их в очередь на скачивание.
 */
class LinksGetter {
public:
  /**
   * @brief Конструктор.
   */
  LinksGetter();

  /**
   * @brief Установить HTML-страницу.
   * HTML-страница в виде строки.
   * @param html HTML-страница.
   */
  void setHtml(const std::string html);

  /**
   * @brief Установить ссылку на текущую HTML-страницу.
   * @param link Ссылка на HTML-страницу.
   */

  void setCurrentLink(const httputils::Link &link);

  /**
   * @brief Получить ссылки.
   * @return Ссылки с текущей HTML-страницы.
   */
  std::vector<httputils::Link> getLinks() const;

  /**
   * @brief Получить протокол HTML-страницы.
   * @param ptococolType Тип протокола ссылки.
   * @return Протокол текущей HTML-страницы.
   */
  std::string getProtocol(const httputils::ProtocolType &protocolType);

private:
  //! HTML-страница.
  std::string html_;
  //! Ссылка HTML-страницы.
  httputils::Link link_;
  //! Ссылки.
  std::vector<httputils::Link> links_;
  //! HTML-тег.
  std::regex htmlTag_;
  //! URL-адрес.
  std::regex urlRegex_;

  /**
   * @brief Обработать HTML-страницу.
   */
  void handleHtml();

  /**
   * @brief Установить тип протокола.
   * @param ptococol Протокол ссылки в виде строки.
   * @return Протокол ссылки.
   */
  httputils::ProtocolType setProtocolType(const std::string protocol);
};
