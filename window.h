#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QThread>
#include "scraper.h"

class Window:public QMainWindow{
    Q_OBJECT

    public:
        Window();
        ~Window();

    private:
        QLineEdit *url;
        QPushButton *getImages;
        QPushButton *interrupt;

        Scraper *scraper;
        QThread thread;

        QWidget *centralWidget;
        QVBoxLayout *vbox;
        QHBoxLayout *hLineBox;
        QHBoxLayout *hButtonBox1;
        QHBoxLayout *hButtonBox2;
        
    public slots:
        void curlingDone(int a);

    private slots:
        void scrape();
        void interruptCurl();
        
    signals:
        void scrapeImage(char*);
};
