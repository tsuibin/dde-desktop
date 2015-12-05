/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -N -p pinyin_interface.h:pinyin_interface.cpp -c PinyinInterface pinyin.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef PINYIN_INTERFACE_H_1448959766
#define PINYIN_INTERFACE_H_1448959766

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.api.Pinyin
 */
class PinyinInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.api.Pinyin"; }

public:
    PinyinInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~PinyinInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QStringList> Query(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("Query"), argumentList);
    }

    inline QDBusPendingReply<QString> QueryList(const QStringList &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("QueryList"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

#endif
