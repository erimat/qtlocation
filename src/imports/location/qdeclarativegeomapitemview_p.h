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

#ifndef QDECLARATIVEGEOMAPITEMVIEW_H
#define QDECLARATIVEGEOMAPITEMVIEW_H

#include "QModelIndex"

#include <QtQuick/QQuickItem>
#include <QtQml/QQmlParserStatus>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QAbstractItemModel;
class QListModelInterface;
class QDeclarativeGeoMap;
class QDeclarativeGeoMapItemBase;

class QDeclarativeGeoMapItemView : public QObject, public QQmlParserStatus
{
    Q_OBJECT

    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QVariant model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QQmlComponent* delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(qreal z READ zValue WRITE setZValue NOTIFY zChanged)

public:
    explicit QDeclarativeGeoMapItemView(QQuickItem *parent = 0);
    ~QDeclarativeGeoMapItemView();

    QVariant model() const;
    void setModel(const QVariant &);

    QQmlComponent *delegate() const;
    void setDelegate(QQmlComponent*);

    void setMapData(QDeclarativeGeoMap*);
    void repopulate();
    void removeInstantiatedItems();

    qreal zValue();
    void setZValue(qreal zValue);

    void setVisible(bool visible);
    bool isVisible() const;

    QDeclarativeGeoMapItemBase* createItem(int modelRow);
    // From QQmlParserStatus
    virtual void componentComplete();
    void classBegin() {}

Q_SIGNALS:
    void modelChanged();
    void delegateChanged();
    void visibleChanged();
    void zChanged();

private:
    QDeclarativeGeoMapItemBase *createItemFromItemModel(int modelRow);
    QDeclarativeGeoMapItemBase *createItemFromListModel(int modelRow);

private Q_SLOTS:
    void itemModelReset();
    void itemModelRowsInserted(QModelIndex, int start, int end);
    void itemModelRowsRemoved(QModelIndex, int start, int end);

    void listModelItemsInserted(int index, int count);
    void listModelItemsRemoved(int index, int count);
    void listModelItemsChanged(int index, int count, const QList<int> &roles);
    void listModelItemsMoved(int from, int to, int count);

private:
    bool visible_;
    bool componentCompleted_;
    QQmlComponent *delegate_;
    QVariant modelVariant_;
    QAbstractItemModel* itemModel_;
    QListModelInterface* listModel_;
    QDeclarativeGeoMap *map_;
    QList<QDeclarativeGeoMapItemBase*> mapItemList_;
};

QT_END_NAMESPACE
QML_DECLARE_TYPE(QT_PREPEND_NAMESPACE(QDeclarativeGeoMapItemView));

#endif
