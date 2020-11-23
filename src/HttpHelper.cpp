#include "HttpHelper.h"
HttpHelper::HttpHelper(/* args */)
{
}

HttpHelper::~HttpHelper()
{
}

void HttpHelper::setPath(String path) {
    httpRequest.path = path;
}
void HttpHelper::setHost(String host) {
    httpRequest.host = host;
}
void HttpHelper::setBody(String faceId, String personId, String personGroupId) {
    String tmp = "{";
    tmp.concat("\"faceId\":\"");
    tmp.concat(faceId);
    tmp.concat("\",\"personId\":\"");
    tmp.concat(personId);
    tmp.concat("\",\"PersonGroupId\":\"");
    tmp.concat(personGroupId);
    tmp.concat("\"}\r\n\r\n");
    httpRequest.body = tmp;

}
void HttpHelper::setContentLength(int length) {
    httpRequest.length = String(length);
}


void HttpHelper::setContent(String content) {
    httpRequest.content = content;
}

void HttpHelper::setKey(String key) {
    httpRequest.key = key;
}

void HttpHelper::setResponeStatus(int status) {
    this->httpResponse.status = status;
}

void HttpHelper::setResponeBody(String body) {
    this->httpResponse.body = body;
}

const char * HttpHelper::getHost() {
    return httpRequest.host.c_str();
}

const char * HttpHelper::makeDetect() {
    String tmp;
}

const char * HttpHelper::makeVerify() {
    httpRequest.length = this->getBodyLength();
   
    return  this->makeHeader().c_str();
}

String HttpHelper::getBodyLength() {
    return String(httpRequest.body.length());  
}

String HttpHelper::makeHeader() {
    String tmp;
    tmp.concat(HTTP_METHOD_POST);
    tmp.concat(" ");
    tmp.concat(httpRequest.path);
    tmp.concat(" HTTP/1.0");
    tmp.concat("\r\n");
    tmp.concat("Ocp-Apim-Subscription-Key:");
    tmp.concat(httpRequest.key);
    tmp.concat("\r\n");
    tmp.concat("Content-Type:");
    tmp.concat(httpRequest.content);
    tmp.concat("\r\n");
    tmp.concat("Host:");
    tmp.concat(httpRequest.host);
    tmp.concat("\r\n");
    tmp.concat("Content-Length:");
    tmp.concat(httpRequest.length);
    tmp.concat("\r\n\r\n");
    if(httpRequest.content == HTTP_JSON){
        tmp.concat(httpRequest.body);
    }

    return tmp;
}