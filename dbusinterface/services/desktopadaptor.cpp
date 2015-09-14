/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -N -c DesktopAdaptor -a desktopadaptor dde-desktop.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "desktopadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class DesktopAdaptor
 */

DesktopAdaptor::DesktopAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    ConflictInfo::registerMetaType();
    setAutoRelaySignals(true);
}

DesktopAdaptor::~DesktopAdaptor()
{
    // destructor
}

ConflictInfo DesktopAdaptor::AskSkip(const QString &in0, const QString &in1, const QString &in2, int in3)
{
    // handle method call com.deepin.dde.Desktop.AskSkip
    ConflictInfo out0;
    QMetaObject::invokeMethod(parent(), "AskSkip", Q_RETURN_ARG(ConflictInfo, out0), Q_ARG(QString, in0), Q_ARG(QString, in1), Q_ARG(QString, in2), Q_ARG(int, in3));
    return out0;
}

ConflictInfo DesktopAdaptor::ConflictDialog()
{
    // handle method call com.deepin.dde.Desktop.ConflictDialog
    ConflictInfo out0;
    QMetaObject::invokeMethod(parent(), "ConflictDialog", Q_RETURN_ARG(ConflictInfo, out0));
    return out0;
}
