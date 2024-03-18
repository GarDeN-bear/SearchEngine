#include <iostream>
#include <memory>
#include <pqxx/pqxx>

/**
 * @brief Данные для соединения с SQL базой данных.
 */
struct SqlDataConnection
{
  //! Локальный хост.
  std::string host;
  //! Порт подключения.
  std::string port;
  //! Название базы данных.
  std::string dbname;
  //! Имя пользователя.
  std::string user;
  //! Пароль для подключения к базе данных.
  std::string password;
};

/**
 * @brief SQL база данных.
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
   * подключения к базе данных PostgreSQL.
   * @param host Локальный хост.
   * @param port Порт подключения.
   * @param dbname Названиве базы данных.
   * @param user Имя пользователя.
   * @param password Пароль для подключения к базе данных.
   */
  SqlDatabase(const SqlDataConnection & sqlDataConnection);
  /**
   * @brief Деструктор.
   * @details Выполняет отключение от базы данных и разрушает сам объект
   * SqlDatabase.
   */
  ~SqlDatabase();

  // // Метод, создающий структуру БД (таблицы)
  // void createTable(std::string table);
  // // Метод, позволяющий добавить нового клиента
  // void addClient(std::string table, std::string first_name,
  //                std::string last_name, std::string email,
  //                std::string phone_number);
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
  // // Метод, позволяющий найти клиента по его данным (имени, фамилии, email-у или
  // // телефону)
  // void findClient(std::string table, std::string first_name,
  //                 std::string last_name, std::string email,
  //                 std::string phone_number);

private:
  //! Данные для соединения с SQL базой данных.
  SqlDataConnection sqlDataConnection_;
  //! Соединение к базе данных SQL.
  std::unique_ptr<pqxx::connection> c_;
};
