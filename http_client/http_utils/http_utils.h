#pragma once

#include "link.h"
#include <string>
#include <unordered_set>
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

} // namespace httputils
