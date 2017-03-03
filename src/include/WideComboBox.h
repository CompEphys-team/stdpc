#ifndef WIDECOMBOBOX_H
#define WIDECOMBOBOX_H

#include <QComboBox>

class WideComboBox : public QComboBox
{
    Q_OBJECT
public:
    WideComboBox(QWidget *parent = nullptr);
protected:
    bool event(QEvent *event);
};

#endif // WIDECOMBOBOX_H
