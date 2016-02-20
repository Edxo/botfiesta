// Written by MrEdxo to auto-join raffles by pajbot and his deriatives
// gachiGASM OMGScoots gachiGASM

#include "ui/mainwindow.h"
#include <QApplication>

#include <QTime>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    w.show();

    return a.exec();
}
