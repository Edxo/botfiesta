#include "service/service.h"

service::service( ) {
    timer1 = new QTimer(this);
    timer1->setSingleShot(true);
    timer2 = new QTimer(this);
    timer2->setSingleShot(true);
    timer3 = new QTimer(this);
    timer3->setSingleShot(true);
    distimer = new QTimer(this);
    server = new socket;

    connected = false;

    QObject::connect( timer1, SIGNAL(timeout()), this, SLOT(JoinRaffle1()) );
    QObject::connect( timer2, SIGNAL(timeout()), this, SLOT(JoinRaffle2()) );
    QObject::connect( timer3, SIGNAL(timeout()), this, SLOT(JoinRaffle3()) );
    QObject::connect( distimer, SIGNAL(timeout()), this, SLOT(reconnect()) );
    QObject::connect( server, SIGNAL(NewData(QString)), this, SLOT(ParseString(QString)) );
    QObject::connect( server, SIGNAL(Disconnected()), this, SLOT(Disconnection()) );
    QObject::connect( server, SIGNAL(Connected()), this, SLOT(Connection()) );
}

service::~service() {
    delete timer1;
    delete timer2;
    delete timer3;
    delete distimer;
    delete server;
}

void service::addChannel( QString channel, QString bot ) {
    channel_meta newChannel;
    newChannel.channel = channel;
    newChannel.bot = bot;
    _channels.push_back( newChannel );
}

int service::connect( QString nick, QString oauth ) {
    _nick = nick;
    _oauth = oauth;
    connected = true;

    int returnValue = server->start(QHostAddress("192.16.64.208"), 6667);   // Hardcoded twitches IP address, professional coding Elegiggle
    if( returnValue != 0 )
       return returnValue;

    QString command = "PASS oauth:" + _oauth + "\r\nNICK " + _nick + "\r\n";

    for( unsigned int x = 0; x < _channels.size(); x++ ) {
       qDebug() << "Joining channel " << x << " name:" << _channels[x].channel;
       command = command + "JOIN #" + _channels[x].channel + "\r\n";
    }

    server->send( command );

    return returnValue;
};

void service::SendMessage( QString message ) {
    for( unsigned int x = 0; x < _channels.size(); x++ ) {
        server->send("PRIVMSG #" + _channels[x].channel + " :" + message + "\r\n");
    }
}

void service::ParseString(QString data) {
    QStringList split = data.split("\r\n");
    for( int x = 0; x < split.size(); x++ ) {
        if( split[x] == "" )
            split.removeAt(x);
    }

    for( int x = 0; x < split.size(); x++ ) {
        ParseIRCCommand( split[x] );
    }

}

void service::ParseIRCCommand( QString command ) {
    QStringList section = command.split(" ");
    if( section[0] == "PING" ) {
        qDebug() << "Received PING, sending PONG";
        server->send( QString("PONG :tmi.twitch.tv\r\n") );
    } else {
        ParseMessage( section );
    }
}

void service::ParseMessage( QStringList message ) {
    if( message[1] == "PRIVMSG" ) {
        message[0].remove( 0, 1 );
        QString msg = "";
        QString user = QStringList( message[0].split("!") )[0];
        message[3].remove( 0, 1 );
        for( int x = 3; x < message.size(); x++ ) {
            msg = msg + message[x];
            if( x + 1 != message.size() )
                msg = msg + " ";
        }

        //qDebug() << "Channel:" << message[2] << " User:" << user << " Message:" << msg;

        int BotMessage = -1;
        for( unsigned int x = 0; x < _channels.size(); x++ ) {
            if( user == _channels[x].bot ) {
                BotMessage = x;
            }
        }
        if( BotMessage >= 0 ) {
            if( msg.contains("raffle has begun" ) ) {      // is it a raffle?
                qDebug() << "Channel:" << message[2] << " User:" << user << " Message:" << msg;
                bool positive = false;
                for( int x = 1; x < message.size(); x++ ) {        // Is it positive or zero?
                    if( message[x].contains("points") ) {
                        bool isNumber;
                        int number;
                        number = message[x-1].toInt( &isNumber, 10 );
                        if(!isNumber) {
                            qDebug() << "Unable to join raffle, invalid point quantity: " << message[x-1];
                            return;
                        }
                        if( number >= 0 ) {
                            positive = true;
                        }
                    }
                }
                if( !positive ) {
                    qDebug() << "Not joining raffle, negative point quantity";
                    return;
                }

                int timeRaffle;
                for( int x = 1; x < message.size(); x++ ) {        // what is the timeout
                    if( message[x].contains("seconds") ) {
                        bool isNumber;
                        timeRaffle = message[x-1].toInt( &isNumber, 10 );
                        if(!isNumber) {
                            qDebug() << "Unable to join raffle, invalid raffle timeout: " << message[x-1];
                            return;
                        }
                        qDebug() << "Time to raffle end: " << timeRaffle;
                        if( timeRaffle < 3 ) {
                            timeRaffle = 0;
                        } else {
                            timeRaffle = qrand() % (timeRaffle - 2);
                            qDebug() << "Time until join: " << timeRaffle;
                        }
                    }
                }

                switch(BotMessage) {
                case 0:
                    timer1->start(timeRaffle * 1000);
                    break;
                case 1:
                    timer2->start(timeRaffle * 1000);
                    break;
                case 2:
                    timer3->start(timeRaffle * 1000);
                    break;
                default:
                    qDebug() << "Invalid bot id requested to start a raffle join countdown, id:" << BotMessage;
                }

            }
        }

    } else {
        qDebug() << "Unknown message: " << message;
    }
}

void service::reconnect() {
    if( !connect( _nick, _oauth ) )
        distimer->stop();
}

void service::JoinRaffle1() {
    qDebug() << "Joining raffle on channel" << _channels[0].channel << "!";
    server->send( QString("PRIVMSG #" + _channels[0].channel + " :!join\r\n") );
}

void service::JoinRaffle2() {
    qDebug() << "Joining raffle on channel" << _channels[1].channel << "!";
    server->send( QString("PRIVMSG #" + _channels[1].channel + " :!join\r\n") );
}

void service::JoinRaffle3() {
    qDebug() << "Joining raffle on channel" << _channels[2].channel << "!";
    server->send( QString("PRIVMSG #" + _channels[2].channel + " :!join\r\n") );
}

void service::Connection() {
    qDebug() << "Parser: Connection";
    emit status_update(true);
}

void service::Disconnection() {
    if( connected )
        distimer->start(1000);
    emit status_update(false);
    qDebug() << "Parser: Disconnection";
}

void service::disconnect() {
    connected = false;
    timer1->stop();
    timer2->stop();
    timer3->stop();
    server->stop();
    _channels.clear();
}
