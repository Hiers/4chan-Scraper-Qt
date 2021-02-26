#include "curl.h"

Curl::Curl(){
    //you have to use this to initialize curl functions
    curl_global_init(CURL_GLOBAL_DEFAULT);
    //we make a handle that we use later to set options
    get = curl_easy_init();
}

Curl::~Curl(){
    curl_easy_cleanup(get);
}

static size_t Curl::write_data(char *ptr, size_t size, size_t nmemb, std::string *ourpointer){

   //throwing the result curl gets into buffer so we can use it later for parsing 
    ourpointer->append(ptr, size*nmemb);
    return size*nmemb;

}

void Curl::curl_setup(char *URL){
    curl_easy_setopt(get, CURLOPT_URL, URL);

    //setting up the callback func
    curl_easy_setopt(get, CURLOPT_WRITEFUNCTION, write_data);

    //telling curl to write to the buffer
    curl_easy_setopt(get, CURLOPT_WRITEDATA, &buffer);
}
