/**************************************************************************
** This file is part of LiteIDE
**
** Copyright (c) 2011-2016 LiteIDE Team. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** In addition, as a special exception,  that plugins developed for LiteIDE,
** are allowed to remain closed sourced and can be distributed under any license .
** These rights are included in the file LGPL_EXCEPTION.txt in this package.
**
**************************************************************************/
// Module: filtermanager.h
// Creator: visualfc <visualfc@gmail.com>

#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include "liteapi/liteapi.h"
#include "quickopenwidget.h"
#include <QPointer>

using namespace LiteApi;

class QComboBox;
class FilterManager : public IFilterManager
{
    Q_OBJECT
public:
    FilterManager();
    virtual ~FilterManager();
    virtual bool initWithApp(IApplication *app);
public:
    void createActions();
public:
    virtual void addFilter(const QString &sym, IFilter *filter);
    virtual void removeFilter(IFilter *filter);
    virtual QList<IFilter*> filterList() const;
    virtual void setCurrentFilter(IFilter *filter);
    virtual IFilter *currentFilter() const;
public slots:
    void showQuickOpen();
    void hideQuickOpen();
protected:
    QuickOpenWidget *m_widget;
    QAction     *m_quickOpenAct;
    QMap<QString,IFilter*> m_filterMap;
    QPointer<IFilter> m_currentFilter;
};

#endif // FILTERMANAGER_H
