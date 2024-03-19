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
    // std::cout << "Connection to SQL database is ready" << std::endl;
  } catch (std::exception &ex) {
    std::cout << "Error: " << ex.what() << std::endl;
  }

  createTables();
}

SqlDatabase::~SqlDatabase() {
  c_->disconnect();
  // std::cout << "Disconnection SQL database is ready" << std::endl;
}

void SqlDatabase::createTables() {
  createTableDocuments();
  createTableWords();
  createTableDocumentsWords();
}

void SqlDatabase::createTableDocuments() {
  pqxx::work tx{*c_};
  const std::string sqlQueryCrTab = "CREATE TABLE IF NOT EXISTS documents"
                                    " (id SERIAL PRIMARY KEY, "
                                    "document VARCHAR(100) NOT NULL);";
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
  pqxx::work tx0{*c_};
  std::string insert =
      "SELECT * FROM documents WHERE document = '" + tx0.esc(URL_) + "'";
  pqxx::result check = tx0.exec(insert);

  if (check.empty()) {
    tx0.commit();
    pqxx::work tx1{*c_};
    insert =
        "INSERT INTO documents (document) VALUES ('" + tx1.esc(URL_) + "')";
    tx1.exec(insert);
    tx1.commit();

    pqxx::work tx2{*c_};
    pqxx::result documentIdresult = tx2.exec("SELECT MAX(id) FROM documents");
    const std::string documentId = documentIdresult[0][0].as<std::string>();
    tx2.commit();

    for (int i = 0; i < documentsWords_[URL_].size(); ++i) {
      pqxx::work tx3{*c_};
      insert = "INSERT INTO words (word) VALUES ('" +
               tx3.esc(documentsWords_[URL_][i]) + "')";
      tx3.exec(insert);
      tx3.commit();

      pqxx::work tx4{*c_};
      pqxx::result wordIdresult = tx4.exec("SELECT MAX(id) FROM words");
      const std::string wordId = wordIdresult[0][0].as<std::string>();
      tx4.commit();

      pqxx::work tx5{*c_};
      insert = "INSERT INTO documents_words VALUES (" + tx5.esc(documentId) +
               ", " + tx5.esc(wordId) + ", " +
               tx5.esc(std::to_string(wordsCounts_[documentsWords_[URL_][i]])) +
               ")";
      tx5.exec(insert);
      tx5.commit();
    }

    findWordsCounts();

    for (int i = 0; i < documentsWords_[URL_].size(); ++i) {
      pqxx::work tx6{*c_};
      insert = "UPDATE documents_words SET count = " +
               tx6.esc(std::to_string(wordsCounts_[documentsWords_[URL_][i]])) +
               " WHERE (word_id IN (SELECT id FROM words WHERE word = '" +
               tx6.esc(documentsWords_[URL_][i]) +
               "')) AND (document_id = (SELECT id FROM documents WHERE "
               "document = '" +
               tx6.esc(URL_) + "'))";
      tx6.exec(insert);
      tx6.commit();
    }
  }
}
