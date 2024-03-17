#include "links_getter.h"
// #include <iostream>

LinksGetter::LinksGetter()
    : html_(), link_(httputils::ProtocolType::HTTPS, "", ""), links_(),
      htmlTag_(R"(<a href="([^"]*))"),
      urlRegex_(R"((https?)://([^/]+)(/.*)?$)") {}

void LinksGetter::setHtml(const std::string html) {
  html_ = html;
  handleHtml();
}

void LinksGetter::setCurrentLink(const httputils::Link &link) { link_ = link; }

void LinksGetter::handleHtml() {
  std::smatch match;
  std::string::const_iterator searchStart(html_.cbegin());
  while (std::regex_search(searchStart, html_.cend(), match, htmlTag_)) {
    std::string protocol, hostName, path;
    std::string url = match[1].str();
    std::smatch urlMatch;
    // std::cout << "URL: " << url << std::endl;

    if (std::regex_match(url, urlMatch, urlRegex_)) {
      if (urlMatch.size() == 4) {
        protocol = urlMatch[1].str();
        hostName = urlMatch[2].str();
        path = urlMatch[3].str();
      }
    } else if (url.substr(0, 2) == "//") {
      url = getProtocol() + ":" + url;
      if (std::regex_match(url, urlMatch, urlRegex_)) {
        if (urlMatch.size() == 4) {
          protocol = urlMatch[1].str();
          hostName = urlMatch[2].str();
          path = urlMatch[3].str();
        }
      }
    } else {
      url = getProtocol() + "://" + link_.hostName + url;
      if (std::regex_match(url, urlMatch, urlRegex_)) {
        if (urlMatch.size() == 4) {
          protocol = urlMatch[1].str();
          hostName = urlMatch[2].str();
          path = urlMatch[3].str();
        }
      }
    }

    httputils::Link link{setProtocolType(protocol), hostName, path};
    // std::cout << "ProtocolType: " << protocol << std::endl;
    // std::cout << "HostName: " << hostName << std::endl;
    // std::cout << "Path: " << path << std::endl;
    // std::cout << std::endl;
    if (links_.size() < 10) {
      links_.push_back(link);
    } else {
      break;
    }
    searchStart = match.suffix().first;
  }
}

std::string LinksGetter::getProtocol() {
  std::string protocol;
  switch (link_.protocol) {
  case httputils::HTTP:
    protocol = "http";
    break;
  default:
    protocol = "https";
    break;
  }
  return protocol;
}

httputils::ProtocolType
LinksGetter::setProtocolType(const std::string protocol) {
  httputils::ProtocolType protocolType;
  if (protocol == "http") {
    protocolType = httputils::HTTP;

  } else if (protocol == "https") {
    protocolType = httputils::HTTPS;

  } else {
  }
  return protocolType;
}

std::vector<httputils::Link> LinksGetter::getLinks() const { return links_; }
