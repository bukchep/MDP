#include "winmain.h"
#include <QtGui>
#include <QWidget>

int main (int argc, char** argv)
{
    QApplication app(argc, argv);
    QApplication::setStyle(new QPlastiqueStyle);
    QFile file(":/qss/style.qss");
    file.open(QFile::ReadOnly);
    app.setStyleSheet(file.readAll());

    winmain *win = new winmain;
    QIcon icon(":/images/logo.jpeg");
    win->setWindowTitle("MDP 1.0");
    win->setWindowIcon(icon);
    win->show();
    return app.exec();
}

