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

#ifndef QGEOCODINGMANAGERENGINE_TEST_H
#define QGECODINGMANAGERENGINE_TEST_H

#include <qgeoserviceprovider.h>
#include <qgeocodingmanagerengine.h>
#include <QLocale>
#include <qgeoaddress.h>
#include <qgeolocation.h>
#include <qgeocodereply.h>

#include <QTimer>
#include <QDebug>
#include <QTimerEvent>

QT_USE_NAMESPACE


class GeocodeReplyTest :public QGeocodeReply
{
    Q_OBJECT
public:
    GeocodeReplyTest(QObject *parent=0):QGeocodeReply (parent) {}

    void  callAddLocation ( const QGeoLocation & location ) {addLocation(location);}
    void  callSetError ( Error error, const QString & errorString ) {setError(error, errorString);}
    void  callSetFinished ( bool finished ) {setFinished(finished);}
    void  callSetLimit ( int limit ) {setLimit(limit);}
    void  callSetOffset ( int offset ) {setOffset(offset);}
    void  callSetLocations ( const QList<QGeoLocation> & locations ) {setLocations(locations);}
    void  callSetViewport ( QGeoBoundingArea * viewport ) {setViewport(viewport);}
    void abort() {
        emit aborted();
    }
Q_SIGNALS:
    void aborted();
};

class QGeocodingManagerEngineTest: public QGeocodingManagerEngine

{
Q_OBJECT
public:
    QGeocodingManagerEngineTest(const QMap<QString, QVariant> &parameters,
        QGeoServiceProvider::Error *error, QString *errorString) :
        QGeocodingManagerEngine(parameters),
        validateWellKnownValues_(false),
        finishRequestImmediately_(true),
        supported_(true),
        geocodeReply_(0),
        timerId_(0),
        errorCode_(QGeocodeReply::NoError)
    {
        Q_UNUSED(error)
        Q_UNUSED(errorString)
        if (parameters.contains("supported"))
            supported_ = qvariant_cast<bool>(parameters.value("supported"));
        if (parameters.contains("finishRequestImmediately"))
            finishRequestImmediately_ = qvariant_cast<bool>(parameters.value("finishRequestImmediately"));
        if (parameters.contains("validateWellKnownValues"))
            validateWellKnownValues_ = qvariant_cast<bool>(parameters.value("validateWellKnownValues"));

        setSupportsGeocoding(supported_);
        setSupportsReverseGeocoding(supported_);
        setLocale(QLocale (QLocale::German, QLocale::Germany));
    }

    QGeocodeReply* geocode(const QString &searchString,
                            int limit = -1,
                            int offset = 0,
                            QGeoBoundingArea *bounds = 0)
    {
        geocodeReply_ = new GeocodeReplyTest();
        connect(geocodeReply_, SIGNAL(aborted()), this, SLOT(requestAborted()));
        geocodeReply_->callSetViewport(bounds);

        if (searchString.length() == 1) {
            errorString_ = searchString;
            errorCode_ = (QGeocodeReply::Error)searchString.toInt();
        } else {
            errorString_ = "";
            errorCode_ = QGeocodeReply::NoError;
        }

        if (errorCode_ == QGeocodeReply::NoError)
            setLocations(geocodeReply_, searchString, limit, offset);

        if (finishRequestImmediately_) {
            // check if we should finish with error
            if (errorCode_) {
                geocodeReply_->callSetError(errorCode_, errorString_);
            } else {
                geocodeReply_->callSetFinished(true);
            }
        } else {
            // we only allow serialized requests in QML - previous must have been aborted
            Q_ASSERT(timerId_ == 0);
            timerId_ = startTimer(200);
        }
        return static_cast<QGeocodeReply*>(geocodeReply_);
    }

    QGeocodeReply*  geocode ( const QGeoAddress & address, QGeoBoundingArea * bounds )
    {
        geocodeReply_ = new GeocodeReplyTest();
        connect(geocodeReply_, SIGNAL(aborted()), this, SLOT(requestAborted()));
        geocodeReply_->callSetViewport(bounds);

        if (address.street().startsWith("error")) {
            errorString_ = address.street();
            errorCode_ = (QGeocodeReply::Error)address.county().toInt();
        } else {
            errorString_ = "";
            errorCode_ = QGeocodeReply::NoError;
        }
        // 1. Check if we are to validate values
        if (validateWellKnownValues_) {
            if (address.street() != "wellknown street") {
                 geocodeReply_->callSetError(QGeocodeReply::EngineNotSetError, address.street());
            } else {
                geocodeReply_->callSetError(QGeocodeReply::NoError,address.street());
            }
        }

        // 2. Set the locations into the reply
        setLocations(geocodeReply_, address);

        // 3. Finish the request
        if (finishRequestImmediately_) {
            // check if we should finish with error
            if (errorCode_) {
                geocodeReply_->callSetError(errorCode_, errorString_);
            } else {
                geocodeReply_->callSetFinished(true);
            }
        } else {
            // we only allow serialized requests in QML - previous must have been aborted
            Q_ASSERT(timerId_ == 0);
            timerId_ = startTimer(200);
        }
        return static_cast<QGeocodeReply*>(geocodeReply_);
    }

public Q_SLOTS:
    void requestAborted()
    {
        if (timerId_) {
            killTimer(timerId_);
            timerId_ = 0;
        }
        errorString_ = "";
        errorCode_ = QGeocodeReply::NoError;
    }

public:
    void setLocations(GeocodeReplyTest* reply, const QString searchString, int limit, int offset)
    {
        if (limit < 0)
            limit = 0;
        for (int i = 0; i < limit; ++i) {
            QGeoLocation location;
            QGeoAddress address;
            address.setStreet(searchString);
            address.setCounty(QString::number(offset));
            location.setAddress(address);
            reply->callAddLocation(location);
        }
    }

    void setLocations(GeocodeReplyTest* reply, const QGeoAddress& address)
    {
        int count = address.county().toInt();

        for (int i = 0; i < count; ++i) {
            QGeoLocation location;
            location.setAddress(address);
            reply->callAddLocation(location);
        }
    }

    void setLocations(GeocodeReplyTest* reply, const QGeoCoordinate & coordinate)
    {
        for (int i = 0; i < coordinate.longitude(); ++i) {
            QGeoLocation location;
            location.setCoordinate(coordinate);
            reply->callAddLocation(location);
        }
    }

    QGeocodeReply*  reverseGeocode ( const QGeoCoordinate & coordinate, QGeoBoundingArea * bounds )
    {
        geocodeReply_ = new GeocodeReplyTest();
        connect(geocodeReply_, SIGNAL(aborted()), this, SLOT(requestAborted()));

        setLocations(geocodeReply_, coordinate);
        geocodeReply_->callSetViewport(bounds);

        if (coordinate.latitude() > 70) {
            errorString_ = "error";
            errorCode_ = (QGeocodeReply::Error) (coordinate.latitude() - 70);
        } else {
            errorString_ = "";
            errorCode_ = QGeocodeReply::NoError;
        }
        if (finishRequestImmediately_) {
            if (errorCode_) {
                geocodeReply_->callSetError(errorCode_, errorString_);
            } else {
                geocodeReply_->callSetError(QGeocodeReply::NoError,coordinate.toString());
                geocodeReply_->callSetFinished(true);
            }
        } else {
            // we only allow serialized requests in QML - previous must have been aborted or finished
            Q_ASSERT(timerId_ == 0);
            timerId_ = startTimer(200);
        }
        return static_cast<QGeocodeReply*>(geocodeReply_);
    }

protected:
     void timerEvent(QTimerEvent *event)
     {
         Q_ASSERT(timerId_ == event->timerId());
         Q_ASSERT(geocodeReply_);
         killTimer(timerId_);
         timerId_ = 0;
         if (errorCode_) {
             geocodeReply_->callSetError(errorCode_, errorString_);
             emit error(geocodeReply_, errorCode_, errorString_);
        } else {
             geocodeReply_->callSetError(QGeocodeReply::NoError, "no error");
             geocodeReply_->callSetFinished(true);
         }
         emit finished(geocodeReply_);
     }

private:
    bool validateWellKnownValues_;
    bool finishRequestImmediately_;
    bool supported_;
    GeocodeReplyTest* geocodeReply_;
    int timerId_;
    QGeocodeReply::Error errorCode_;
    QString errorString_;
};

#endif