/****************************************************************************

****************************************************************************/

#include <QApplication>
#include <QMessageBox>
#include "main.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.connect(&app,SIGNAL(lastWindowClosed()),SLOT(quit()));

    MyMainWindow mw;
    mw.show();

    return app.exec();
}
