/****************************************************************************

****************************************************************************/

#include <QApplication>
#include <QMessageBox>
#include "MainWin.h"
#include "Main.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.connect(&app,SIGNAL(lastWindowClosed()),SLOT(quit()));

    MyMainWindow mw;
    mw.show();

    return app.exec();
}