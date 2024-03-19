#pragma once

#include "html_parser/html_parser.h"
#include "links_getter/links_getter.h"
#include "sql_database/sql_database.h"
#include <string>

/**
 * @brief Индексатор.
 * @details Индексатор удаляет HTML-теги из страницы, а затем удаляет все знаки
 * препинания, переносы строк, табуляцию, оставляя только чистый текст. Затем
 * индексатор анализирует текст на странице и сохраняет информацию в базе данных
 * для того, чтобы вести поиск по этим данным.
 */
class Indexer {
public:
  /**
   * @brief Конструктор.
   * @param sqlDataConnection Данные для соединения с SQL БД.
   */
  Indexer(const SqlDataConnection &sqlDataConnection);

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
   * @brief Получить обработанную HTML-страницу.
   * @return Обработанная HTML-страница в виде строки.
   */
  std::string getHandledHtml() const;

private:
  //! SQL база данных.
  SqlDatabase sqlDatabase_;
  //! HTML-парсер.
  HtmlParser htmlParser_;
  //! links-геттер.
  LinksGetter linksGetter_;

  /**
   * @brief Обработать HTML-страницу.
   */
  void handleHtml();
};
