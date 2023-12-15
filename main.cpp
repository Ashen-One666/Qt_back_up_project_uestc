#include "QtMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtMainWindow w;
    w.show();
    return a.exec();
}
