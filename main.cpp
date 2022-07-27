#include "numberselector.h"
#include "wallpaperselector.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WallpaperSelector w;

    w.show();
    return a.exec();
}
