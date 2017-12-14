#include "../include/cargui/Cargui.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cargui w;
    w.show();

    return a.exec();
}
