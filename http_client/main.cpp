#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "html_parser/html_parser.h"
#include "http_utils/http_utils.h"
#include "ini_parser/ini_parser.h"
#include "links_getter/links_getter.h"
#include <functional>

namespace {

const std::string configPath =
    "/home/garden/GraduateWork/SearchEngine/configs/config.ini";
const std::string StartPageSection = "Crowler.StartPage";
const std::string RecursionDepthSection = "Crowler.RecursionDepth";

} // namespace

std::mutex mtx;
std::condition_variable cv;
std::queue<std::function<void()>> tasks;
bool exitThreadPool = false;

void threadPoolWorker() {
  std::unique_lock<std::mutex> lock(mtx);
  while (!exitThreadPool || !tasks.empty()) {
    if (tasks.empty()) {
      cv.wait(lock);
    } else {
      auto task = tasks.front();
      tasks.pop();
      lock.unlock();
      task();
      lock.lock();
    }
  }
}

void parseLink(const httputils::Link &link, int depth) {
  try {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::string html = getHtmlContent(link);

    if (html.size() == 0) {
      std::cout << "Failed to get HTML Content" << std::endl;
      return;
    }
    // TODO: Parse HTML code here on your own (done)
    HtmlParser htmlParser;
    htmlParser.setHtml(html);

    std::cout << "html content:" << std::endl;
    std::cout << htmlParser.getHandledHtml() << std::endl;

    // TODO: Collect more links from HTML code and add them to the parser like
    // that:

    std::vector<httputils::Link> links = {
        {httputils::HTTP, "en.wikipedia.org", "/wiki/Wikipedia"},
        {httputils::HTTPS, "wikimediafoundation.org", "/"},
    };
    // LinksGetter linksGetter;
    // linksGetter.setCurrentLink(link);
    // linksGetter.setHtml(html);

    // std::vector<httputils::Link> links = linksGetter.getLinks();

    if (depth > 0) {
      std::lock_guard<std::mutex> lock(mtx);

      size_t count = links.size();
      size_t index = 0;
      for (auto &subLink : links) {
        tasks.push([subLink, depth]() { parseLink(subLink, depth - 1); });
      }
      cv.notify_one();
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

int main() {
  try {
    IniParser iniParser(configPath);
    int depth = 1;
    int startPage = 1;
    try {
      depth = iniParser.getValue<int>(RecursionDepthSection);
      startPage = iniParser.getValue<int>(StartPageSection);
    } catch (std::exception &ex) {
      std::cout << ex.what();
    }
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threadPool;

    for (int i = 0; i < numThreads; ++i) {
      threadPool.emplace_back(threadPoolWorker);
    }

    httputils::Link link{httputils::ProtocolType::HTTPS, "en.wikipedia.org",
                         "/wiki/Main_Page"};

    {
      std::lock_guard<std::mutex> lock(mtx);
      tasks.push([link, depth]() { parseLink(link, depth); });
      cv.notify_one();
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
      std::lock_guard<std::mutex> lock(mtx);
      exitThreadPool = true;
      cv.notify_all();
    }

    for (auto &t : threadPool) {
      t.join();
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
