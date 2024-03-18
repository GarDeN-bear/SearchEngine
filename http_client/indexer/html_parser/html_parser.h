#pragma once

#include <regex>
#include <string>

/**
 * @brief HTML-парсер.
 * @details Страница очищается от HTML-тегов, от знаков препинания, табуляции,
 * переносов строк и т. д. Остаются только слова, разделённые пробелами; слова
 * переводятся в нижний регистр. Для простоты отбрасываются все слова короче
 * трёх или длиннее 32 символов;
 */
class HtmlParser {
public:
  /**
   * @brief Конструктор.
   */
  HtmlParser();

  /**
   * @brief Установить HTML-страницу.
   * HTML-страница в виде строки.
   * @param html HTML-страница.
   */
  void setHtml(const std::string html);

  /**
   * @brief Получить обработанную HTML-страницу.
   * @return Обработанная HTML-страница в виде строки.
   */
  std::string getHandledHtml() const;

private:
  //! HTML-страница.
  std::string html_;
  //! Обработанная HTML-страница.
  std::string htmlHandled_;
  //! HTML-теги.
  std::regex htmlTags_;
  //! Знаки препинания, табуляции, переносов строк и т.д.
  std::regex nonAlnum_;
  //! Слова короче 3 символов.
  std::regex pattern_;

  /**
   * @brief Обработать HTML-страницу.
   */
  void handleHtml();
};
