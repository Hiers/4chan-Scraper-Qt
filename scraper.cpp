#include "scraper.h"
#define BUFFER 64


Scraper::Scraper(){  
    xml = new Xml();
    curl = new Curl();
}

Scraper::~Scraper(){
    delete xml;
    delete curl;
}

void Scraper::curlImage(char *URL){

    curl->curl_setup(URL);
    //the magic command that runs curl
    if(curl_easy_perform(curl->get) == CURLE_OK){
        xml->htmltoxml(curl->buffer);
        if(xml->parsexml(curl))//this curls again to get the image
            emit finished(-1);//failed
        else
            emit finished(0);//succeded
    }
    else
        emit finished(-1);
    
}

void Scraper::interruptImage(){
    xml->stopCurl();
}
