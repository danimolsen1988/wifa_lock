#include "TlsClientHandler.h"


TlsClientHandler::TlsClientHandler(/* args */)
{
}

TlsClientHandler::~TlsClientHandler()
{
}
void TlsClientHandler::setup() {
        // need a Particle time sync for X509 certificates verify.
    if (millis() - lastSync > ONE_DAY_MILLIS) {
        Particle.syncTime();
        lastSync = millis();
    }
}


int TlsClientHandler::Detect(uint8_t * img, uint32_t length) {
    
    memset(buff, 0, sizeof(buff));
    http.setPath(DETECT_PATH);
    http.setHost(HOST);
    http.setKey(KEY);
    http.setContent(http.HTTP_OCTET);
    http.setBody(img);
    http.setContentLength(length); //set to the right size
            // setup Root CA pem. have to be done before every connect! 
    client.init(letencryptCaPem, sizeof(letencryptCaPem));
    //connect to client on dis, port
#if DEBUG_AZURE == 1
    Serial.println(http.makeDetect());
#endif
    client.connect(this->URL, 443);

    // Send header to azure.
    int len = sprintf((char *)buff, http.makeDetect());
    client.write(buff, len );
    client.write(img,length);;
    // GET HTTPS response.
    memset(buff, 0, sizeof(buff));
    while(1) {

        // mabye check length and set buffer after this.
        int ret = client.read(buff, sizeof(buff) - 1);
        if (ret == MBEDTLS_ERR_SSL_WANT_READ) {
            delay(100);
        } else if (ret <= 0) {
            break;
        } else {
#if DEBUG_AZURE == 1
            Serial.println((char *)buff);
#endif
        }
    }
    http.setResponse((char*) buff,true);
    int status;
    if((status = http.getStatus()) != 200) {
        return status;
    }
    return this->Verify(http.getFaceId());
}

int TlsClientHandler::Verify(String id) {
    if(id.equals("")) {
        return false;
    }
    http.setPath(VERIFY_PATH);
    http.setHost(HOST);
    http.setKey(KEY);
    http.setContent(http.HTTP_JSON);
    http.setBody(id,this->NIKOLAJ,this->PERSONGROUP);
            // setup Root CA pem. have to be done before every connect! 
    client.init(letencryptCaPem, sizeof(letencryptCaPem));
    //connect to client on dis, port
#if DEBUG_AZURE == 1
    Serial.println(http.getHost());
    Serial.println(http.makeVerify());
#endif
    client.connect(this->URL, 443);

    // Send header to azure.
    int len = sprintf((char *)buff, http.makeVerify());
    client.write(buff, len );
    //make loop that 

    // GET HTTPS response.
    memset(buff, 0, sizeof(buff));
    while(1) {
        // mabye check length and set buffer after this.
        int ret = client.read(buff, sizeof(buff) - 1);
        if (ret == MBEDTLS_ERR_SSL_WANT_READ) {
            delay(100);
        } else if (ret <= 0) {
            break;
        } else {
#if DEBUG_AZURE == 1
            Serial.println((char *)buff);
#endif
        }
    }
    http.setResponse((char*) buff,false);

   if(http.getIsIdentical()) {
       return true;
   }
   return false;
}