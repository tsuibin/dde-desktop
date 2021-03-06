/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -N -p createdirjob_interface.h:createdirjob_interface.cpp -c CreateDirJobInterface createdirjob.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef CREATEDIRJOB_INTERFACE_H_1436864303
#define CREATEDIRJOB_INTERFACE_H_1436864303

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.filemanager.Backend.Operations.CreateDirJob
 */
class CreateDirJobInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.filemanager.Backend.Operations.CreateDirJob"; }

public:
    CreateDirJobInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~CreateDirJobInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Execute()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Execute"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void Done(const QString &in0, const QString &in1);
};

#endif
