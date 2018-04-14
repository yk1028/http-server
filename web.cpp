#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include <fstream>
#include "web.h"

using namespace std;

Web::Web(int clnt_sock){

    this->root = getcwd(NULL,0);
    this->clnt_sock = clnt_sock;
    this->response = "";
    this->statusCode = 200;
    this->httpVersion = "HTTP/1.1";

    receiveHttpRequest();
    parseHttpRequest();
    generateHttpResponse();
    sendHttpResponse();
}

//read http request from client socket
void Web::receiveHttpRequest(){

    read(clnt_sock, buf, BUF_SIZE);

}

//http request parsing
void Web::parseHttpRequest(){

    vector<char *> line;

    char *ptr = strtok(buf, "\r\n");

    while (ptr != NULL) {
        line.push_back(ptr);
        ptr = strtok(NULL, "\r\n");
    }
    parseFirstLine(line[0]);
    for (int i = 1; i < line.size(); i++) {
        parseHeaderLine(line[i]);
    }

}

//start line parsing
void Web::parseFirstLine(char* line) {

    char *ptr = strtok(line, " ");
    this->requestinfo.setMethod(ptr);
    ptr = strtok(NULL, " ");
    this->requestinfo.setTarget(ptr);
    ptr = strtok(NULL, " ");
    this->requestinfo.setHttpVersion(ptr);

}

//header line parsing
void Web::parseHeaderLine(char* line) {

    char *ptr1;
    char *ptr = strtok(line, ":");
    ptr1 = ptr;
    ptr = strtok(NULL, "\0");
    ptr++; //delete white space
    string str1(ptr1);
    string str(ptr);
    this->requestinfo.setHeader(str1,str);

}

//generate http response
void Web::generateHttpResponse(){

    if(this->requestinfo.getHttpVersion().compare("HTTP/1.1") != 0){
        statusCode = 400;
    }else {
        this->response += generateResponseHeader();
        if (this->requestinfo.getMethod().compare("GET") == 0) {
            this->response += generateResponseBody();
        } else {
            this->response += "myserver supports only GET method yet";
        }
    }

    //http response start line
    this->response =  this->httpVersion + generateStatusMessage() + this->response;

}

//generate http status message
string Web::generateStatusMessage(){

    switch(this->statusCode){
        case 200:
            return " 200 OK\r\n";
        case 400:
            return " 400 Bad Request\r\n";
        case 404:
            return " 404 Not Found\r\n";
        default:
            return " 500 Internal Server Error\r\n";
    }

}

//generate http response header
string Web::generateResponseHeader(){

    string responseHeader = "";

    responseHeader += "Server: myserver\r\n";
    responseHeader += "Content-Type: text/html\r\n";
    responseHeader += "Connection: close\r\n";
    responseHeader += "\r\n";

    return responseHeader;
}

//generate http response body
string Web::generateResponseBody(){

    string path = this->root + this->requestinfo.getTarget();
    string responseBody,line;
    ifstream targetFile;

    if(this->requestinfo.getTarget() == "/"){
        path += "index.html";
    }

    targetFile.open(path);

    if (!targetFile.good()){
        this->statusCode = 404;
        return "<h1>404 fail</h1>";
    }

    while(getline(targetFile, line))
    {
        responseBody += line;
    }

    return responseBody;
}

void Web::sendHttpResponse(){

    send(this->clnt_sock, this->response.c_str(), strlen(this->response.c_str()), 0);

}
