/****************************************************************************

****************************************************************************/

#include <QApplication>
#include <QMessageBox>
#include <QString>
#include "main.h"

int main(int argc, char **argv)
{
    
    QApplication app(argc, argv);
    app.connect(&app,SIGNAL(lastWindowClosed()),SLOT(quit()));

    //QMessageBox::warning(NULL, QString("My Application"),
//                QString("The signal for index change has been received"),
//               QMessageBox::Close);
    MyMainWindow mw;
    mw.show();

    return app.exec();
}
