#include <iostream>
#include <memory>
#include <pqxx/pqxx>

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
   * @param host Локальный хост.
   * @param port Порт подключения.
   * @param dbname Названиве БД.
   * @param user Имя пользователя.
   * @param password Пароль для подключения к БД.
   */
  SqlDatabase(const SqlDataConnection &sqlDataConnection);

  /**
   * @brief Деструктор.
   * @details Выполняет отключение от БД и разрушает сам объект
   * SqlDatabase.
   */
  ~SqlDatabase();

  /**
   * @brief Добавить новый документ.
   * @param URL URL документа.
   */
  void addURL(const std::string URL);

  /**
   * @brief Добавить новое слово из документа.
   * @param word Слово.
   */
  void addWord(const std::string word);
  // // Метод, позволяющий добавить телефон для существующего клиента
  // void addPhoneNumber(std::string table, int id, std::string phone_number);
  // // Метод, позволяющий изменить данные о клиенте
  // void updateClient(std::string table, int id, std::string first_name_new,
  //                   std::string last_name_new, std::string email_new,
  //                   std::string phone_number_new);
  // // Метод, позволяющий удалить телефон для существующего клиента
  // void dropPhoneNumber(std::string table, int id, std::string phone_number);
  // // Метод, позволяющий удалить существующего клиента
  // void deleteClient(std::string table, int id);
  // // Метод, позволяющий найти клиента по его данным (имени, фамилии, email-у
  // или
  // // телефону)
  // void findClient(std::string table, std::string first_name,
  //                 std::string last_name, std::string email,
  //                 std::string phone_number);

private:
  //! Данные для соединения с SQL базой данных.
  SqlDataConnection sqlDataConnection_;
  //! Соединение к БД SQL.
  std::unique_ptr<pqxx::connection> c_;

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
};
