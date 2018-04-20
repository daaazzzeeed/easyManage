#include "brtrcontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMenuBar menuBar;
    brtrControl w;
    w.setWindowTitle("СУБД команд МПИ управления БРТР");
    w.show();

    return a.exec();
}
