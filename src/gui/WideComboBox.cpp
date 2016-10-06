#include "WideComboBox.h"
#include <QEvent>
#include <QAbstractItemView>
#include <QApplication>
#include <iostream>

WideComboBox::WideComboBox(QWidget *parent) :
    QComboBox(parent)
{}

bool WideComboBox::event(QEvent *e)
{
    if ( e->type() == QEvent::ChildPolished ) {
        if ( !QComboBox::event(e) )
            return false;
        Qt::TextElideMode old = view()->textElideMode();
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
        view()->setMinimumWidth(scroll + max + 5);
        view()->setTextElideMode(old);
        return true;
    }
    return QComboBox::event(e);
}
