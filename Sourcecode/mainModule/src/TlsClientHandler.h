#include "TlsTcpClient.h"
#include "HttpHelper.h"
#include "debug.h"
#include "codes.h"

#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)

class TlsClientHandler
{
private:
    const char letencryptCaPem[sizeof(LET_ENCRYPT_CA_PEM)] = LET_ENCRYPT_CA_PEM;
    char URL[52] = "iot-facial-compare-test.cognitiveservices.azure.com";   // have a look at moving this to codes
    unsigned long lastSync = millis();
        unsigned char buff[256*4];
    /* data */
    const String NIKOLAJ = "376bbcec-3679-40fc-9d25-5072d8e29433";
    const String CHRIS ="bb55d87d-9898-438f-b4bb-2259fc30c6be"; //test
    const String DANI = "fe420812-1e70-49d0-a4d0-bf28647ed141";
    const String JAKOB = "c835400d-e89b-4602-bfde-03bdf20285b3";
    
    const String PERSONGROUP = "1";

        TlsTcpClient client;
        HttpHelper http;

    int Verify(String id);
public:
    TlsClientHandler(/* args */);
    ~TlsClientHandler();
    void setup();
    int Detect(uint8_t * img, uint32_t length);
};

