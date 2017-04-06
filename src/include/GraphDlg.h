#ifndef GRAPHDLG_H
#define GRAPHDLG_H

#include <QWidget>
#include "ChannelListModel.h"
#include "WideComboBox.h"
#include <QCheckBox>
#include <QToolButton>
#include <QColorDialog>

namespace Ui {
class GraphDlg;
}


class ColorButton : public QToolButton
{
    Q_OBJECT
public:
    template <typename... Args> ColorButton(Args... args) :
        QToolButton(args...),
        color("black")
    {
        setColor(color);
        connect(this, &ColorButton::clicked, [=](){
            QColor ret = QColorDialog::getColor(color, parentWidget(), "Choose a graph color");
            if ( ret.isValid() )
                setColor(ret);
        });
    }

    QColor color;

public slots:
    inline void setColor(QColor const& c)
    {
        color = c;
        setStyleSheet(QString("background-color: rgb(%1,%2,%3);").arg(c.red()).arg(c.green()).arg(c.blue()));
    }
};


class GraphDlg : public QDialog
{
     Q_OBJECT
private:
    Ui::GraphDlg *ui;
    ChannelListModel *clm;

    QVector<QCheckBox*> actives;
    QVector<ColorButton*> colors;
    QVector<QComboBox*> types;
    QVector<WideComboBox*> channels;
    QMetaObject::Connection activec, typec, channelc;

    int m_plot;
    bool interactive;

    void addRow(int row, QCheckBox *active, ColorButton *colBtn, QComboBox *type, WideComboBox *channel);

private slots:
    void growTable(bool reactive = true);
    void checkChannelTypes();

    void on_TraceActivate_clicked();
    void on_TraceDeactivate_clicked();
    void on_TraceClear_clicked();

    void on_removePlot_clicked();

public:
     GraphDlg(QWidget *parent = 0);
     ~GraphDlg();

     inline void setPlot(int g) { m_plot = g; }
     inline int plot() const { return m_plot; }

     inline void setInteractive(bool maybe) { interactive = maybe; }

public slots:
     void open();
     void accept();

signals:
     void updatePlot(int row);
     void removePlot(int row);
}; 

#endif
