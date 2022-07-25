#include "widget.h"
#include "scrollselector.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NumberSelector w(0,60);
    w.show();
    return a.exec();
}
