#include "html_parser.h"

HtmlParser::HtmlParser()
    : html_(), htmlHandled_(), htmlTags_("<[^>]*>"),
      nonAlnum_("[^a-zA-Z0-9 ]") {}

void HtmlParser::setHtml(const std::string html) { html_ = html; }

std::string HtmlParser::getHandledHtml() const { return htmlHandled_; }

void HtmlParser::handleHtml() {
  htmlHandled_ = std::regex_replace(html_, htmlTags_, "");
  htmlHandled_ = std::regex_replace(htmlHandled_, nonAlnum_, "");
}