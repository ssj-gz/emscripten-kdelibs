/*
 * This file is part of the KDE project.
 *
 * Copyright (C) 2007 Trolltech ASA
 * Copyright (C) 2008 Urs Wolfer <uwolfer @ kde.org>
 * Copyright (C) 2008 Laurent Montel <montel@kde.org>
 * Copyright (C) 2008 Michael Howell <mhowell123@gmail.com>
 * Copyright (C) 2009 Dawit Alemayehu <adawit @ kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */
#ifndef KGRAPHICSWEBVIEW_H
#define KGRAPHICSWEBVIEW_H

#include <kdewebkit_export.h>

#include <QtWebKit/QGraphicsWebView>

class KUrl;
template<class T> class KWebViewPrivate;

/**
 * @short A re-implementation of QGraphicsWebView that provides KDE integration.
 *
 * This convenience class provides full integration with KDE technologies such as
 * KIO for network request handling, KCookiejar for cookie handling and 
 * KWebPluginFactory for embeded non-html content handling using KDE KParts.
 *
 * @author Urs Wolfer <uwolfer @ kde.org>
 * @since 4.4
 */
class KDEWEBKIT_EXPORT KGraphicsWebView : public QGraphicsWebView
{
    Q_OBJECT
public:
    /**
     * Constructs an empty KWebView with parent @p parent.
     */
    explicit KGraphicsWebView(QGraphicsItem *parent = 0);

    /**
     * Destroys the KWebView.
     */
    ~KGraphicsWebView();

    /**
     * Returns true if access to remote content is allowed.
     *
     * By default access to remote content is allowed.
     *
     * @see setAllowExternalContent()
     * @see KWebPage::isExternalContentAllowed()
     */
    bool isExternalContentAllowed() const;

    /**
     * Set @p allow to false if you want to prevent access to remote content.
     *
     * If this function is set to false only resources on the local system
     * can be accessed through this class. By default fetching external content
     * is allowed.
     *
     * @see isExternalContentAllowed()
     * @see KWebPage::setAllowExternalContent(bool)
     */
    void setAllowExternalContent(bool allow);

Q_SIGNALS:
    /**
     * This signal is emitted when the user wants to navigate to @p url.
     */
    void openUrl(const KUrl &url);

    /**
     * This signal is emitted when the user wants to save @p url.
     *
     * It is activated as a result of a shift-click on a link with the left
     * mouse button.
     */
    void saveUrl(const KUrl &url);

    /**
     * This signal is emitted when the user wants to open @p url in a new window.
     *
     * It is activated as a result of a click on a link with the middle mouse
     * button or a ctrl-click with the left mouse button.
     */
    void openUrlInNewWindow(const KUrl &url);

protected:
    /**
     * Reimplemented for internal reasons, the API is not affected.
     *
     * @see QWidget::wheelEvent
     * @internal
     */
    void wheelEvent(QGraphicsSceneWheelEvent *event);

    /**
     * Reimplemented for internal reasons, the API is not affected.
     *
     * @see QWidget::mousePressEvent
     * @internal
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /**
     * Reimplemented for internal reasons, the API is not affected.
     *
     * @see QWidget::mouseReleaseEvent
     * @internal
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    friend class KWebViewPrivate<KGraphicsWebView>;
    KWebViewPrivate<KGraphicsWebView> * const d;
};

#endif // KWEBVIEW_H