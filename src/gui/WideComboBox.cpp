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
