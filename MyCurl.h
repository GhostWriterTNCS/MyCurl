#pragma once

#include <string>
#include "curl/curl.h"

/**
	A small collection of functions working with curl.
*/
namespace MyCurl {

/**
	Parse the html by url and return the content as string.
*/
std::string urlToString(std::string url);

/**
	Parse the html by url and create a file from the page content.
	Returns true if success, otherwise false.
*/
bool urlToFile(std::string url, std::string filename);

/**
	Find and replace html codes in string.
*/
std::string decodeHtml(std::string html);

/**
	Get the redirected url for an url.
*/
std::string redirectedUrl(std::string url);

} // namespace MyCurl
