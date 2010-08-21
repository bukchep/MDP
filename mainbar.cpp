#include "mainbar.h"

mainbar::mainbar()
{
    pToolMain = new QToolBar();
    /*QFile file(":/qss/stylemainbar.qss");
    file.open(QFile::ReadOnly);
    pToolMain->setStyleSheet(file.readAll());*/
    pToolMain->setIconSize(QSize(16,16));
    pToolMain->setMaximumHeight(24);
    pShowToolDevice = new QAction(QIcon(":/images/ShowToolBar.png"),"Show Tool bar",this);
    pHideToolDevice = new QAction(QIcon(":/images/HideToolBar.png"),"Hide Tool bar",this);
   // pInfo = new QAction(QIcon(":/images/info.png"),"Info",this);
    connect(pShowToolDevice,SIGNAL(triggered()),this,SLOT(Show()));
    connect(pHideToolDevice,SIGNAL(triggered()),this,SLOT(Hide()));
    //connect(pInfo,SIGNAL(triggered()),this,SLOT(Info()));
    pToolMain->addAction(pHideToolDevice);
    pEx = new QHBoxLayout;
    pW = new QWidget;
    pEx->setSpacing(0);
    pEx->setMargin(0);
    pEx->addStretch(1);
    pW->setLayout(pEx);
    pToolMain->addWidget(pW);
    //pToolMain->addAction(pInfo);
    pToolMain->setContentsMargins(0,0,0,0);

    /***создание менеджера компоновки***/
    QHBoxLayout *pMain = new QHBoxLayout;
    //задание свойств менеджера компоновки
    pMain->setSpacing(0);
    pMain->setMargin(0);
    /***упаковка панели в менеджер***/
    pMain->addWidget(pToolMain);
    setLayout(pMain); //установка лэйаута в Widget
}
void mainbar::Show()
{
    delete pEx;
    delete pW;
    pEx= new QHBoxLayout;
    pW= new QWidget;
    pToolMain->clear();
    pToolMain->addAction(pHideToolDevice);
    emit SwowToolBar();
    pEx->setSpacing(0);
    pEx->setMargin(0);
    pEx->addStretch(1);
    pW->setLayout(pEx);
    pToolMain->addWidget(pW);
    //pToolMain->addAction(pInfo);
}
void mainbar::Hide()
{
    delete pEx;
    delete pW;
    pEx= new QHBoxLayout;
    pW= new QWidget;
    pToolMain->clear();
    pToolMain->addAction(pShowToolDevice);
    emit HideToolBar();
    pEx->setSpacing(0);
    pEx->setMargin(0);
    pEx->addStretch(1);
    pW->setLayout(pEx);
    pToolMain->addWidget(pW);
    //pToolMain->addAction(pInfo);
}
/*void mainbar::Info()
{
    QIcon icon(":/images/info.png");
    QPixmap pic(":/images/logo.jpeg");
    QMessageBox msgBox;
    msgBox.setWindowIcon(icon);
    msgBox.setIconPixmap(pic);
    msgBox.setWindowTitle("MDP");
    QString str(tr("Привет"));
    msgBox.setText("<FONT COLOR=#080941> "+str+" </FONT>");
    msgBox.exec();
}*/
