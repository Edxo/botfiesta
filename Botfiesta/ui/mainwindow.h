#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "service/service.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_button_start_1_clicked();
    void on_button_message_clicked();

    void status_update(bool);

private:
    service *handler;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
