/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QPLACEREPLY_H
#define QPLACEREPLY_H

#include "qmobilityglobal.h"
#include <QObject>

QT_BEGIN_NAMESPACE

class QPlaceReplyPrivate;
class Q_LOCATION_EXPORT QPlaceReply : public QObject
{
    Q_OBJECT
public:
    enum Error {
        NoError,
        DoesNotExistError,
        PlaceDoesNotExistError,
        CategoryDoesNotExistError,
        CommunicationError,
        ParseError,
        PermissionsError,
        UnsupportedError,
        CancelError,
        UnknownError
    };

    enum Type {
        Reply,
        PlaceDetailsReply,
        PlaceSearchReply,
        TextPredictionReply,
        ReviewReply,
        MediaReply
    };

    QPlaceReply(QObject *parent);
    virtual ~QPlaceReply();

    bool isFinished() const;

    virtual Type type() const;

    QString errorString() const;
    QPlaceReply::Error error() const;

public Q_SLOTS:
    virtual void abort();

Q_SIGNALS:
    void finished();
    void error(QPlaceReply::Error error, const QString &errorString = QString());

protected:
    QPlaceReply(QPlaceReplyPrivate *, QObject *parent = 0);
    void setFinished(bool finished);
    void setError(QPlaceReply::Error error, const QString &errorString);
    QPlaceReplyPrivate *d_ptr;
private:
    Q_DISABLE_COPY(QPlaceReply)
};

QT_END_NAMESPACE

#endif // QPLACEREPLY_H