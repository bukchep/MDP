#include "winmain.h"

winmain::winmain()
{
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);

    flagSaveData=0;

    ToolBar = new toolbar;
    MainBar = new mainbar;
    Control = new control;

    connect(MainBar,SIGNAL(SwowToolBar()),ToolBar,SLOT(Show()));
    connect(MainBar,SIGNAL(HideToolBar()),ToolBar,SLOT(Hide()));
    connect(ToolBar,SIGNAL(SetDisplayMainRegion(int)),Control->MainRegion,SLOT(SetDisplayMainRegion(int)));
    connect(ToolBar,SIGNAL(startDevice(int,QTime,int,int)),Control,SLOT(start(int,QTime,int,int)));
    connect(ToolBar,SIGNAL(stopDevice()),Control,SLOT(stop()));
    connect(ToolBar,SIGNAL(SaveData()),Control,SLOT(save()));
    connect(Control,SIGNAL(SetFlagSaveData()),this,SLOT(SetFlagSave()));
    connect(Control,SIGNAL(Error()),ToolBar,SLOT(ErrorUSB()));
    connect(ToolBar,SIGNAL(setFlagSaveData()),this,SLOT(SetFlagStart()));

/***создание менеджеров компоновки***/
    QVBoxLayout *pMain = new QVBoxLayout; //основной
//задание свойств менеджеров компоновки
    pMain->setSpacing(0);
    pMain->setMargin(0);
/***упаковка плотера и панелей в менеджеры***/
    pMain->addWidget(Control->MainRegion);
    pMain->addWidget(ToolBar);
    //pMain->addWidget(MainBar);
    setLayout(pMain); //установка основного лэйаута в Widget
}
void winmain::closeEvent(QCloseEvent *event)
{
    if(flagSaveData==1)
    {
         if (askOnClose()) event->accept();
         else  event->ignore();
    }
    else event->accept();
 }
bool winmain::askOnClose()
{
    int r = QMessageBox::question(this, tr("Подтвердите"),tr("Выйти из программы?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes,
            QMessageBox::Cancel | QMessageBox::Escape);
    return (r == QMessageBox::Yes);
}
void winmain::SetFlagSave()
{
    flagSaveData=0;
}
void winmain::SetFlagStart()
{
    flagSaveData=1;
}
