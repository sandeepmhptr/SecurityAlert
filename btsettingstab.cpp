#include "btsettingstab.h"
//#include <QDialogButtonBox>
//#include <QtCore/qmath.h>
#include<math.h>

static long int  mobile_num(10), imei(10) ;
static char passwd[16] = "abc";
pthread_t threadId;
/**/
static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");




void BTSettingsTab::connectClicked()
{

}

void BTSettingsTab::showMessage(const QString & sender, const QString& message)
{
QMessageBox::information(0,QString::fromLatin1("%1:\n").arg(sender),QString::fromLatin1("%1:\n").arg(message),QMessageBox::Ok);
}

void BTSettingsTab::sendClicked()
{
    showMessage(localName, "ui->sendText->text()");
        emit sendMessage("ui->sendText->text()");
}

void BTSettingsTab::clientConnected(const QString& name)
{
    QMessageBox::information(0,QString("accepted connection from"),QString(name),QMessageBox::Ok);
}

void BTSettingsTab::connected(const QString& name)
{

}

void BTSettingsTab::clientDisconnected()
{

}

void BTSettingsTab::clientDisconnected(const QString& name)
{
    BTPairedClient *client = qobject_cast<BTPairedClient *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}

void BTSettingsTab::discoveryFinished()
{

}

/*void BTSettingsTab::on_remoteDevices_itemActivated(QListWidgetItem *item)
{

}*/

void BTSettingsTab::on_cancelButton_clicked()
{

}


void BTSettingsTab::reject()
{

}
void BTSettingsTab::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
#if 0
    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();
#endif

    m_service = serviceInfo;
    QMapIterator<QListWidgetItem *, QBluetoothServiceInfo> i(m_discoveredServices);
    while (i.hasNext()){
        i.next();
        if (serviceInfo.device().address() == i.value().device().address()){
            return;
        }
    }
    int rssi = serviceInfo.device().rssi();
    int txPower = -59;
    double distance;
    double ratio;
    if(rssi == 0)
        distance = -1.0;
    else
    {
        ratio = rssi * 1.0 /txPower;
        if(ratio < 1.0)
            distance = pow(ratio,10);
        else
            distance = (0.89976)*pow(ratio,7.7095) + 0.111;
    }
    QBluetoothUuid list = serviceInfo.serviceUuid();
    devices.append(QString::number(list.TxPower));
    devices.append(QString::number(distance));
    /*QList<QBluetoothUuid>::iterator it = list.begin();
    for(;it!=list.end();it++)
    {
        devices.append(QString::number(it->CyclingPowerMeasurement));
    }*/
    QString remoteName;
    if (serviceInfo.device().name().isEmpty())
        remoteName = serviceInfo.device().address().toString();
    else
        remoteName = serviceInfo.device().name();

    QListWidgetItem *item =
        new QListWidgetItem(QString::fromLatin1("%1 %2").arg(remoteName,
                                                             serviceInfo.serviceName()));

    QString item2= QString::fromLatin1("%1 %2 %3").arg(remoteName,serviceInfo.serviceName(),QString::number(rssi));
    //sprintf(item2,"%s,%s",remoteName,serviceInfo.serviceName());
    //cout<<item2<<endl;
    m_discoveredServices.insert(item, serviceInfo);
    //ui->remoteDevices->addItem(item);
    devices.append(item2);
    //on_remoteDevices_itemActivated(item);
     flagState1 =  EAddDevices;
     QBluetoothServiceInfo service = m_service;

                     qDebug() << "Connecting to service 2" << service.serviceName()
                              << "on" << service.device().name();

                     // Create client
                     qDebug() << "Going to create client";
                     BTPairedClient *client = new BTPairedClient(m_service,this);
             qDebug() << "Connecting...";

                     connect(client, SIGNAL(messageReceived(QString,QString)),
                             this, SLOT(showMessage(QString,QString)));
                     connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
                     connect(client, SIGNAL(connected(QString)), this, SLOT(connected(QString)));
                     connect(this, SIGNAL(sendMessage(QString)), client, SLOT(sendMessage(QString)));
             qDebug() << "Start client";
                     client->startClient(service);

                     clients.append(client);
                     //::accept();
     emit accept();

}


void BTSettingsTab::on_remoteDevices_itemActivated(QListWidgetItem *item)
{
    qDebug() << "got click" << item->text();
    m_service = m_discoveredServices.value(item);
    if (m_discoveryAgent->isActive())
        m_discoveryAgent->stop();

    //accept();
}



void BTSettingsTab::accept()
{
    printf("Hello World\n");
    if(flagState1 == EChoseSettings)
    {
        QString curr = settingsListBox->currentItem()->text();
        if(!strcmp(curr.toLatin1().data(),"register to locket"))
        {
            //flag = true;
            //scannedDevices = new QLabel(tr("Scanned Devices :"));
            //newConfiguration(&devices,&bluetoothNamesAndAddresses, &numBTDevDiscovered);
            localAdapters = QBluetoothLocalDevice::allDevices();
            //char a[] = "back";
            QList<QBluetoothHostInfo>::iterator i;
            /*or (i = localAdapters.begin(); i != localAdapters.end(); ++i)
                //cout << *i << endl;
                devices.append(i->address().toString());*/
            //QBluetoothLocalDevice adapter(localAdapters.at(0).address());
            //(check)adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
            server = new BTPairedServer(this);
                connect(server, SIGNAL(clientConnected(QString)), this, SLOT(clientConnected(QString)));
                connect(server, SIGNAL(clientDisconnected(QString)), this, SLOT(clientDisconnected(QString)));
                connect(server, SIGNAL(messageReceived(QString,QString)),
                        this, SLOT(showMessage(QString,QString)));
                connect(this, SIGNAL(sendMessage(QString)), server, SLOT(sendMessage(QString)));
                server->startServer();

                localName = QBluetoothLocalDevice().name();
                const QBluetoothAddress adapter2 = localAdapters.isEmpty() ?
                                                       QBluetoothAddress() :
                                                       localAdapters.at(currentAdapterIndex).address();

                //QBluetoothAddress adapter2 = localAdapters.at(0).address();
                m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapter2);

                connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                        this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
                connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
                connect(m_discoveryAgent, SIGNAL(canceled()), this, SLOT(discoveryFinished()));
                if (m_discoveryAgent->isActive())
                    m_discoveryAgent->stop();

                //ui->remoteDevices->clear();

                m_discoveryAgent->setUuidFilter(QBluetoothUuid(serviceUuid));
                m_discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
            //devices.append(a);

        }
        else
        {
            deviceListBox->insertItems(0,devices);
        }
    }
    else if(flagState1==EChoseDevices)
    {
        QString curr = deviceListBox->currentItem()->text();
        if(!strcmp(curr.toLatin1().data(),"back"))
        {
            flagState1 = EChoseSettings;
            //free(bluetoothNamesAndAddresses);
        }
        else
        {
            int choice = deviceListBox->currentRow() ;
            //int choiceArray[numBTDevDiscovered]= {0};
            char destinationAddress[18];


        }
    }
    else if(flagState1 ==  EAddDevices)
    {
        scannedDevices = new QLabel(tr("Scanned Devices :"));
        char a[] = "back";
        devices.append(a);
        deviceListBox = new QListWidget;
        deviceListBox->insertItems(0,devices);
        layout2 = new QVBoxLayout;
        layout2->addWidget(scannedDevices);
        layout2->addWidget(deviceListBox);
         QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                             | QDialogButtonBox::Cancel);

        connect(buttonBox, &QDialogButtonBox::accepted, this, &BTSettingsTab::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &BTSettingsTab::reject);
        layout2->addWidget(buttonBox);

        delete(chooseSettings);
        delete(layout1);
        setLayout(layout2);
        flagState1 = EChoseDevices;
    }

}



BTSettingsTab::BTSettingsTab(const QFileInfo & fileInfo, QWidget*parent)
    :QDialog(parent),currentAdapterIndex(0)
{
    chooseSettings = new QLabel(tr("Choose Settings"));
    settingsListBox = new QListWidget;

    settings.append(tr("register to locket"));
    settingsListBox->insertItems(0,settings);
    layout1 = new QVBoxLayout;
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted,this,&BTSettingsTab::accept);
    connect(buttonBox,&QDialogButtonBox::rejected,this,&BTSettingsTab::reject);
    layout1->addWidget(chooseSettings);
    layout1->addWidget(settingsListBox);
    layout1->addWidget(buttonBox);
    setLayout(layout1);
}
