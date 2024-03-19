#include "sql_database.h"
#include <iostream>

SqlDatabase::SqlDatabase(const SqlDataConnection &sqlDataConnection)
    : sqlDataConnection_(sqlDataConnection), URL_(), documentsWords_() {
  try {
    c_ = std::make_unique<pqxx::connection>(
        "host=" + sqlDataConnection_.host + " " + "port=" +
        sqlDataConnection_.port + " " + "dbname=" + sqlDataConnection_.dbname +
        " " + "user=" + sqlDataConnection_.user + " " +
        "password=" + sqlDataConnection_.password + " ");
  } catch (std::exception &ex) {
    std::cout << "Error: " << ex.what() << std::endl;
  }

  createTables();
}

SqlDatabase::~SqlDatabase() { c_->disconnect(); }

void SqlDatabase::createTables() {
  createTableDocuments();
  createTableWords();
  createTableDocumentsWords();
}

void SqlDatabase::dropTables() {
  //! TODO Не работает, почему не знаю.
  // pqxx::work tx{*c_};
  // const std::string sqlQueryDropTabs = "DROP TABLE IF EXISTS documents_words;
  // "
  //                                      "DROP TABLE IF EXISTS documents; "
  //                                      "DROP TABLE IF EXISTS words; ";
  // tx.exec(sqlQueryDropTabs);
  // tx.commit();
}

void SqlDatabase::createTableDocuments() {
  pqxx::work tx{*c_};
  const std::string sqlQueryCrTab = "CREATE TABLE IF NOT EXISTS documents"
                                    " (id SERIAL PRIMARY KEY, "
                                    "document VARCHAR(200) NOT NULL);";
  tx.exec(sqlQueryCrTab);
  tx.commit();
}

void SqlDatabase::createTableWords() {
  pqxx::work tx{*c_};
  const std::string sqlQueryCrTab = "CREATE TABLE IF NOT EXISTS words"
                                    " (id SERIAL PRIMARY KEY, "
                                    "word VARCHAR(100) NOT NULL);";
  tx.exec(sqlQueryCrTab);
  tx.commit();
}

void SqlDatabase::createTableDocumentsWords() {
  pqxx::work tx{*c_};
  const std::string sqlQueryCrTab =
      "CREATE TABLE IF NOT EXISTS documents_words"
      " (document_id INT REFERENCES documents(id), "
      "word_id INT REFERENCES words(id), "
      "count INT, "
      "CONSTRAINT documents_words_pk PRIMARY KEY(document_id, word_id));";
  tx.exec(sqlQueryCrTab);
  tx.commit();
}

void SqlDatabase::findWordsCounts() {
  for (int i = 0; i < documentsWords_[URL_].size(); ++i) {
    wordsCounts_[documentsWords_[URL_][i]]++;
  }
}

void SqlDatabase::setURL(const std::string URL) { URL_ = URL; }

void SqlDatabase::setWord(const std::string word) {
  documentsWords_[URL_].push_back(word);
}

void SqlDatabase::addIds() {
  pqxx::work tx1{*c_};
  std::string insert =
      "SELECT * FROM documents WHERE document = '" + tx1.esc(URL_) + "';";
  const pqxx::result check = tx1.exec(insert);
  tx1.commit();
  if (check.empty()) {
    pqxx::work tx2{*c_};
    std::string insert =
        "INSERT INTO documents (document) VALUES ('" + tx2.esc(URL_) + "');";
    tx2.exec(insert);
    tx2.commit();
  }

  pqxx::work t3{*c_};
  pqxx::result documentIdresult = t3.exec("SELECT MAX(id) FROM documents;");
  const std::string documentId = documentIdresult[0][0].as<std::string>();
  t3.commit();

  for (int i = 0; i < documentsWords_[URL_].size(); ++i) {
    pqxx::work tx4{*c_};
    insert = "INSERT INTO words (word) VALUES ('" +
             tx4.esc(documentsWords_[URL_][i]) + "');";
    tx4.exec(insert);
    tx4.commit();

    pqxx::work tx5{*c_};
    pqxx::result wordIdresult = tx5.exec("SELECT MAX(id) FROM words;");
    const std::string wordId = wordIdresult[0][0].as<std::string>();
    tx5.commit();

    pqxx::work tx6{*c_};
    insert = "INSERT INTO documents_words VALUES (" + tx6.esc(documentId) +
             ", " + tx6.esc(wordId) + ", " + tx6.esc(std::to_string(0)) + ");";
    tx6.exec(insert);
    tx6.commit();
  }
  findWordsCounts();

  for (int i = 0; i < documentsWords_[URL_].size(); ++i) {
    pqxx::work tx{*c_};
    const std::string insert =
        "UPDATE documents_words SET count = " +
        tx.esc(std::to_string(wordsCounts_[documentsWords_[URL_][i]])) +
        " WHERE (word_id IN (SELECT id FROM words WHERE word = '" +
        tx.esc(documentsWords_[URL_][i]) +
        "')) AND (document_id = (SELECT id FROM documents WHERE "
        "document = '" +
        tx.esc(URL_) + "'));";
    tx.exec(insert);
    tx.commit();
  }
}
