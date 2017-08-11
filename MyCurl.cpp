#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "MyCurl.h"

namespace MyCurl {

/**
	Convert html content to string.
*/
size_t writeString(char* buf, size_t size, size_t nmemb, std::wstring* html) {
	for (int c = 0; c < size * nmemb; c++) {
		html->push_back(buf[c]);
	}
	return size * nmemb; // tell curl how many bytes we handled
}
std::string urlToString(std::string url) {
	std::wstring html; // wstring can work with UTF8 encoding
	CURL* curl;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // tell curl to output its progress

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return std::string(html.begin(), html.end());
}

/**
	Convert html content to file.
*/
size_t writeFile(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written; // tell curl how many bytes we handled
}
bool urlToFile(std::string url, std::string filename) {
	CURL* curl;
	FILE* file;
	if (fopen_s(&file, filename.c_str(), "wb") == 0) {
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeFile);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // tell curl to output its progress

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(file);
		return true;
	}
	return false;
}

std::string decodeHtml(std::string html) {
	// TODO: add missing codes.
	std::vector<std::vector<std::string>> encodings = {
		{"&lsquo;", "‘"}, {"&rsquo;", "’"}, {"&ldquo;", "“"}, {"&rdquo;", "”"}, {"&amp;", "&"},
		{"&#33;", "!"},   {"&#033;", "!"},  {"&#38;", "&"},   {"&#038;", "&"},  {"&#39;", "'"},
		{"&#039;", "'"},  {"&#333;", "ō"},  {"&#8730;", "√"}, {"â€“", "–"},		{"âˆš", "√"}};

	for (int i = 0; i < encodings.size(); i++) {
		if (html.find(encodings[i][0]) != std::string::npos) {
			html =
				html.replace(html.find(encodings[i][0]), encodings[i][0].length(), encodings[i][1]);
		}
	}
	return html;
}

std::string redirectedUrl(std::string url) {
	char* redirect;
	CURL* curl;
	CURLcode curl_res;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

	// Perform the request, curl_res will get the return code
	curl_res = curl_easy_perform(curl);

	// Check for errors
	if (curl_res != CURLE_OK)
		std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(curl_res) << std::endl;
	else {
		curl_res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &redirect);
		if ((curl_res == CURLE_OK) && redirect) {
			std::cout << "CURLINFO_EFFECTIVE_URL: " << redirect << std::endl;
			return redirect;
		}
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return "";
}

} // namespace MyCurl
