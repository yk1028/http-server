#ifndef HTTP_SERVER_WEB_H
#define HTTP_SERVER_WEB_H

#include "requestinfo.h"

#define BUF_SIZE 2000

//receive http-request and send http-response
class Web {
private:
    string root;
    int clnt_sock;
    char buf[BUF_SIZE];
    std::string response;
    RequestInfo requestinfo;
    int statusCode;
    string httpVersion;

public:
    Web(int clnt_sock);
    void receiveHttpRequest();
    void parseHttpRequest();
    void parseFirstLine(char* line);
    void parseHeaderLine(char* line);
    void generateHttpResponse();
    std::string generateStatusMessage();
    std::string generateResponseHeader();
    std::string generateResponseBody();
    void sendHttpResponse();

};


#endif //HTTP_SERVER_WEB_H
