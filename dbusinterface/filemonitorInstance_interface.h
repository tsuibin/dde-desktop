/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -N -p filemonitorInstance_interface.h:filemonitorInstance_interface.cpp -c FileMonitorInstanceInterface /home/djf/workspace/github/QtDBusXmlTool/filemonitorInstance.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef FILEMONITORINSTANCE_INTERFACE_H_1436344876
#define FILEMONITORINSTANCE_INTERFACE_H_1436344876

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.filemanager.Backend.Monitor.m14
 */
class FileMonitorInstanceInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.filemanager.Backend.Monitor"; }

public:
    FileMonitorInstanceInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~FileMonitorInstanceInterface();

    Q_PROPERTY(uint ID READ iD)
    inline uint iD() const
    { return qvariant_cast< uint >(property("ID")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Cancel()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Cancel"), argumentList);
    }

    inline QDBusPendingReply<bool> IsCancelled()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("IsCancelled"), argumentList);
    }

    inline QDBusPendingReply<> SetRateLimit(int in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("SetRateLimit"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void Changed(const QString &in0, const QString &in1, uint in2);
};

#endif
