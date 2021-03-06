/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DESKTOPITEM_H
#define DESKTOPITEM_H

#include "dbusinterface/dbustype.h"
#include "desktopenums.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>


class ElidedLabel;
class GrowingElideTextEdit;
class QGraphicsEffect;
class QImage;

class DesktopItem : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)
    Q_PROPERTY(bool hover READ isHover WRITE setHover)
public:

    explicit DesktopItem(QWidget *parent = 0);
    explicit DesktopItem(QString icon, QString name, QWidget *parent = 0);
    explicit DesktopItem(QPixmap icon, QString name, QWidget *parent = 0);
    explicit DesktopItem(QString url, QString icon, QString name, QWidget *parent = 0);
    ~DesktopItem();

    static int OneTextHeight;
    static int DoubleTextHeight;

    void init();
    void initUI();
    void initReadableLabel();
    void initPermissionLabel();
    void initConnect();

    bool isInAppGroup();
    QPixmap getDesktopIcon();
    QString getDesktopName();
    QPixmap getAppGroupIcon();
    QString getUrl();
    QString getRawUrl();
    bool isChecked();
    bool isHover();
    bool isCuted();
    bool isEditing();
    bool isShowSimpleMode();
    QString gridKey();
    DesktopItemInfo& getDesktopItemInfo();
    QMap<QString, DesktopItemInfo> getAppGroupItems();
    GrowingElideTextEdit* getTextEdit();

    QPixmap applyShadowToPixmap(const QString filename);
    void addImageShadow();
    void addTextShadow();
    void disabledTextShadow();
    void enableTextShadow();

    QLabel* getIconLabel();

    void drawBorder(int borderWidth, int radius, QColor borderColor, QColor brushColor);

signals:
    void desktopIconChanged(QString icon);
    void desktopIconChanged(QPixmap& icon);
    void desktopNameChanged(QString name);
    void hoverChanged(bool hover);
    void checkedChanged(bool checked);

public slots:
    void updateSizeByGridSize(SizeType type);
    void setIsInAppGroup(bool flag);
    void setDesktopIcon(QString icon);
    void setDesktopIcon(QPixmap& icon);
    void updateAppGroupIcon();
    void setDesktopName(QString name);
    void setChecked(bool checked, bool isExpanded=false);
    void setHover(bool hover);
    void setCuted();
    void cancelCuted();
    void setEdited(bool flag);
    void setEditing();
    void setAllChecked(bool flag);
    void setHoverObjectName(QString name);
    void setUrl(QString url);
    void setRaWUrl(QString url);
    void setDesktopItemInfo(DesktopItemInfo& desktopItemInfo);
    void setAppGroupItems(QMap<QString, DesktopItemInfo> items);
    void changeToBeAppGroupIcon();
    void changeBacktoNormal();
    void showFullWrapName();
    void showSimpWrapName();
    void updateHeight(int textHeight);

    void showContextMenu(QPoint pos);
    void handleCut(QString url);
    void handleCopy(QString url);
    void handlePaste(QString url);
    void handleSelectAll(QString url);

    void setUserReadOnly(bool isUserReadOnly);
    void setUserReadPermisson_000(bool isUserPermisson_000);
    void setReadable(bool isReadable);
    void show();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void moveEvent(QMoveEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void paintEvent(QPaintEvent* event);

private:
    QLabel* m_iconLabel;
    QLabel* m_permissionIndicatorLabel;
    QLabel* m_unReadableIndicatorLabel;
    GrowingElideTextEdit* m_textedit;
    QPixmap m_desktopIcon;
    QString m_desktopName = "";
    QString m_url;
    QString m_rawUrl;
    bool m_isCuted = false;
    bool m_isInAppGroup = false;
    bool m_mouseRightRelease = false;
    bool m_checked = false;
    bool m_hover = false;
    bool m_isEditing = false;
    bool m_isAllChecked = false;
    bool m_isReadable = false;
    bool m_isUserReadOnly = false;
    bool m_isUserPermisson_000 = false;
    QString m_hoverObjectName = "Hover";
    DesktopItemInfo m_desktopItemInfo;
    QMap<QString, DesktopItemInfo> m_appGroupItems;
    QSettings m_settings;
};

#endif // DESKTOPITEM_H
