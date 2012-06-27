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

#include "qplaceresult.h"
#include "qplaceresult_p.h"
#include <QtCore/qnumeric.h>

QT_USE_NAMESPACE

QPlaceResultPrivate::QPlaceResultPrivate()
    :   QPlaceSearchResultPrivate(), distance(qQNaN()), sponsored(false)
{
}

QPlaceResultPrivate::QPlaceResultPrivate(const QPlaceResultPrivate &other)
:   QPlaceSearchResultPrivate(), distance(other.distance), place(other.place),
    sponsored(other.sponsored)
{
}

QPlaceResultPrivate::~QPlaceResultPrivate()
{
}

bool QPlaceResultPrivate::compare(const QPlaceSearchResultPrivate *other) const
{
    const QPlaceResultPrivate *od = static_cast<const QPlaceResultPrivate *>(other);
    return QPlaceResultPrivate::compare(other)
           && distance == od->distance
           && place == od->place
           && sponsored == od->sponsored;
}

/*!
    \class QPlaceResult
    \inmodule QtLocation
    \ingroup QtLocation-places
    \ingroup QtLocation-places-data
    \since Qt Location 5.0

    \brief The QPlaceResult class represents a search result containing a place.

    The PlaceResult holds the distance to the place from the center of the search request,
    an instance of the place and an indication of whether the result is
    sponsored or \l {http://en.wikipedia.org/wiki/Organic_search}{organic}.

    The intended usage is that a QPlaceSearchResult can be converted into a QPlaceResult
    like so:

    \snippet snippets/places/requesthandler.h Convert search result

    The implmentation is handled in such a way that object slicing is not an issue.

    \sa QPlaceSearchResult
*/

/*!
    Constructs a new place result object.
*/
QPlaceResult::QPlaceResult()
:   QPlaceSearchResult(new QPlaceResultPrivate)
{
}

/*!
    Destructor.
*/
QPlaceResult::~QPlaceResult()
{
}

/*!
    \fn QPlaceResult::QPlaceResult(const QPlaceSearchResult &other)
    Constructs a copy of \a other if possible, otherwise constructs a default place result.
*/
Q_IMPLEMENT_SEARCHRESULT_COPY_CTOR(QPlaceResult)

Q_IMPLEMENT_SEARCHRESULT_D_FUNC(QPlaceResult)

/*!
    Returns the distance of the place to the search center.  This
    field is only relevant provided the search request contained
    a search area with a search center.  Otherwise,
    the distance is NaN indicating an undefined distance.  The default value
    for distance is NaN.
*/
qreal QPlaceResult::distance() const
{
    Q_D(const QPlaceResult);
    return d->distance;
}

/*!
    Set the \a distance of the search result's place from a search center.
*/
void QPlaceResult::setDistance(qreal distance)
{
    Q_D(QPlaceResult);
    d->distance = distance;
}

/*!
    Returns the place of the search result.
*/
QPlace QPlaceResult::place() const
{
    Q_D(const QPlaceResult);
    return d->place;
}

/*!
    Sets the \a place that this result refers to.
*/
void QPlaceResult::setPlace(const QPlace &place)
{
    Q_D(QPlaceResult);
    d->place = place;
}

/*!
    Returns true if the result is a sponsored result.

    \sa setSponsored()
*/
bool QPlaceResult::isSponsored() const
{
    Q_D(const QPlaceResult);
    return d->sponsored;
}

/*!
    Sets whether the result is a \a sponsored result or not.

    \sa isSponsored()
*/
void QPlaceResult::setSponsored(bool sponsored)
{
    Q_D(QPlaceResult);
    d->sponsored = sponsored;
}