#include "requestinfo.h"

using namespace std;

RequestInfo::RequestInfo() {
}

string RequestInfo::getMethod() {
    return this->method;
}

string RequestInfo::getTarget() {
    return this->target;
}

string RequestInfo::getHttpVersion() {
    return this->httpVersion;
}

string RequestInfo::getHeader(string header){
    return this->header[header];
}

void RequestInfo::setMethod(char* method){
    this->method = method;
}

void RequestInfo::setTarget(char* target){
    this->target = target;
}

void RequestInfo::setHttpVersion(char* httpVersion){
    this->httpVersion = httpVersion;
}

void RequestInfo::setHeader(string header, string value){
    this->header.insert(pair<string, string>(header, value));
}