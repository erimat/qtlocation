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

#include "qplaceicon.h"
#include "qplaceicon_p.h"
#include "qplacemanager.h"
#include "qplacemanagerengine.h"

QT_USE_NAMESPACE

QPlaceIconPrivate::QPlaceIconPrivate()
    : QSharedData(), manager(0)
{
}

QPlaceIconPrivate::QPlaceIconPrivate(const QPlaceIconPrivate&other)
    : QSharedData(other),
      manager(other.manager),
      parameters(other.parameters)
{
}

QPlaceIconPrivate::~QPlaceIconPrivate()
{
}

QPlaceIconPrivate &QPlaceIconPrivate::operator=(const QPlaceIconPrivate &other)
{
    if (this == &other)
        return *this;

    manager = other.manager;
    parameters = other.parameters;

    return *this;
}

bool QPlaceIconPrivate::operator == (const QPlaceIconPrivate &other) const
{
    return manager == other.manager
            && parameters == other.parameters;
}

/*!
    \class QPlaceIcon
    \inmodule QtLocation
    \ingroup QtLocation-places
    \ingroup QtLocation-places-data
    \since Qt Location 5.0

    \brief The QPlaceIcon class represents an icon.

    \section2 Usage
    The typical usage of an icon is to use the url() function to specify
    a preferred icon size.

    \snippet snippets/places/requesthandler.h icon

    The icons are typically backend dependent, if a manager backend does not support a given size, the URL of the icon that most
    closely matches those parameters is returned.

    The icon class also has a key-value set of parameters.  The precise key one
    needs to use depends on the \l {Information about plugins} {plugin backend}
    being used.  These parameters influence which icon URL is returned by
    the manager and may also be used to specify icon URL locations when
    saving icons.

    If there is only ever one image for an icon, then QPlaceIcon::SingleUrl can be used as a parameter
    key with a QUrl as the associated value.  If this key is set, then the url() function will always return the specified URL
    and not defer to any manager.
*/

/*!
    Parameter key for an icon that always has a single image URL.  The paramter value to be used with this key
    is a QUrl.  An icon with this parameter set will always return the specified URL regardless
    of the requested size when url() is called.
*/
const QString QPlaceIcon::SingleUrl(QLatin1String("singleUrl"));

/*!
    Constructs an icon.
*/
QPlaceIcon::QPlaceIcon()
    : d(new QPlaceIconPrivate)
{
}

/*!
    Constructs a copy of \a other.
*/
QPlaceIcon::QPlaceIcon(const QPlaceIcon &other)
    : d(other.d)
{
}

/*!
    Destroys the icon.
*/
QPlaceIcon::~QPlaceIcon()
{
}

/*!
    Assigns \a other to this icon and returns a reference to this icon.
*/
QPlaceIcon &QPlaceIcon::operator=(const QPlaceIcon &other)
{
    if (this == &other)
        return *this;

    d = other.d;
    return *this;
}

/*!
    Returns true if this icon is equal to \a other, otherwise returns false.
*/
bool QPlaceIcon::operator==(const QPlaceIcon &other) const
{
    return *d == *(other.d);
}

/*!
    \fn QPlaceIcon::operator!=(const QPlaceIcon &other) const

    Returns true if \a other is not equal to this icon, otherwise returns false.
*/

/*!
    Returns an icon URL according to the given \a size.

    If no manager has been assigned to the icon, and the parameters do not contain the QPlaceIcon::SingleUrl key, a default constructed QUrl
    is returned.
*/
QUrl QPlaceIcon::url(const QSize &size) const
{
    if (d->parameters.contains(QPlaceIcon::SingleUrl)) {
        QVariant value = d->parameters.value(QPlaceIcon::SingleUrl);
        if (value.type() == QVariant::Url)
            return value.toUrl();

        return QUrl();
    }

    if (!d->manager)
        return QUrl();

    return d->manager->d->constructIconUrl(*this, size);
}

/*!
    Returns a set of parameters for the icon that are manager/plugin specific.
    These parameters are used by the manager to return the appropriate
    URL when url() is called and to specify locations to save to
    when saving icons.

    Consult the \l {Information about plugins} {plugin documentation}
    for what parameters are supported and how they should be used.
*/
QVariantMap QPlaceIcon::parameters() const
{
    return d->parameters;
}

/*!
    Sets the parameters of the icon.
*/
void QPlaceIcon::setParameters(const QVariantMap &parameters)
{
    d->parameters = parameters;
}

/*!
    Returns the manager that this icon is associated with.
*/
QPlaceManager *QPlaceIcon::manager() const
{
    return d->manager;
}

/*!
    Sets the \a manager that this icon is associated with.  The icon does not take
    ownership of the pointer.
*/
void QPlaceIcon::setManager(QPlaceManager *manager)
{
    d->manager = manager;
}

/*!
    Returns a boolean indicating whether the all the fields of the icon are empty or not.
*/
bool QPlaceIcon::isEmpty() const
{
    return (d->manager == 0
            && d->parameters.isEmpty());
}
