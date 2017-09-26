#ifndef BTPAIREDCLIENT_H
#define BTPAIREDCLIENT_H
#include <qbluetoothserviceinfo.h>
//#include "btsettingstab.h"
#include <QtCore/QObject>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class BTPairedClient : public QObject
{
    Q_OBJECT

public:
    explicit BTPairedClient(QBluetoothServiceInfo serviceInfo,QObject* settingTab = 0);
    ~BTPairedClient();

    void startClient(const QBluetoothServiceInfo &remoteService);
    void stopClient();

public slots:
    void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &sender, const QString &message);
    void connected(const QString &name);
    void disconnected();

private slots:
    void readSocket();
    void connected();

private:
    QBluetoothSocket *socket;
     QBluetoothServiceInfo m_service;
     QObject* m_settingTab;
};
#endif // BTPAIREDCLIENT_H
