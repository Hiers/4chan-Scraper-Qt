#pragma once
#include <string>
#include <curl/curl.h>

class Curl {
    public:
        Curl();
        ~Curl();
        std::string buffer;
        CURL* get;
        
        void curl_setup(char *URL);
        static size_t write_data(char *ptr, size_t size, size_t nmemb, std::string *ourpointer);
        
};
