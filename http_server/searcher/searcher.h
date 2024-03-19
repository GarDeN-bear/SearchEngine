#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>

/**
 * @brief Данные для соединения с SQL базой данных (БД).
 */
struct SearcherConnection {
  //! Локальный хост.
  std::string host;
  //! Порт подключения.
  std::string port;
  //! Название БД.
  std::string dbname;
  //! Имя пользователя.
  std::string user;
  //! Пароль для подключения к БД.
  std::string password;
};

/**
 * @brief Поисковик.
 * @details Выполняет поиск по базе данных, ранжирует результат и возвращает его
 * пользователю.
 */
class Searcher {
public:
  /**
   * @brief Конструктор.
   * @details Создаёт объект соединения pqxx::connection и заполняет данные для
   * подключения к БД PostgreSQL.
   * @param sqlDataConnection Данные для соединения с SQL базой данных (БД).
   */
  Searcher(const SearcherConnection &searcherConnection);

  /**
   * @brief Деструктор.
   * @details Выполняет отключение от БД и разрушает сам объект
   * Searcher.
   */
  ~Searcher();

  /**
   * @brief Установить значение для поиска.
   * @param value Значение поиска.
   */
  void setSearchValue(const std::string value);

  /**
   * @brief Возвращает поисковый результат.
   * @return URL документов.
   */
  std::vector<std::string> getSearchResult() const;

private:
  //! Значение для поиска.
  std::string value_;
  //! Результат поиска.
  std::vector<std::string> searchResult_;
};
