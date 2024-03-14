#pragma once

#include <string>

class HtmlParser {
public:
  HtmlParser();
  void setHtml();
  void getHandledHtml();

private:
  std::string html_;
  std::string htmlHandled_;
  void handleHtml();
};