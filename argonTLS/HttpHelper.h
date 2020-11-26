#include "Particle.h"
class HttpHelper
{
    typedef struct
{
  String path;
  String host;
  String body;
  uint8_t * octetBody;
  String content;
  String key;
  String length;
} http_request_t;

/**
 * HTTP Response struct.
 * status  response status code.
 * body	response body
 */
typedef struct
{
  int status;
  String body;
} http_response_t;


private:
    const char* HTTP_METHOD_GET     = "GET";
    const char* HTTP_METHOD_POST    = "POST";
    const char* HTTP_METHOD_PUT     = "PUT";
    const char* HTTP_METHOD_DELETE  = "DELETE";
    const char* HTTP_METHOD_PATCH   = "PATCH";

    int length = 0;
    http_request_t httpRequest;
    http_response_t httpResponse;
    String makeHeader();
    String getBodyLength();
public:
    const String  HTTP_JSON ="application/json";
    const String  HTTP_OCTET ="application/octet-stream";

    HttpHelper(/* args */);
    ~HttpHelper();
    void setPath(String path);
    void setHost(String host);
    void setBody(String faceId, String personId, String personGroupId);
    void setBody(uint8_t * body);    
    void setContentLength(uint32_t length);
    void setContent(String content);
    void setKey(String key);
    void setResponeStatus(int status);
    void setResponeBody(String body);
    const char * getHost();

    const char* makeDetect();

    const char * makeVerify();
};

