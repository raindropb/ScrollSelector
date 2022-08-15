#include "numberselector.h"
#include "wallpaperselector.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WallpaperSelector w;
//    w.setGeometry(0,0,54,218);
    w.show();
    return a.exec();
}
