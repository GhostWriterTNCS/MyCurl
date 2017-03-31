#pragma once

#include <string>
#include "curl/curl.h"

namespace MyCurl {

std::string urlToString(std::string url);
std::string decodeHtml(std::string html);
std::string redirectedUrl(std::string url);

} // namespace MyCurl