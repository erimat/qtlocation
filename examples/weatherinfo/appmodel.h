/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef APPMODEL_H
#define APPMODEL_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkReply>
#include <QtQml/QQmlListProperty>

#include <qgeopositioninfo.h>

//! [0]
class WeatherData : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString dayOfWeek
               READ dayOfWeek WRITE setDayOfWeek
               NOTIFY dataChanged)
    Q_PROPERTY(QString weather
               READ weather WRITE setWeather
               NOTIFY dataChanged)
    Q_PROPERTY(QString weatherDesc
               READ weatherDesc WRITE setWeatherDesc
               NOTIFY dataChanged)
    Q_PROPERTY(QString tempString
               READ tempString WRITE setTempString
               NOTIFY dataChanged)

public:
    explicit WeatherData(QObject *parent = 0);
    WeatherData(const WeatherData &other);

    const WeatherData &operator=(const WeatherData &other);

    QString dayOfWeek() const;
    QString weather() const;
    QString weatherDesc() const;
    QString tempString() const;

    void setDayOfWeek(QString value);
    void setWeather(QString value);
    void setWeatherDesc(QString value);
    void setTempString(QString value);

signals:
    void dataChanged();
//! [0]
private:
    QString m_dayOfWeek;
    QString m_weather;
    QString m_weatherDesc;
    QString m_tempString;
//! [1]
};
//! [1]

Q_DECLARE_METATYPE(WeatherData)

class AppModelPrivate;
//! [2]
class AppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready
               READ ready
               NOTIFY readyChanged)
    Q_PROPERTY(bool hasSource
               READ hasSource
               NOTIFY readyChanged)
    Q_PROPERTY(bool hasValidCity
               READ hasValidCity
               NOTIFY cityChanged)
    Q_PROPERTY(bool hasValidWeather
               READ hasValidWeather
               NOTIFY weatherChanged)
    Q_PROPERTY(bool useGps
               READ useGps WRITE setUseGps
               NOTIFY useGpsChanged)
    Q_PROPERTY(QString city
               READ city WRITE setCity
               NOTIFY cityChanged)
    Q_PROPERTY(WeatherData *weather
               READ weather
               NOTIFY weatherChanged)
    Q_PROPERTY(QQmlListProperty<WeatherData> forecast
               READ forecast
               NOTIFY weatherChanged)

public:
    explicit AppModel(QObject *parent = 0);
    ~AppModel();

    bool ready() const;
    bool hasSource() const;
    bool useGps() const;
    bool hasValidCity() const;
    bool hasValidWeather() const;
    void setUseGps(bool value);

    QString city() const;
    void setCity(const QString &value);

    WeatherData *weather() const;
    QQmlListProperty<WeatherData> forecast() const;

public slots:
    Q_INVOKABLE void refreshWeather();

//! [2]
private slots:
    void networkSessionOpened();
    void positionUpdated(QGeoPositionInfo gpsPos);
    // these would have QNetworkReply* params but for the signalmapper
    void handleGeoNetworkData(QObject *networkReply);
    void handleWeatherNetworkData(QObject *networkReply);

//! [3]
signals:
    void readyChanged();
    void useGpsChanged();
    void cityChanged();
    void weatherChanged();

//! [3]

private:
    AppModelPrivate *d;

//! [4]
};
//! [4]

#endif // APPMODEL_H
