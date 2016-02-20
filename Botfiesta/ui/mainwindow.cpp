#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->oauth_1->setEchoMode( QLineEdit::Password );
    handler = new service();

    QObject::connect( handler, SIGNAL(status_update(bool)), this, SLOT(status_update(bool)) );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_button_start_1_clicked() {
    if( ui->button_start_1->text() == "Connect" ) {
        ui->button_start_1->setText("Disconnect");

        if( ui->channel_1->text() != "" && ui->bot_1->text() != "" )
            handler->addChannel( ui->channel_1->text(), ui->bot_1->text() );
        if( ui->channel_2->text() != "" && ui->bot_2->text() != "" )
            handler->addChannel( ui->channel_2->text(), ui->bot_2->text() );
        if( ui->channel_3->text() != "" && ui->bot_3->text() != "" )
            handler->addChannel( ui->channel_3->text(), ui->bot_3->text() );

        handler->connect( ui->nick_1->text(), ui->oauth_1->text() );
    } else {
        ui->button_start_1->setText("Connect");
        handler->disconnect();
    }
}

void MainWindow::on_button_message_clicked() {
    handler->SendMessage( ui->message->text() );
    ui->message->setText("");
}

void MainWindow::status_update(bool status) {
    if( status == 0 )
        ui->status_1->setText("Not Connected");
    else
        ui->status_1->setText("Connected");
}
