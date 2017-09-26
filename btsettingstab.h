#ifndef BTSETTINGSTAB_H
#define BTSETTINGSTAB_H
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfo>
#include <QDialogButtonBox>
#include <QDialog>
#include <pthread.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <QMessageBox>
#include <QList>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothsocket.h>
#include <qbluetoothhostinfo.h>
#include <qbluetoothuuid.h>
#include <qbluetoothserver.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothlocaldevice.h>
#include <btpairedserver.h>
#include <btpairedclient.h>

enum flag
{
    EChoseSettings,
    EChoseDevices,
    EAddDevices
};
class BTSettingsTab : public QDialog
{
    Q_OBJECT
    QListWidget *settingsListBox, *deviceListBox;
    QLabel *chooseSettings,*scannedDevices;
    QStringList settings, devices;
    QVBoxLayout *layout1,*layout2;
    QList<QBluetoothHostInfo> localAdapters;
    BTPairedServer *server;
    QString localName;
    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo m_service;
    QMap<QListWidgetItem *, QBluetoothServiceInfo> m_discoveredServices;
    enum flag flagState1;
    int currentAdapterIndex;
    QList<BTPairedClient *> clients;
public:
    explicit BTSettingsTab(const QFileInfo&fileInfo, QWidget*parent = 0);
    void accept();
    void reject();
    void sendClicked();
signals:
    void sendMessage(const QString& message);

private slots:
    void connectClicked();
    void showMessage(const QString & sender, const QString& message);

    void clientConnected(const QString& name);
    void connected(const QString& name);
    void clientDisconnected();
    void clientDisconnected(const QString& name);
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    void on_remoteDevices_itemActivated(QListWidgetItem *item);
    void on_cancelButton_clicked();
    //void on_remoteDevices_itemActivated(QListWidgetItem *item);
};

#endif // BTSETTINGSTAB_H
