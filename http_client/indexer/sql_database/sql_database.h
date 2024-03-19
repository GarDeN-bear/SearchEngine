#pragma once

#include <map>
#include <pqxx/pqxx>
#include <string>

/**
 * @brief Данные для соединения с SQL базой данных (БД).
 */
struct SqlDataConnection {
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
 * @brief SQL БД.
 * @details Для хранения информации о частотности слов будут использоваться две
 * таблицы — «Документы» и «Слова» — с реализованной  между ними связью
 * «многие-ко-многим» с помощью промежуточной таблицы. В эту же промежуточную
 * таблицу будет записана частота использования слов в документе.
 */
class SqlDatabase {
public:
  /**
   * @brief Конструктор.
   * @details Создаёт объект соединения pqxx::connection и заполняет данные для
   * подключения к БД PostgreSQL.
   * @param sqlDataConnection Данные для соединения с SQL базой данных (БД).
   */
  SqlDatabase(const SqlDataConnection &sqlDataConnection);

  /**
   * @brief Деструктор.
   * @details Выполняет отключение от БД и разрушает сам объект
   * SqlDatabase.
   */
  ~SqlDatabase();

  /**
   * @brief Установить URL нового документ.
   * @param URL URL документа.
   */
  void setURL(const std::string URL);

  /**
   * @brief Установить новое слово из документа.
   * @param word Слово.
   */
  void setWord(const std::string word);

  /**
   * @brief Добавить id URL документа, id слова и количество слов в документе в
   * таблицу связку.
   */
  void addIds();

private:
  //! Данные для соединения с SQL БД.
  SqlDataConnection sqlDataConnection_;
  //! Соединение к БД SQL.
  std::unique_ptr<pqxx::connection> c_;
  //! URL нового документа.
  std::string URL_;
  //! Словарь документов и слов.
  std::map<std::string, std::vector<std::string>> documentsWords_;
  //! Словарь слов и их количества в документе.
  std::map<std::string, int> wordsCounts_;

  /**
   * @brief Создать таблицы "documents", "words", "documents_words".
   */
  void createTables();

  /**
   * @brief Создать таблицу "documents".
   */
  void createTableDocuments();

  /**
   * @brief Создать таблицу "words".
   */
  void createTableWords();

  /**
   * @brief Создать таблицу "documents_words".
   */
  void createTableDocumentsWords();

  /**
   * @brief Определить количество слов.
   */
  void findWordsCounts();
};
