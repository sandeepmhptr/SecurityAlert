
#include "btpairedclient.h"
#include <qbluetoothsocket.h>
#include "btsettingstab.h"
#include <QMessageBox>
#include <QTimer>

BTPairedClient::BTPairedClient(QBluetoothServiceInfo serviceInfo,QObject* settingTab)
:   QObject(settingTab), socket(0),m_service(serviceInfo),m_settingTab(settingTab)
{
}

BTPairedClient::~BTPairedClient()
{
    stopClient();
}

void BTPairedClient::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}

void BTPairedClient::stopClient()
{
    delete socket;
    socket = 0;
}

void BTPairedClient::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
        QMessageBox::information(0,socket->peerName(),QString::fromUtf8(line.constData(), line.length()),QMessageBox::Ok);
    }
}

void BTPairedClient::sendMessage(const QString &message)
{
    QMessageBox::information(0,QString("accepted connection from"),QString("BTPairedClient::sendMessage"),QMessageBox::Ok);
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
}

void BTPairedClient::connected()
{
    emit connected(socket->peerName());
    /*QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),(BTSettingsTab *)m_settingTab, SLOT(sendClicked()));
    timer->start(60000);*/
    ((BTSettingsTab*)m_settingTab)->sendClicked();
}
