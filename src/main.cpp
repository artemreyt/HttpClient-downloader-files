#include "HttpClient.hpp"
#include <iostream>
#include <string>


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " [url]" << std::endl;
		return 1;
	}

	std::string url(argv[1]);
	std::string filename = url.substr(url.rfind('/') + 1);
	HttpClient client(url, filename);
	if (!client.get()) {
		std::cout << "Can't get" << std::endl;
		return 2;
	}
	std::cout << "File downloaded and written in " << filename << std::endl;
	return 0;
}
