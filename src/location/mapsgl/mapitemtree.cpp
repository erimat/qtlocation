/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/
#include "mapitemtree_p.h"
#include "mapitem.h"

#include <QHash>

MapItemTree::MapItemTree()
        : root_(new AAInterval2DTree<MapItem*>()) {}

MapItemTree::~MapItemTree()
{
    delete root_;
}

void MapItemTree::insert(MapItem *item)
{
    if (item && item->bounds().isValid())
        root_->insert(item->bounds(), item);
}

void MapItemTree::remove(MapItem *item)
{
    if (item && item->bounds().isValid())
        root_->remove(item->bounds(), item);
}

bool MapItemTree::isEmpty() const
{
    return root_->isEmpty();
}

int MapItemTree::size() const
{
    return root_->items().size();
}

QList<MapItem*> MapItemTree::items() const
{
    return root_->items();
}

QList<MapItem*> MapItemTree::itemsAt(const QPoint &point) const
{
    return root_->itemsAt(point);
}

QList<MapItem*> MapItemTree::itemsWithin(const QRect &viewport) const
{
    return root_->itemsWithin(viewport);
}

// TODO make static private
bool checkPoint(const QPoint &p, const QRect &r)
{
    return ((r.x() <= p.x())
            && (p.x() <= r.x() + r.width())
            && (r.y() <= p.y())
            && (p.y() <= r.y() + r.height()));
}

bool checkOverlap(const QRect& r1, const QRect &r2)
{
    return (((r1.x() <= r2.x() + r2.width()) && (r2.x() <= r1.x() + r1.width()))
            &&
            ((r1.y() <= r2.y() + r2.height()) && (r2.y() <= r1.y() + r1.height())));
}

int getIndex(const QPoint &p, const QRect &oldRec, const QRect &newRect)
{
    int result = 0;

    if (oldRec.isValid() && checkPoint(p, oldRec))
        result += 1;

    if (checkPoint(p, newRect))
        result += 2;

    return result;
}

void MapItemTree::makeVisible(const QRect& viewport, QList<MapItem*> &added, QList<MapItem*> &removed)
{
    added.clear();
    removed.clear();

    QList<int> x;

    if (viewport_.isValid()) {
        x << viewport_.x();
        if (viewport_.width() != 0)
            x << viewport_.x() + viewport_.width();
    }

    x << viewport.x();
    if (viewport.width() != 0)
        x << viewport.x() + viewport.width();

    qSort(x.begin(), x.end());

    for (int i = x.size() - 1; i > 0; --i) {
        int x1 = x.at(i);
        int x2 = x.at(i - 1);
        if (x1 == x2) {
            x.removeAt(i);
        }
    }

    QList<int> y;

    if (viewport_.isValid()) {
        y << viewport_.y();
        if (viewport_.height() != 0)
            y << viewport_.y() + viewport_.height();
    }

    y << viewport.y();
    if (viewport.height() != 0)
        y << viewport.y() + viewport.height();

    qSort(y.begin(), y.end());

    for (int i = y.size() - 1; i > 0; --i) {
        int y1 = y.at(i);
        int y2 = y.at(i - 1);
        if (y1 == y2) {
            y.removeAt(i);
        }
    }

    QList<QRect> wasVisible;
    QList<QRect> isVisible;
    QRect overlap;

    int xsize = x.size();
    int ysize = y.size();

    for (int j = 0; j < ysize - 1; ++j) {
        int y1 = y.at(j);
        int y2 = y.at(j + 1);
        for (int i = 0; i < xsize - 1; ++i) {
            int x1 = x.at(i);
            int x2 = x.at(i + 1);

            QRect r = QRect(x1, y1, x2 - x1, y2 - y1);

            QHash<int, int> counts;
            counts[0] = 0;
            counts[1] = 0;
            counts[2] = 0;
            counts[3] = 0;

            counts[getIndex(QPoint(x1, y1), viewport_, viewport)] += 1;
            counts[getIndex(QPoint(x1, y2), viewport_, viewport)] += 1;
            counts[getIndex(QPoint(x2, y1), viewport_, viewport)] += 1;
            counts[getIndex(QPoint(x2, y2), viewport_, viewport)] += 1;

            if (counts[0] != 0) {
                continue;
            }
            if (counts[3] == 4) {
                overlap = r;
                continue;
            }

            if (counts[1] != 0) {
                if (counts[2] != 0) {
                    continue;
                } else {
                    wasVisible << r;
                }
            } else {
                if (counts[2] != 0) {
                    isVisible << r;
                } else {
                    continue;
                }
            }
        }
    }

    for (int i = 0; i < wasVisible.size(); ++i) {

        QList<MapItem*> items = itemsWithin(wasVisible.at(i));

        for (int j = 0; j < items.size(); ++j) {

            MapItem *item = items.at(j);

            bool inMiddleGround = false;
            if (overlap.isValid())
                inMiddleGround = checkOverlap(item->bounds(), overlap);

            if (item->visibleFromViewport() && !inMiddleGround) {
                item->setVisibleFromViewport(false);
                removed << item;
            }
        }
    }

    if (overlap.isValid())
        isVisible << overlap;

    for (int i = 0; i < isVisible.size(); ++i) {

        QList<MapItem*> items = itemsWithin(isVisible.at(i));

        for (int j = 0; j < items.size(); ++j) {

            MapItem *item = items.at(j);

            if (!item->visibleFromViewport()) {
                item->setVisibleFromViewport(true);
                added << item;
            }
        }
    }

    viewport_ = viewport;
}