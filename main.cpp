#include <QApplication>
#include "window.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    
    Window window;
    
    window.setWindowTitle("4chan Image Scraper");
    window.resize(380, 140);
    window.show();
    
    return app.exec();
}
