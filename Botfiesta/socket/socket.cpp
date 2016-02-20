#include "socket/socket.h"

socket::socket(QObject *parent) : QObject(parent), _server( new QTcpSocket(this) ) {
    _port = 0;
}

int socket::start( QHostAddress ip, quint16 port ) {
    _ip = ip; _port = port;

    _server->connectToHost( _ip, _port );

    if( !_server->waitForConnected(1000) ) {
        qDebug() << "Unable to connect to irc.twitch.tv";
        qDebug() << "Error: " << _server->errorString();
        return 1;
    }

    QObject::connect(_server, SIGNAL(readyRead()), this, SLOT(onRead()));
    QObject::connect(_server, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    emit Connected();
    return 0;
}

void socket::send( QByteArray data ) {
    //qDebug() << "Sending:" << data;
    _server->write( data );
    _server->waitForBytesWritten(1000);
}

void socket::send( QString data ) {
    //qDebug() << "Sending:" << data;
    _server->write( data.toUtf8().constData() );
    _server->waitForBytesWritten(1000);
}

void socket::stop() {
    _server->close();
    _server = new QTcpSocket(this);
    emit Disconnected();
}

void socket::onRead() {
    QByteArray data = _server->readAll();
    emit NewData(data);
}

void socket::onDisconnected() {
    _server->close();
    _server = new QTcpSocket(this);
    emit Disconnected();
}
