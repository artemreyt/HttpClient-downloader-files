#include <iostream>
#include <string>
#include <fstream>
extern "C" {
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
}

#define HTTP_PORT 80
#define HTTP_VERSION "1.1"

#define BUF_SIZE 2048

class HttpClient {
public:
    HttpClient(const std::string &_url, const std::string &_filename);
    bool get();
    std::string getHost() const ;
    std::string getProtocol() const ;
    std::string getPath() const ;
    std::string getParams() const ;
    static std::string getHeader(const std::string &response, const std::string &header);

private:
    int sock;
    struct sockaddr_in addr;
    struct hostent* raw_host;
    const std::string url;
    const std::string filename;

    bool connect();
    bool sendAll(const std::string &request);
    bool receive();
};
