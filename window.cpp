#include <QStatusBar>
#include "window.h"

Window::Window(){
    /*
     * This sets up the graphical front-end
     * Graphic design isn't my passion
     */
    vbox = new QVBoxLayout();
    hLineBox = new QHBoxLayout();
    hButtonBox1 = new QHBoxLayout();
    hButtonBox2 = new QHBoxLayout();
    
    url = new QLineEdit();
    url->setPlaceholderText("Insert URL");

    getImages = new QPushButton("Get images", this);
    interrupt = new QPushButton("Interrupt", this);
    getImages->setFixedSize(230, 23);
    interrupt->setFixedSize(150, 23);
    interrupt->setVisible(0);
    
    hLineBox->addWidget(url);
    hLineBox->insertSpacing(0, 30);
    hLineBox->insertSpacing(-1, 30);

    hButtonBox1->addStretch(1);
    hButtonBox1->addWidget(getImages);
    hButtonBox1->addStretch(1);

    hButtonBox2->addStretch(1);
    hButtonBox2->addWidget(interrupt);
    hButtonBox2->addStretch(1);

    vbox->addStretch(1);
    vbox->addLayout(hLineBox);
    vbox->addSpacing(20);
    vbox->addLayout(hButtonBox1);
    vbox->addLayout(hButtonBox2);
    vbox->addStretch(1);

    
    centralWidget = new QWidget();
    centralWidget->setLayout(vbox);
    setCentralWidget(centralWidget);
    
    /*
     * Create thread to do the actual image scraping
     * Otherwise the GUI will just freeze
     */
    scraper = new Scraper();
    scraper->moveToThread(&thread);
    
    /*
     * Qt signals and slots
     * Connects the front-end to the back-end
     */
    connect(getImages, &QPushButton::clicked, this, &Window::scrape);
    connect(this, &Window::scrapeImage, scraper, &Scraper::curlImage);
    connect(scraper, &Scraper::finished, this, &Window::curlingDone);
    connect(interrupt, &QPushButton::clicked, this, &Window::interruptCurl);
    thread.start();
}

Window::~Window(){
    delete url;
    delete getImages;
    delete interrupt;
    delete hLineBox;
    delete hButtonBox1;
    delete hButtonBox2;
    delete vbox;
    delete centralWidget;
    
    delete scraper;
    thread.quit();
    thread.wait();
}

void Window::scrape(){
    if(url->text().isEmpty()){
        statusBar()->showMessage("Error: No URL was chosen");
    }
    else{
        statusBar()->showMessage("Downloading...");
        /*  
         * three method calls to get to the char*
         * thank you qt
         */
        emit scrapeImage(url->text().toUtf8().data());
        getImages->setDisabled(1);
        interrupt->setVisible(1);
        
    }
    
}

void Window::curlingDone(int a){
    getImages->setDisabled(0);
    interrupt->setVisible(0);
    statusBar()->clearMessage();
    if(a){
        statusBar()->showMessage("Error: Invalid URL");
    }
}

void Window::interruptCurl(){
    scraper->interruptImage();
}
