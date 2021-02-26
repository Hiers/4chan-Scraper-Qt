#pragma once
#include <iostream>
#include "curl.h"

class Xml {
    public :
        Xml();
        ~Xml();
        size_t sizeofxml;
        char* htmlbuf;
        const char* reply_img;
        const char* op_img;
        int stop;

        void htmltoxml(std::string data);
        int parsexml(Curl *curl);
        void stopCurl();
    
};
