#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QDebug>
#include <QObject>

#include <QFile>
#include <QTextStream>

#include <QTimer>

#include "socket/socket.h"

struct channel_meta{
    QString channel;
    QString bot;
};

class service : public QObject {
    Q_OBJECT
public:
    service();
    ~service();

    int connect( QString nick, QString oauth );
    void disconnect();

    void addChannel( QString channel, QString bot );

    void SendMessage( QString message );

private slots:
    void reconnect();
    void JoinRaffle1();
    void JoinRaffle2();
    void JoinRaffle3();

    void ParseString( QString data );
    void ParseIRCCommand( QString command );
    void ParseMessage( QStringList message );
    void Connection();
    void Disconnection();

signals:
    void status_update(bool status);

private:
    bool connected;

    QTimer *timer1, *timer2, *timer3;
    QTimer *distimer;
    QString _nick, _oauth;

    std::vector<channel_meta> _channels;

    // scoket connection
    socket *server;
    QString data;
};

#endif // SERVICE_H
