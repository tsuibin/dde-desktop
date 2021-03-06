/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "global.h"
#include <QMimeDatabase>
#include <QStandardPaths>
#include "widgets/util.h"


QString getThumbnailsPath(){
    QString cachePath = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0);
    QString thumbnailPath = joinPath(cachePath, "thumbnails");
    if (!QDir(thumbnailPath).exists()){
        QDir(thumbnailPath).mkpath(thumbnailPath);
    }
    return thumbnailPath;
}

QString decodeUrl(QString url){
    if (!url.startsWith(FilePrefix)){
        url = FilePrefix + url;
        url = QUrl(url).toLocalFile();
    }else{
        url = QUrl(url).toLocalFile();
    }
    return url;
}

QString deleteFilePrefix(QString path){
    QString ret = path.replace(FilePrefix, "");
    return decodeUrl(ret);
}

bool isDesktop(QString url){
    if (url.startsWith(FilePrefix)){
        url.replace(FilePrefix, "");
    }
    if (url == desktopLocation){
        return true;
    }
    return false;
}

bool isAppGroup(QString url){
    if (url.startsWith(FilePrefix)){
        url.replace(FilePrefix, "");
    }
    QFileInfo f(url);
    if (decodeUrl(f.path()) == desktopLocation){
        if (f.fileName().startsWith(RichDirPrefix)){
            return true;
        }
    }
    return false;
}


bool isApp(QString url){
    if (url.endsWith(AppSuffix)){
        return true;
    }
    return false;
}

bool isAllApp(QStringList urls){
    if (urls.length() > 0){
        bool ret = true;
        foreach (QString url, urls) {
            ret = ret && isApp(url);
            if (!ret){
                return false;
            }
        }
        return ret;
    }else{
        return false;
    }
}

bool isComputer(QString url){
    if (url == ComputerUrl){
        return true;
    }
    return false;
}

bool isTrash(QString url){
    if (url == TrashUrl){
        return true;
    }
    return false;
}


bool isFolder(QString url){
    url.replace(FilePrefix, "");
    if (QDir(url).exists()){
        if (decodeUrl(url).contains(desktopLocation)){
            if (QDir(url).dirName().startsWith(RichDirPrefix)){
                return false;
            }
        }
        return true;
    }
    return false;
}

bool isInDesktop(QString url){
    if (decodeUrl(QFileInfo(url).path()) == desktopLocation){
        return true;
    }else if (QFileInfo(url).path() == desktopLocation){
        return true;
    }else if (url.contains(desktopLocation)){
        return true;
    }
    return false;
}

bool isInDesktopFolder(QString url){
    if (decodeUrl(QFileInfo(url).path()) == desktopLocation){
        return true;
    }else if (QFileInfo(url).path() == desktopLocation){
        return true;
    }
    return false;
}


bool isDesktopAppFile(QString url){
    if (QFileInfo(url).fileName().endsWith(AppSuffix)){
        return true;
    }
    return false;
}

bool isRequestThumbnail(QString url){
    QString mimetypeName = getMimeTypeName(url);
    if (SupportMimeTypes.contains(mimetypeName)){
        return true;
    }else{
        return false;
    }
}

QString getMimeTypeGenericIconName(QString url){
    QMimeDatabase mimeDataBae;
    QMimeType mimeType = mimeDataBae.mimeTypeForFile(deleteFilePrefix(url));
    return mimeType.genericIconName();
}

QString getMimeTypeIconName(QString url){
    QMimeDatabase mimeDataBae;
    QMimeType mimeType = mimeDataBae.mimeTypeForFile(deleteFilePrefix(url));
    return mimeType.iconName();
}

QString getMimeTypeName(QString url){
    QMimeDatabase mimeDataBae;
    QMimeType mimeType = mimeDataBae.mimeTypeForFile(deleteFilePrefix(url));
    return mimeType.name();
}
