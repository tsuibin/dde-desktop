#include <QGSettings>
#include <QVariant>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>

#include "schemawatcher.h"

SchemaWatcher::SchemaWatcher(QObject *parent) :
    QObject(parent),
    m_gsettings(new QGSettings("com.deepin.dde.desktop",
                              "/com/deepin/dde/desktop/",
                              this))
{
    bool isFirstRun = m_gsettings->get("first-run").toBool();

    if (isFirstRun) {
        onGSettingsChanged();
        m_gsettings->set("first-run", false);
    }

    connect(m_gsettings, &QGSettings::changed, this, &SchemaWatcher::onGSettingsChanged);
}

void SchemaWatcher::onGSettingsChanged()
{
    bool showComputer = m_gsettings->get("show-computer-icon").toBool();
    bool showTrash = m_gsettings->get("show-trash-icon").toBool();

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    if (showComputer) {
        //TODO: use Qt code instead shell commands.
        QProcess::execute(QString("cp /usr/share/applications/dde-computer.desktop %1").arg(desktopDir));
        QProcess::startDetached(QString("chmod +x %1/dde-computer.desktop").arg(desktopDir));
    }

    if (showTrash) {
        //TODO: use Qt code instead shell commands.
        QProcess::execute(QString("cp /usr/share/applications/dde-trash.desktop %1").arg(desktopDir));
        QProcess::startDetached(QString("chmod +x %1/dde-trash.desktop").arg(desktopDir));
    }
}
