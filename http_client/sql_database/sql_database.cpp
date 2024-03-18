#include "sql_database.h"

SqlDatabase::SqlDatabase(const SqlDataConnection &sqlDataConnection)
    : sqlDataConnection_(sqlDataConnection) {
  try {
    c_ = std::make_unique<pqxx::connection>(
        "host=" + sqlDataConnection_.host + " " + "port=" +
        sqlDataConnection_.port + " " + "dbname=" + sqlDataConnection_.dbname +
        " " + "user=" + sqlDataConnection_.user + " " +
        "password=" + sqlDataConnection_.password + " ");
    std::cout << "Connection to SQL database is ready" << std::endl;
  } catch (std::exception &ex) {
    std::cout << "Error: " << ex.what() << std::endl;
  }
}

SqlDatabase::~SqlDatabase() {
  c_->disconnect();
  std::cout << "Disconnection SQL database is ready" << std::endl;
}

// void SqlDatabase::createTable(std::string table) {
//   pqxx::work tx{*c};
//   std::string sqlQueryCrTab = "CREATE TABLE " + table +
//                               " (id SERIAL PRIMARY KEY, "
//                               "first_name VARCHAR(50) NOT NULL, "
//                               "last_name VARCHAR(50) NOT NULL, "
//                               "email VARCHAR(50) UNIQUE NOT NULL, "
//                               "phone_number VARCHAR(1000) UNIQUE);";
//   tx.exec(sqlQueryCrTab);
//   tx.commit();
//   std::cout << "Table created successfully" << std::endl;
// }

// void SqlDatabase::addClient(std::string table, std::string first_name,
//                             std::string last_name, std::string email,
//                             std::string phone_number) {
//   pqxx::work tx{*c};
//   std::string insert =
//       "INSERT INTO " + tx.esc(table) +
//       " (first_name, last_name, email, phone_number) VALUES ('" +
//       tx.esc(first_name) + "', '" + tx.esc(last_name) + "', '" +
//       tx.esc(email) +
//       "', '" + tx.esc(phone_number) + "')";
//   tx.exec(insert);
//   tx.commit();
//   std::cout << "Client added successfully" << std::endl;
// }

// void SqlDatabase::addPhoneNumber(std::string table, int id,
//                                  std::string phone_number) {
//   pqxx::work tx{*c};
//   std::string getPhoneNumber;
//   pqxx::row r = tx.exec1("SELECT phone_number FROM " + tx.esc(table) +
//                          " WHERE id = " + tx.esc(std::to_string(id)));
//   getPhoneNumber = r.begin().as<std::string>();
//   std::string addPhoneNumber;
//   if (getPhoneNumber != "") {
//     addPhoneNumber = "UPDATE " + tx.esc(table) + " SET phone_number = '" +
//                      tx.esc(getPhoneNumber) + ", " + tx.esc(phone_number) +
//                      "' WHERE id = '" + tx.esc(std::to_string(id)) + "'";
//   } else {
//     addPhoneNumber = "UPDATE " + tx.esc(table) + " SET phone_number = '" +
//                      tx.esc(phone_number) + "' WHERE id = '" +
//                      tx.esc(std::to_string(id)) + "'";
//   }
//   tx.exec(addPhoneNumber);
//   tx.commit();
//   std::cout << "Phone number added successfully" << std::endl;
// }

// void SqlDatabase::updateClient(std::string table, int id,
//                                std::string first_name_new,
//                                std::string last_name_new, std::string
//                                email_new, std::string phone_number_new) {
//   pqxx::work tx{*c};
//   std::string updateClient = "UPDATE " + tx.esc(table) + " SET first_name =
//   '" +
//                              tx.esc(first_name_new) + "', last_name = '" +
//                              tx.esc(last_name_new) + "', email = '" +
//                              tx.esc(email_new) + "', phone_number = '" +
//                              tx.esc(phone_number_new) + "' WHERE id = '" +
//                              tx.esc(std::to_string(id)) + "'";
//   tx.exec(updateClient);
//   tx.commit();
//   std::cout << "Client updated successfully" << std::endl;
// }

// void SqlDatabase::dropPhoneNumber(std::string table, int id,
//                                   std::string phone_number) {
//   pqxx::work tx{*c};
//   std::string getPhoneNumber;
//   pqxx::row r = tx.exec1("SELECT phone_number FROM " + tx.esc(table) +
//                          " WHERE id = " + tx.esc(std::to_string(id)));
//   getPhoneNumber = r.begin().as<std::string>();
//   std::string newPhoneNumber = "empty";
//   bool findIndexCheck = getPhoneNumber.find(phone_number) !=
//   std::string::npos; if (findIndexCheck == true) {
//     int findIndex = getPhoneNumber.find(phone_number);
//     if (getPhoneNumber.find(phone_number + ", ") != std::string::npos ||
//         getPhoneNumber.find(", " + phone_number + ", ") != std::string::npos)
//         {
//       getPhoneNumber.erase(findIndex, phone_number.length() + 2);
//     } else if (getPhoneNumber.find(", " + phone_number) != std::string::npos)
//     {
//       getPhoneNumber.erase(findIndex - 2, phone_number.length() + 4);
//     } else {
//       getPhoneNumber.erase(getPhoneNumber.find(phone_number),
//                            phone_number.length());
//     }
//   } else {
//     std::cout << "No such phone number" << std::endl;
//     tx.commit();
//     return;
//   }
//   newPhoneNumber = getPhoneNumber;
//   std::string dropPhoneNumber = "UPDATE " + tx.esc(table) +
//                                 " SET phone_number = '" +
//                                 tx.esc(newPhoneNumber) + "' WHERE id = '" +
//                                 tx.esc(std::to_string(id)) + "'";
//   tx.exec(dropPhoneNumber);
//   tx.commit();
//   std::cout << "Phone number dropped successfully" << std::endl;
// }

// void SqlDatabase::deleteClient(std::string table, int id) {
//   pqxx::work tx(*c);
//   std::string deleteClient = "DELETE FROM " + tx.esc(table) +
//                              " WHERE id = " + tx.esc(std::to_string(id));
//   tx.exec(deleteClient);
//   tx.commit();
//   std::cout << "Client deleted successfully" << std::endl;
// }

// void SqlDatabase::findClient(std::string table, std::string first_name,
//                              std::string last_name, std::string email,
//                              std::string phone_number) {
//   pqxx::work tx(*c);
//   std::string findClient =
//       "SELECT * FROM " + tx.esc(table) + " WHERE first_name = '" +
//       tx.esc(first_name) + "' AND last_name = '" + tx.esc(last_name) +
//       "' AND email = '" + tx.esc(email) + "' AND phone_number = '" +
//       tx.esc(phone_number) + "'";
//   pqxx::result R(tx.exec(findClient));
//   if (R.empty() == 1) {
//     std::cout << "Client wasn't found" << std::endl;
//   } else {
//     for (const auto &el : R) {
//       std::cout << "id: " << el.at(0).as<int>() << std::endl;
//       std::cout << "first_name: " << el.at(1).as<std::string>() << std::endl;
//       std::cout << "last_name: " << el.at(2).as<std::string>() << std::endl;
//       std::cout << "email: " << el.at(3).as<std::string>() << std::endl;
//       std::cout << "phone_number: " << el.at(4).as<std::string>() <<
//       std::endl;
//     }
//   }
//   tx.commit();
// }
