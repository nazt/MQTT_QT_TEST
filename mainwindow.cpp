#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "qmqtt.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _client = new QMQTT::Client("iot.eclipse.org", 1883);

    _client->setClientId("xxo");

//    client->setUsername("user");

//    client->setPassword("password");

    connect(_client, SIGNAL(connected()), this, SLOT(onMQTT_Connected()));
    connect(_client, SIGNAL(connacked(quint8)), this, SLOT(onMQTT_Connacked(quint8)));
    connect(_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onMQTT_error(QAbstractSocket::SocketError)));
    connect(_client, SIGNAL(published(QMQTT::Message &)), this, SLOT(onMQTT_Published(QMQTT::Message &)));
    connect(_client, SIGNAL(pubacked(quint8, quint16)), this, SLOT(onMQTT_Pubacked(quint8, quint16)));
    connect(_client, SIGNAL(received(const QMQTT::Message &)), this, SLOT(onMQTT_Received(const QMQTT::Message &)));
    connect(_client, SIGNAL(subscribed(const QString &)), this, SLOT(onMQTT_subscribed(const QString &)));
    connect(_client, SIGNAL(subacked(quint16, quint8)), this, SLOT(onMQTT_subacked(quint16, quint8)));
    connect(_client, SIGNAL(unsubscribed(const QString &)), this, SLOT(onMQTT_unsubscribed(const QString &)));
    connect(_client, SIGNAL(unsubacked(quint16)), this, SLOT(onMQTT_unsubacked(quint16)));
    connect(_client, SIGNAL(pong()), this, SLOT(onMQTT_Pong()));
    connect(_client, SIGNAL(disconnected()), this, SLOT(onMQTT_disconnected()));

    _client->connect();

    ui->setupUi(this);
    ui->textBrowser->setText("HELLO");
}

void MainWindow::log(const QString & msg)
{
   qDebug() << msg << endl;
}

/* -----------------------------------------------------------
 MQTT Client Slots
 -----------------------------------------------------------*/
void MainWindow::onMQTT_Connected()
{
    log(tr("connected to %1:%2").arg(_client->host()).arg(_client->port()));
    _client->subscribe("cmmc/Dragon", 0);
}

void MainWindow::onMQTT_Connacked(quint8 ack)
{
    QString ackStatus;
    switch(ack) {
    case QMQTT::CONNACK_ACCEPT:
        ackStatus = "Connection Accepted";
        break;
    case QMQTT::CONNACK_PROTO_VER:
        ackStatus = "Connection Refused: unacceptable protocol version";
        break;
    case QMQTT::CONNACK_INVALID_ID:
        ackStatus = "Connection Refused: identifier rejected";
        break;
    case QMQTT::CONNACK_SERVER:
        ackStatus = "Connection Refused: server unavailable";
        break;
    case QMQTT::CONNACK_CREDENTIALS:
        ackStatus = "Connection Refused: bad user name or password";
        break;
    case QMQTT::CONNACK_AUTH:
        ackStatus = "Connection Refused: not authorized";
        break;
    }
    log(tr("connacked: %1, %2").arg(ack).arg(ackStatus));
}
void MainWindow::onMQTT_error(QAbstractSocket::SocketError err)
{
    QString errInfo;
    switch(err) {
    // 0	The connection was refused by the peer (or timed out).
    case QAbstractSocket::ConnectionRefusedError:
        errInfo = tr("Connection Refused");
    //	 1	The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.
    case QAbstractSocket::RemoteHostClosedError:
        errInfo = tr("Remote Host Closed");
    //	2	The host address was not found.
    case QAbstractSocket::HostNotFoundError:
        errInfo = tr("Host Not Found Error");
    // 	3	The socket operation failed because the application lacked the required privileges.
    case QAbstractSocket::SocketAccessError:
        errInfo = tr("Socket Access Error");
    // 	4	The local system ran out of resources (e.g., too many sockets).
    case QAbstractSocket::SocketResourceError:
        errInfo = tr("Socket Resource Error");
    // 	5	The socket operation timed out.
    case QAbstractSocket::SocketTimeoutError:
        errInfo = tr("Socket Timeout Error");
    default:
        errInfo = tr("Socket Error");
    }
    log(errInfo);
}

void MainWindow::onMQTT_Published(QMQTT::Message &message)
{
    log(tr("message published to %1").arg(message.topic()));
    log(message.payload());
}

void MainWindow::onMQTT_Pubacked(quint8 type, quint16 msgid)
{
    log(tr("pubacked: type=%1, msgid=%2").arg(type, msgid));
}

void MainWindow::onMQTT_Received(const QMQTT::Message &message)
{
    log(tr("message recevied from %1: qos=%2").arg(message.topic()).arg(message.qos()));
    log(message.payload());
    ui->textBrowser->setText(message.payload());
}

void MainWindow::onMQTT_subscribed(const QString &topic)
{
    log(tr("subscribed %1").arg(topic));
}

void MainWindow::onMQTT_subacked(quint16 msgid, quint8 qos)
{
    log(tr("subacked: msgid=%1, qos=%2").arg(msgid).arg(qos));
}

void MainWindow::onMQTT_unsubscribed(const QString &topic)
{
    log(tr("unsubscribed %1").arg(topic));
}

void MainWindow::onMQTT_unsubacked(quint16 msgid)
{
    log(tr("unsubacked: msgid=%1").arg(msgid));
}

void MainWindow::onMQTT_Pong()
{
    log("pong received.");
}

void MainWindow::onMQTT_disconnected()
{
    log("disconnected");
}



MainWindow::~MainWindow()
{
    delete ui;
}
