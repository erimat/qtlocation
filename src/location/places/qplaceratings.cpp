/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtLocation module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qplaceratings.h"
#include "qplaceratings_p.h"

QT_USE_NAMESPACE

QPlaceRatingsPrivate::QPlaceRatingsPrivate()
    :   QSharedData(), average(0), maximum(0), count(0)
{
}

QPlaceRatingsPrivate::QPlaceRatingsPrivate(const QPlaceRatingsPrivate &other)
:   QSharedData(), average(0), maximum(other.maximum), count(other.count)
{
}

QPlaceRatingsPrivate::~QPlaceRatingsPrivate()
{
}

bool QPlaceRatingsPrivate::operator==(const QPlaceRatingsPrivate &other) const
{
    return average == other.average && maximum == other.maximum && count == other.count;
}

bool QPlaceRatingsPrivate::isEmpty() const
{
    return count == 0 && average == 0 && maximum == 0;
}

/*!
    \class QPlaceRatings
    \inmodule QtLocation
    \ingroup QtLocation-places
    \ingroup QtLocation-places-data
    \since Qt Location 5.0

    \brief The QPlaceRatings class holds rating information about a place.

    Rating information is used to describe how good a place is conceived to be.
    Typically this information is visualized as a number of stars.
    The average() function returns an aggregated ratings value out of a possible
    maximum as given by the maximum() function.

    \snippet snippets/places/requesthandler.h Ratings
*/

/*!
    Constructs a new ratings object.
*/
QPlaceRatings::QPlaceRatings()
    : d(new QPlaceRatingsPrivate)
{
}

/*!
    Constructs a copy of \a other.
*/
QPlaceRatings::QPlaceRatings(const QPlaceRatings &other)
    :d(other.d)
{
}

/*!
    Destroys the ratings object.
*/
QPlaceRatings::~QPlaceRatings()
{
}

/*!
    Assigns \a other to this ratings object and returns
    a reference to this ratings object.
*/
QPlaceRatings &QPlaceRatings::operator=(const QPlaceRatings &other)
{
    if (this == &other)
        return *this;

    d = other.d;
    return *this;
}

/*!
    Returns true if \a other is equal to this ratings object,
    otherwise returns false.
*/
bool QPlaceRatings::operator==(const QPlaceRatings &other) const
{
    return (*(d.constData()) == *(other.d.constData()));
}

/*!
    \fn bool QPlaceRatings::operator!=(const QPlaceRatings &other) const

    Returns true if \a other is not equal to this ratings object,
    otherwise returns false.
*/

/*!
    Returns the average value of individual ratings.
*/
qreal QPlaceRatings::average() const
{
    return d->average;
}

/*!
    Sets the \a average value of the ratings.
*/
void QPlaceRatings::setAverage(qreal average)
{
    d->average = average;
}

/*!
    Returns the maximum possible rating value.
*/
qreal QPlaceRatings::maximum() const
{
    return d->maximum;
}

/*!
    Sets the maximum possible rating value to \a max.
*/
void QPlaceRatings::setMaximum(qreal max)
{
    d->maximum = max;
}

/*!
    Returns the total number of individual ratings.
*/
int QPlaceRatings::count() const
{
    return d->count;
}

/*!
    Sets the total number of individual ratings to \a count.
*/
void QPlaceRatings::setCount(int count)
{
    d->count = count;
}

/*!
    Returns true if all fields of the place ratings are 0; otherwise returns false.
*/
bool QPlaceRatings::isEmpty() const
{
    return d->isEmpty();
}
