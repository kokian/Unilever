#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap("/Users/Kirill/Works/Unilever/ch1_bg.png");
    QSplashScreen splash(pixmap);
    splash.show();
    QTime dieTime= QTime::currentTime().addSecs(1);
    while( QTime::currentTime() < dieTime )
        a.processEvents(QEventLoop::AllEvents, 100);
    splash.hide();
    MainWindow w;
    w.show();

    return a.exec();
}
