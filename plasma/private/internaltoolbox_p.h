/*
 *   Copyright 2007 by Aaron Seigo <aseigo@kde.org>
 *   Copyright 2008 by Marco Martin <notmart@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef PLASMA_TOOLBOX_P_H
#define PLASMA_TOOLBOX_P_H

#include <QGraphicsWidget>

#include "containment.h"
#include "abstracttoolbox.h"

class QAction;

class KConfigGroup;

namespace Plasma
{

//class Widget;
//class EmptyGraphicsItem;
class InternalToolBoxPrivate;

class InternalToolBox : public AbstractToolBox
{
    Q_OBJECT
#if QT_VERSION >= 0x040600
    Q_INTERFACES(QGraphicsItem)
#endif
public:
    enum Corner {
        Top = 0,
        TopRight,
        TopLeft,
        Left,
        Right,
        Bottom,
        BottomRight,
        BottomLeft
    };

    explicit InternalToolBox(Containment *parent);
    ~InternalToolBox();

    /**
     * create a toolbox tool from the given action
     * @p action the action to associate hte tool with
     */
    void addTool(QAction *action);
    /**
     * remove the tool associated with this action
     */
    void removeTool(QAction *action);
    int size() const;
    void setSize(const int newSize);
    QSize iconSize() const;
    void  setIconSize(const QSize newSize);
    bool isShowing() const;
    void setShowing(const bool show);

    virtual void setCorner(const Corner corner);
    virtual Corner corner() const;

    bool isMovable() const;
    void setIsMovable(bool movable);

    bool isToolbar() const;
    void setIsToolbar(bool toolbar);

    QTransform viewTransform() const;
    void setViewTransform(const QTransform &transform);

    void save(KConfigGroup &cg) const;
    void load(const KConfigGroup &containmentGroup = KConfigGroup());
    void reposition();

    virtual QSize fullWidth() const;
    virtual QSize fullHeight() const;
    virtual QSize cornerSize() const;

    virtual void showToolBox() = 0;
    virtual void hideToolBox() = 0;
public Q_SLOTS:
    /**
     * re-show the toolbox, in case any tools have changed
     */
    void updateToolBox();

protected:
    Containment *containment();
    QPoint toolPosition(int toolHeight);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected Q_SLOTS:
    virtual void toolTriggered(bool);

private:
    InternalToolBoxPrivate *d;

};

} // Plasma namespace
#endif // multiple inclusion guard
