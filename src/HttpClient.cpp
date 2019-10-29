#include "HttpClient.hpp"

HttpClient::HttpClient(const std::string &_url, const std::string &_filename): 
    url(_url), filename(_filename) {}

bool HttpClient::get() {
    std::string protocol = getProtocol();
    if (protocol != "http://") {
        std::cerr << "Don't support no HTTP protocol" << std::endl;
        return false;
    }
    std::string host_name = getHost();

    std::string request = "GET ";
    request += url + " HTTP/" + HTTP_VERSION + "\r\n";
    request += "Host: " + host_name + "\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Can't create socket" << std::endl;
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(HTTP_PORT);

    raw_host = gethostbyname(host_name.c_str());
    if (raw_host == NULL) {
        std::cerr << "No such host: " << host_name << std::endl;
        return false;
    }

    if (!this->connect()) {
        std::cerr << "Can't connect" << std::endl;
        return false;
    } else {
        std::cout << "Connection established" << std::endl;
    }

    if (!sendAll(request)) {
        std::cerr << "Error while sending HTTP request" << std::endl;
        return false;
    }

    if (!receive()) {
        std::cerr << "Error while receiving HTTP response" << std::endl;
        return false;
    }

    close(sock);
    return true;
}

bool HttpClient::sendAll(const std::string &request) {
    const char *message = request.c_str();
    size_t length = request.size();

    size_t total = 0;
    while (total < length) {
        int sended = send(sock, message + total, length - total, 0);

        if (sended < 0)
            return false;
        total += sended;
    }
    return true;
}

bool HttpClient::receive() {
    char buf[BUF_SIZE];
    std::string response = "";
    std::ofstream file;

    while (1) {
        size_t bytes_read = recv(sock, buf, BUF_SIZE - 1, 0);

        if (bytes_read < 0)
            return false;

        if (!file.is_open()) {
            response.append(buf, bytes_read);
            std::string content = getHeader(response, "Content-Type");

            if (!content.empty()) {
                if (content.find("text/") == std::string::npos)
                    file.open(filename, std::ios::binary);
                else
                    file.open(filename);

                std::string::size_type start_file = response.find("\r\n\r\n");
                file << response.substr(start_file + 4);
            }
        }
        else
            file.write(buf, bytes_read);

        if (bytes_read == 0) {
            file.close();
            break;
        }
    }
    return true;
}

// Подключаемся по очереди к разным IP домена, пока не будет удачного соединения
bool HttpClient::connect() {
    size_t i = 0;

    while (raw_host->h_addr_list[i]) {
        memcpy((char *)&addr.sin_addr, (char *)raw_host->h_addr_list[i++], raw_host->h_length);
        if (::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            continue;
        return true;
    }
    return false;
}

std::string HttpClient::getProtocol() const {
    size_t slash_pos = url.find("://");
    if (slash_pos == std::string::npos)
        return std::string();

    return url.substr(0, slash_pos + 3);
}

std::string HttpClient::getHost() const {
    size_t protocol_size = getProtocol().size();
    size_t slash_pos = url.find('/', protocol_size);
    if (slash_pos == std::string::npos)
        return url.substr(protocol_size);
    
    return url.substr(protocol_size, slash_pos - protocol_size);
}

std::string HttpClient::getPath() const {
    size_t start_position = getProtocol().size() + getHost().size();
    size_t end_position = url.find_first_of("?#", start_position);

    return url.substr(start_position, end_position - start_position);
}

std::string HttpClient::getParams() const {
    size_t start_position = getProtocol().size() + getHost().size() \
                            + getPath().size();
    return url.substr(start_position);
}

std::string HttpClient::getHeader(const std::string &response, const std::string &header) {
    std::string::size_type start_position = response.find(header + ":") + header.size() + 1;

    if (start_position != std::string::npos) {
        std::string::size_type end_position = response.find_first_of("\r\n", start_position);

        if (end_position != std::string::npos)
            return response.substr(start_position, end_position - start_position);
    }
    return std::string();
}
