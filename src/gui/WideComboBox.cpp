/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "WideComboBox.h"
#include <QEvent>
#include <QAbstractItemView>
#include <QApplication>

WideComboBox::WideComboBox(QWidget *parent) :
    QComboBox(parent)
{}

bool WideComboBox::event(QEvent *e)
{
    static bool recur = false;
    if ( !recur && e->type() == QEvent::ChildAdded ) {
        recur = true;
        if ( !QComboBox::event(e) )
            return false;
        view()->setTextElideMode(Qt::ElideNone);
        int scroll = count() <= maxVisibleItems() ? 0 :
            QApplication::style()->pixelMetric(QStyle::PixelMetric::PM_ScrollBarExtent);
        int max = 0;
        for (int i = 0; i < count(); i++)
        {
            int width = view()->fontMetrics().width(itemText(i));
            if (max < width)
                max = width;
        }
        view()->setMinimumWidth(scroll + max + 10);
        recur = false;
        return true;
    }
    return QComboBox::event(e);
}
