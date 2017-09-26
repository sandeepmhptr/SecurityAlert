#include "tabdialog.h"

TabDialog::TabDialog(const QString&fileName, QWidget*parent)
    :QDialog(parent)
{
    QFileInfo fileInfo(fileName);
    QVBoxLayout * mainLayout = new QVBoxLayout;
    tabWidget = new QTabWidget;
    tabWidget->addTab(new BTSettingsTab(fileInfo),tr("BTSettings"));
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
    setWindowTitle(tr("Tab Dialog"));
}
