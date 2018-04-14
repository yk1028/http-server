#include <map>
#include <string>

#ifndef HTTP_SERVER_REQUESTINFO_H
#define HTTP_SERVER_REQUESTINFO_H

using std::string;

//store http request information
class RequestInfo{
private:
    string method;
    string target;
    string httpVersion;
    std::map<string, string> header; //http header information

public:

    RequestInfo();

    string getMethod();

    string getTarget();

    string getHttpVersion();

    string getHeader(string header);

    void setMethod(char* method);

    void setTarget(char* target);

    void setHttpVersion(char* httpVersion);

    void setHeader(string header, string value);
};

#endif //HTTP_SERVER_REQUESTINFO_H
