/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "desktopapp.h"
#include "views/global.h"
#include "views/desktopbox.h"
#include "views/desktopitemmanager.h"
#include "views/desktopframe.h"

#include "controllers/appcontroller.h"
#include "dialogs/dtaskdialog.h"
#include "dialogs/confirmdeletedialog.h"
#include "dbusinterface/services/desktopadaptor.h"
#include "dialogs/cleartrashdialog.h"
#include "dialogs/dmovabledialog.h"
#include "dbusinterface/launcher_interface.h"

#include <ddialog.h>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QStandardPaths>
#include <QProcess>

#include "schemawatcher.h"

DWIDGET_USE_NAMESPACE

DesktopApp::DesktopApp(QObject *parent) : QObject(parent)
{
    m_desktopBox = new DesktopBox;
    m_taskDialog = new DTaskDialog;
    m_schemaWatcher = new SchemaWatcher;

    initConnect();
    registerDBusService();
    createLauncerInterface();
}

void DesktopApp::initConnect(){
    connect(signalManager, SIGNAL(gridModeChanged(bool)), this, SLOT(saveGridOn(bool)));
    connect(signalManager, SIGNAL(gridSizeTypeChanged(SizeType)), this, SLOT(saveSizeType(SizeType)));
    connect(signalManager, SIGNAL(sortedModeChanged(QDir::SortFlags)), this, SLOT(saveSortFlag(QDir::SortFlags)));
    connect(signalManager, SIGNAL(copyJobAdded(QMap<QString,QString>)),
            m_taskDialog, SLOT(addCopyMoveTask(QMap<QString,QString>)));
    connect(signalManager, SIGNAL(copyJobRemoved(QMap<QString,QString>)),
            m_taskDialog, SLOT(removeTaskWidget(QMap<QString,QString>)));
    connect(signalManager, SIGNAL(copyJobDataUpdated(QMap<QString,QString>,QMap<QString,QString>)),
            m_taskDialog, SLOT(handleUpdateTaskWidget(QMap<QString,QString>,QMap<QString,QString>)));
    connect(m_taskDialog, SIGNAL(abortCopyTask(QMap<QString,QString>)),
            signalManager, SIGNAL(abortCopyTask(QMap<QString,QString>)));

    connect(signalManager, SIGNAL(moveJobAdded(QMap<QString,QString>)),
            m_taskDialog, SLOT(addCopyMoveTask(QMap<QString,QString>)));
    connect(signalManager, SIGNAL(moveJobRemoved(QMap<QString,QString>)),
            m_taskDialog, SLOT(removeTaskWidget(QMap<QString,QString>)));
    connect(signalManager, SIGNAL(moveJobDataUpdated(QMap<QString,QString>,QMap<QString,QString>)),
            m_taskDialog, SLOT(handleUpdateTaskWidget(QMap<QString,QString>,QMap<QString,QString>)));
    connect(m_taskDialog, SIGNAL(abortMoveTask(QMap<QString,QString>)),
            signalManager, SIGNAL(abortMoveTask(QMap<QString,QString>)));

    connect(signalManager, SIGNAL(deleteFilesExcuted(QMap<QString,QString>)),
            this, SLOT(confimDelete(QMap<QString,QString>)));
    connect(signalManager, SIGNAL(deleteJobAdded(QMap<QString,QString>)),
            m_taskDialog, SLOT(addCopyMoveTask(QMap<QString,QString>)));
    connect(signalManager, SIGNAL(deleteJobRemoved(QMap<QString,QString>)),
            m_taskDialog, SLOT(removeTaskWidget(QMap<QString,QString>)));
    connect(signalManager, SIGNAL(deleteJobDataUpdated(QMap<QString,QString>,QMap<QString,QString>)),
            m_taskDialog, SLOT(handleUpdateTaskWidget(QMap<QString,QString>,QMap<QString,QString>)));
    connect(m_taskDialog, SIGNAL(abortDeleteTask(QMap<QString,QString>)),
            signalManager, SIGNAL(abortDeleteTask(QMap<QString,QString>)));

    connect(m_taskDialog, SIGNAL(conflictRepsonseConfirmed(QMap<QString,QString>,QMap<QString,QVariant>)),
            this, SLOT(confimConflict(QMap<QString,QString>,QMap<QString,QVariant>)));

    connect(m_taskDialog, SIGNAL(conflictShowed(QMap<QString,QString>)),
            signalManager, SIGNAL(conflictTimerStoped(QMap<QString,QString>)));
    connect(m_taskDialog, SIGNAL(conflictHided(QMap<QString,QString>)),
            signalManager, SIGNAL(conflictTimerReStarted(QMap<QString,QString>)));

    connect(signalManager, SIGNAL(conflictDialogShowed(QMap<QString,QString>)),
            m_taskDialog, SLOT(showConflictDiloagByJob(QMap<QString,QString>)));


    connect(qApp, SIGNAL(aboutToQuit()), this, SIGNAL(closed()));
    connect(signalManager, SIGNAL(desktopFrameRectChanged(QRect)), m_taskDialog, SLOT(moveTopRightByRect(QRect)));

    connect(signalManager, SIGNAL(confimClear(int)), this, SLOT(confimClear(int)));
    connect(signalManager, SIGNAL(renameDialogShowed(QString)), this, SLOT(confirmRenameDialog(QString)));
    connect(signalManager, SIGNAL(f1Preesed()), this, SLOT(handleF1Pressed()));
}

void DesktopApp::confimClear(int count){
    Q_UNUSED(count)
    ClearTrashDialog d(m_desktopBox->getDesktopFrame());
    connect(signalManager, SIGNAL(desktopFrameRectChanged(QRect)), &d, SLOT(moveCenterByRect(QRect)));
    connect(&d, SIGNAL(buttonClicked(int)), signalManager, SIGNAL(actionHandled(int)));
    d.exec();
}

void DesktopApp::confimDelete(const QMap<QString, QString> &files){
    m_deletefiles.clear();
    m_deletefiles = files.keys();
    m_deletefiles.removeOne(ComputerUrl);
    m_deletefiles.removeOne(TrashUrl);

    emit signalManager->unCheckedItem(ComputerUrl);
    emit signalManager->unCheckedItem(TrashUrl);

    if (m_deletefiles.length() == 0){
        return;
    }

    QStringList displayNames;
    foreach (QString url, m_deletefiles) {
        displayNames.append(files.value(url));
    }

    ConfirmDeleteDialog d(m_desktopBox->getDesktopFrame());
    connect(signalManager, SIGNAL(desktopFrameRectChanged(QRect)), &d, SLOT(moveCenterByRect(QRect)));
    QString message;
    if (displayNames.length() == 1){
        QString name = displayNames.at(0);
        message = tr("Are you sure to delete %1 ?").arg(name);
    }else{
        message = tr("Are you sure to delete these %1 items?").arg(QString::number(displayNames.length()));
    }
    d.setMessage(message);
    connect(&d, SIGNAL(buttonClicked(int)), this, SLOT(handleDeleteAction(int)));
    int code = d.exec();
    qDebug() << code << "dialog close";

    emit signalManager->shiftReleased(false);

}

void DesktopApp::handleDeleteAction(int index){
    switch (index) {
    case 0:
        return;
        break;
    case 1:
        emit signalManager->filesDeleted(m_deletefiles);
        break;
    default:
        break;
    }
}

void DesktopApp::confimConflict(const QMap<QString, QString> &jobDetail, const QMap<QString, QVariant> &response){
    qDebug() << jobDetail << response;
    emit signalManager->conflictRepsonseConfirmed(jobDetail, response);
}

void DesktopApp::confirmRenameDialog(QString name){
    if (name.startsWith(RichDirPrefix)){
        name = name.mid(QString(RichDirPrefix).length());
    }
    qDebug() << "confirmRenameDialog" << name;
    DDialog* d = new DDialog(m_desktopBox);
    d->setModal(false);
    d->setTitle(tr("\"%1\" already exists, please select a different name.").arg(name));
    d->addButton(tr("Confirm"), true);
    d->setIconPixmap((QPixmap(":/images/skin/dialogs/images/dialog-warning.svg")));
    d->show();
}

void DesktopApp::registerDBusService(){
    new DesktopAdaptor(this);
    QDBusConnection conn = QDBusConnection::sessionBus();
    conn.registerObject(DesktopAdaptor::staticInterfacePath(), this);
}

void DesktopApp::createLauncerInterface()
{
    m_launcherInterface = new LauncherInterface(this);
    connect(m_launcherInterface, SIGNAL(UninstallSuccess(QString)), this, SLOT(handleAppUninstalled(QString)));
}

void DesktopApp::handleAppUninstalled(const QString &appKey){
    qDebug() << "handleAppUninstalled" << appKey;
    removeInvalidDesktopFile(desktopLocation, appKey);
    QDir desktopDir(desktopLocation);
    QFileInfoList desktopInfoList = desktopDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name);
    int size = desktopInfoList.size();
    for (int i = 0; i < size; i++) {
        QFileInfo fileInfo = desktopInfoList.at(i);
        if (isAppGroup(fileInfo.filePath())){
            qDebug() << fileInfo.filePath() << fileInfo.fileName();
            removeInvalidDesktopFile(fileInfo.filePath(), appKey);
        }
    }

}

void DesktopApp::removeInvalidDesktopFile(const QString &path, const QString &appKey)
{
    QDir desktopDir(path);
    QFileInfoList desktopInfoList = desktopDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    int size = desktopInfoList.size();
    for (int i = 0; i < size; i++) {
        QFileInfo fileInfo = desktopInfoList.at(i);
        qDebug() << fileInfo.filePath() << fileInfo.fileName();
        if (fileInfo.fileName().endsWith(".desktop")){
            QSettings desktopFileSettings(fileInfo.filePath(), QSettings::IniFormat);
            desktopFileSettings.beginGroup("Desktop Entry");
            qDebug() << desktopFileSettings.contains("X-Deepin-AppID");
            if (desktopFileSettings.contains("X-Deepin-AppID")){
                if (desktopFileSettings.value("X-Deepin-AppID") == appKey){
                    bool removeFileFlag = QFile(fileInfo.filePath()).remove();
                    qDebug() << "Remove" << fileInfo.filePath() << removeFileFlag;
                }
            }
            desktopFileSettings.endGroup();
        }
    }
}

void DesktopApp::saveGridOn(bool mode){
    QSettings settings;
    settings.beginGroup("Desktop");
    settings.setValue("isGridOn", mode);
    settings.endGroup();
}

void DesktopApp::saveSizeType(SizeType type){
    QSettings settings;
    settings.beginGroup("Desktop");
    settings.setValue("sizeType", type);
    settings.endGroup();
}

void DesktopApp::saveSortFlag(QDir::SortFlags flag){
    QSettings settings;
    settings.beginGroup("Desktop");
    settings.setValue("sortFlag", static_cast<int>(flag));
    settings.endGroup();
}

void DesktopApp::show(){
    m_desktopBox->show();
    emit shown();
}

void DesktopApp::hide(){
    m_desktopBox->hide();
}

void DesktopApp::toggle(){
    if (m_desktopBox->isVisible()){
        hide();
    }else{
        show();
    }
}

void DesktopApp::exit(){
    qApp->quit();
}

void DesktopApp::handleF1Pressed()
{
    QProcess::startDetached("dman dde");
}

DesktopApp::~DesktopApp()
{

}

