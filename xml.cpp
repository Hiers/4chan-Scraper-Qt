#include <iostream>
#include <fstream>
#include <tinyxml2.h>
#include <tidy.h>
#include <tidybuffio.h>
#include "xml.h"

#define BUFFER 64

Xml::Xml(){
    stop = 0;
    htmlbuf = NULL;
}

Xml::~Xml(){
    stop = 1;
    free(htmlbuf);
}

void Xml::htmltoxml(std::string data) {
    const char* input =  data.c_str();
    TidyBuffer output = {0};
    TidyBuffer errbuf = {0};
  
    //create document
    TidyDoc tdoc = tidyCreate();
    
    //convert to xhtml
    int fin = 0;
    if(tidyOptSetBool(tdoc, TidyXhtmlOut, yes)) {
        int fin = tidySetErrorBuffer(tdoc, &errbuf); //get diagnostic of 
        fin += tidyParseString(tdoc, input);  //parse the input
        fin += tidyCleanAndRepair(tdoc);  //Tidy it up
        fin += tidyRunDiagnostics(tdoc);
    
        //if error force output
        if(fin != 3) {
            fin = (tidyOptSetBool(tdoc, TidyForceOutput, yes) ? fin : -1);
            std::cout << fin;
        }
    }

    fin = tidySaveBuffer(tdoc, &output);
    htmlbuf = (char*)malloc(output.size);
    fin += tidySaveString(tdoc, htmlbuf, &output.size);
    if(fin >= 0) {
        std::cout << errbuf.bp;
    }
    else{ 
        std::cout << "A fatal error has occurred" << fin;
    }
    sizeofxml = output.size; 

    //freeing memory
    tidyBufFree(&output);
    tidyBufFree(&errbuf);
    tidyRelease(tdoc);

}

int Xml::parsexml(Curl *curl){
    tinyxml2::XMLDocument doc;
    doc.Parse(htmlbuf, sizeofxml);
    

    
    //Checking if format is the same as the one in a thread
    char check[10];
    if(!doc.FirstChildElement("html")->FirstChildElement("body")->Attribute("class"))
        return -1;
    if(!std::sscanf(doc.FirstChildElement("html")->FirstChildElement("body")->Attribute("class"), "is_thread%9s", check))
        return -1;
    
    //Location of original post in the html file
    const tinyxml2::XMLElement*  op = doc.FirstChildElement("html")->FirstChildElement("body")->FirstChildElement("form")
        ->NextSiblingElement("form")->FirstChildElement("div")->FirstChildElement("div")->FirstChildElement("div")
        ->FirstChildElement("div")->FirstChildElement("div")->NextSiblingElement("div");

    //Location of the first reply in the html file
    const tinyxml2::XMLElement*  replies= doc.FirstChildElement("html")->FirstChildElement("body")
        ->FirstChildElement("form")->NextSiblingElement("form")->FirstChildElement("div")->FirstChildElement("div")
        ->FirstChildElement("div")->NextSiblingElement("div");

    char url[BUFFER];
    const char *filename;
    //check if post has an image
    if (!strcmp(op->Attribute("class"), "file")){
            op_img = op->FirstChildElement("a")->Attribute("href");
            strcpy(url, "https:\0");
            strcat(url, op_img);


            //small loop to account for boards with different name lenghts (a, diy, etc)
            filename = &op_img[14];
            while(filename[0] != '/'){
                filename++;
            }
            filename++;

            std::ofstream file;
            file.open(filename);


            std::cout << filename;
            curl->buffer.clear();
            curl->curl_setup(url);

            curl_easy_perform(curl->get);

            file << curl->buffer;
            file.close();
            std::cout << " finished downloading\n";
            
    }

    while (replies && !stop){
        //get image div from reply element
        const tinyxml2::XMLElement* reply = replies->FirstChildElement("div")->NextSiblingElement("div")
           ->FirstChildElement("div")->NextSiblingElement()->NextSiblingElement();
        
        //check if post has an image
        if (!strcmp(reply->Attribute("class") , "file" )){
            
            reply_img = reply->FirstChildElement("a")->Attribute("href");
            strcpy(url, "https:\0");
            strcat(url, reply_img);


            //small loop to account for boards with different name lenghts (a, diy, etc)
            filename = &reply_img[14];
            while(filename[0] != '/'){
                filename++;
            }
            filename++;

            std::ofstream file;
            file.open(filename);


            std::cout << filename;
            curl->buffer.clear();
            curl->curl_setup(url);

            curl_easy_perform(curl->get);

            file << curl->buffer;
            file.close();
            std::cout << " finished downloading\n";
        }
       replies = replies->NextSiblingElement("div");
    }
    stop = 0;
    free(htmlbuf);
    htmlbuf = NULL;
    return 0;
}

void Xml::stopCurl(){
    stop = 1;
}
