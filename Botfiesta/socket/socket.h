#ifndef RELAY_H
#define RELAY_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QAbstractSocket>

#include <QObject>

class socket : public QObject {
    Q_OBJECT
public:
    explicit socket(QObject *parent = 0);

    int start( QHostAddress ip, quint16 port );
    void stop();
    void send( const QByteArray data );
    void send( const QString data );

signals:
    void NewData(QString data);
    void Disconnected();
    void Connected();

private slots:
    void onRead();
    void onDisconnected();

private:
    QTcpSocket *_server;

    QHostAddress _ip;
    quint16 _port;
};

#endif // RELAY_H
