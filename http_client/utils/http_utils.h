#pragma once

#include "link.h"
#include <string>
#include <vector>

namespace httputils {

//! Тип протокола.
enum ProtocolType {
  //! HTTP-протокол.
  HTTP,
  //! HTTPS-протокол.
  HTTPS
};

//! Парметры ссылки.
struct Link {
  ProtocolType protocol;
  std::string hostName;
  std::string query;
  Link();
  Link(ProtocolType protocol_, std::string hostName_, std::string query_) {
    protocol = protocol_;
    hostName = hostName_;
    query = query_;
  }

  bool operator==(const Link &l) const {
    return protocol == l.protocol && hostName == l.hostName && query == l.query;
  }
};

/**
 * @brief Получить содержимое HTML-страницы.
 * @param link Заданная ссылка.
 * @return Содержимое HTML-страницы.
 */
std::string getHtmlContent(const Link &link);

/**
 * @brief Установить тип протокола.
 * @param ptococol Протокол ссылки в виде строки.
 * @return Протокол ссылки.
 */
httputils::ProtocolType setProtocolType(const std::string protocol);

/**
 * @brief Получить протокол HTML-страницы.
 * @param ptococolType Тип протокола ссылки.
 * @return Протокол текущей HTML-страницы.
 */
std::string getProtocol(const httputils::ProtocolType &protocolType);

} // namespace httputils
