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

void HttpHelper::setBody(uint8_t * body) {
   httpRequest.octetBody = body;
}


void HttpHelper::setContentLength(uint32_t length) {
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
    return  this->makeHeader().c_str();
}

const char * HttpHelper::makeVerify() {
    httpRequest.length = this->getBodyLength();
   
    return  this->makeHeader().c_str();
}
int HttpHelper::getStatus() {
    return httpResponse.status;
}

String HttpHelper::getBodyLength() {
    return String(httpRequest.body.length());  
}

String HttpHelper::getFaceId(){
    return httpResponse.faceId;
}

bool HttpHelper::getIsIdentical(){
    return httpResponse.isIdentical;
}

double HttpHelper::getConfidence(){
    return httpResponse.confidence;
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
    if(httpRequest.content == HTTP_JSON) {
        tmp.concat(httpRequest.body);
        tmp.concat("\r\n\r\n");
    }
    return tmp;
}

void HttpHelper::setResponse(char * respone, bool detect) { 
    httpResponse.confidence = 0;
    httpResponse.faceId = "";
    httpResponse.isIdentical = false;
    
    std::string responeString(respone);

    //get http status as int
    httpResponse.status = atoi(responeString.substr(HTTP_PLACEMENT,HTTP_PLACEMENT+3).c_str()); 
    //get body
    httpResponse.body = responeString.substr((responeString.find("\r\n\r\n")+4));

    JSONValue outerObj = JSONValue::parseCopy(httpResponse.body.c_str());
    JSONArrayIterator iter;
    JSONObjectIterator iterObj;
    if(detect) {
        iter = JSONArrayIterator(outerObj);
        iter.next();    // unpack array
        iterObj = JSONObjectIterator(iter.value());
    } else {
         iterObj  = JSONObjectIterator(outerObj);
    }
    while(iterObj.next()) {   // unpack objects
        if(iterObj.name() == "faceId") {
            //blabla
            httpResponse.faceId = iterObj.value().toString().data();
            
        } else if (iterObj.name() == "isIdentical") {
            httpResponse.isIdentical = iterObj.value().toBool();

        } else if (iterObj.name() == "confidence"){
            httpResponse.confidence = iterObj.value().toDouble();
        } else {
            //error
        }
        
    }

    Serial.println(httpResponse.faceId);
    Serial.println(httpResponse.isIdentical);
    Serial.println(httpResponse.confidence);
}
