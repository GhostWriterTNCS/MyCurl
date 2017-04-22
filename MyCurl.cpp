#include <iostream>
#include <vector>
#include "MyCurl.h"

namespace MyCurl {

std::string html; // will hold the url's contents
size_t writeCallback(char* buf, size_t size, size_t nmemb,
					 void* up) { // callback must have this declaration buf is a pointer to the data
								 // that curl has for us, size*nmemb is the size of the buffer
	for (int c = 0; c < size * nmemb; c++) {
		html.push_back(buf[c]);
	}
	return size * nmemb; // tell curl how many bytes we handled
}
std::string urlToString(std::string url) {
	html = "";
	CURL* curl;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // tell curl to output its progress
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return html;
}

std::string decodeHtml(std::string html) {
	std::vector<std::vector<std::string>> encodings = {
		{"&lsquo;", "‘"}, {"&rsquo;", "’"}, {"&ldquo;", "“"}, {"&rdquo;", "”"}, {"&amp;", "&"},
		{"&#33;", "!"},   {"&#033;", "!"},  {"&#38;", "&"},   {"&#038;", "&"},  {"&#39;", "'"},
		{"&#039;", "'"},  {"&#333;", "ō"},  {"&#8730;", "√"}, {"â€“", "–"},		{"âˆš", "√"}};

	for (int i = 0; i < encodings.size(); i++) {
		html = html.replace(html.find(encodings[i][0]), encodings[i][0].length(), encodings[i][1]);
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

} // namspace MyCurl