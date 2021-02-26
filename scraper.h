#pragma once
#include <QObject>
#include "xml.h"
#include "curl.h"

class Scraper:public QObject{
    Q_OBJECT

    public:
        Scraper();
        ~Scraper();
        void interruptImage();

    private:
        Curl *curl;
        Xml *xml;
    
    public slots:
        void curlImage(char *URL);
        
    signals:
        void finished(int a);
};
