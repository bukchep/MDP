#include "toolbar.h"

toolbar::toolbar()
{
    /***создание панели прибора***/
    pToolDevice = new QToolBar();
    pToolDevice->setIconSize(QSize(22,22));
    pToolDevice->setContentsMargins(0,0,0,0);
    pToolDevice->setMaximumHeight(30);
    //QPalette *pal = new QPalette;
    //pal->setColor(pToolDevice->backgroundRole(),"#b2c8ea");
    //pToolDevice->setPalette(*pal);
    //pToolDevice->setAutoFillBackground(true);

    QHBoxLayout *pEx0 = new QHBoxLayout;
    QWidget *pW0 = new QWidget;
    pEx0->setSpacing(0);
    pEx0->setMargin(0);
    pEx0->addStretch(1);
    pW0->setLayout(pEx0);
    pToolDevice->addWidget(pW0);

    QWidget *WLabel0 = new QWidget;
    label0 = new QLabel;
    label0->setText(tr(" Режим измерения: "));
    QHBoxLayout *pLabel0 = new QHBoxLayout;
    pLabel0->setSpacing(0);
    pLabel0->setMargin(0);
    pLabel0->addWidget(label0);
    WLabel0->setLayout(pLabel0);
    WLabel0->setMaximumHeight(20);
    pToolDevice->addWidget(WLabel0);

    pComboMode= new QComboBox; 
    pComboMode->setMaximumWidth(50);
    pComboMode->setMinimumHeight(24);
    QStringList lstMode;
    lstMode<<tr("НИВ")<<tr("R")<<tr("fсеп");//<<tr("НИВ и fсеп")<<tr("НИВ и R")<<tr("R и fсеп")
    pComboMode->addItems(lstMode);
    connect(pComboMode,SIGNAL(activated(int)),this,SLOT(ControlGroup(int)));
    pToolDevice->addWidget(pComboMode);

    WLabel2 = new QWidget;
    WLabel2->setMinimumWidth(140);
    label2 = new QLabel;
    label2->setText(tr("   Интервал усреднения, с: "));
    QHBoxLayout *pLabel2 = new QHBoxLayout;
    pLabel2->setSpacing(0);
    pLabel2->setMargin(0);
    pLabel2->addWidget(label2);
    WLabel2->setLayout(pLabel2);
    WLabel2->setMaximumHeight(20);
    pToolDevice->addWidget(WLabel2);

    pComboNIT= new QComboBox;
    //pComboNIT->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
   // pComboNIT->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    //pComboNIT->setMinimumContentsLength(2);
    pComboNIT->setMinimumHeight(24);
    pComboNIT->setMinimumWidth(30);
    //pComboNIT->setMaximumWidth(80);

    lstNIT = new QStringList ;
    *lstNIT<<tr("1.0")<<tr("0.5")<<tr("0.1");
    lstF = new QStringList;
    *lstF<<tr("8")<<tr("9")<<tr("10")<<tr("11");
    lstR = new QStringList;
    *lstR<<tr("1кОм")<<tr("1МОм");


    pToolDevice->addWidget(pComboNIT);



    /*QWidget *WLabel3 = new QWidget;
    QLabel *label3 = new QLabel;
    str=tr("  Период переключения: ");
    label3->setText(str);//"<FONT COLOR=#080941>" +str+ "</FONT>"
    QHBoxLayout *pLabel3 = new QHBoxLayout;
    pLabel3->setSpacing(0);
    pLabel3->setMargin(0);
    pLabel3->addWidget(label3);
    WLabel3->setLayout(pLabel3);
    WLabel3->setMaximumHeight(20);*/
    //pToolDevice->addWidget(WLabel3);

    pTime = new QTimeEdit;
    pTime->setMaximumWidth(62);
    pTime->setMinimumHeight(24);
    pTime->setDisabled(1);
    //pToolDevice->addWidget(pTime);
    //pToolDevice->addSeparator();
    //pTime->setStyle(new QPlastiqueStyle);

    /*QWidget *WLabel4 = new QWidget;
    QLabel *label4 = new QLabel;
    str=tr("  Количество тел качения: ");
    label4->setText(str);
    QHBoxLayout *pLabel4 = new QHBoxLayout;
    pLabel4->setSpacing(0);
    pLabel4->setMargin(0);
    pLabel4->addWidget(label4);
    WLabel4->setLayout(pLabel4);
    WLabel4->setMaximumHeight(20);*/
    //pToolDevice->addWidget(WLabel4);

   /* pSpinBox2 = new QSpinBox;
    pSpinBox2->setRange(1,20);
    pSpinBox2->clear();
    pSpinBox2->setMaximumHeight(20);
    pSpinBox2->setMinimumWidth(30);
    pToolDevice->addWidget(pSpinBox2);*/

    //pSpinBox2= new QComboBox;
    //pSpinBox2->setMinimumHeight(24);
    //pSpinBox2->setMinimumWidth(22);
    //pSpinBox2->setMaximumWidth(40);

   // pToolDevice->addWidget(pSpinBox2);
   // pSpinBox2->setDisabled(1);


    QWidget *WLabel5 = new QWidget;
    label5 = new QLabel;
    label5->setText("  ");
    QHBoxLayout *pLabel5 = new QHBoxLayout;
    pLabel5->setSpacing(0);
    pLabel5->setMargin(0);
    pLabel5->addWidget(label5);
    WLabel5->setLayout(pLabel5);
    WLabel5->setMaximumHeight(20);
    pToolDevice->addWidget(WLabel5);
    //label5->hide();

    ///управление USB////
    pStart = new QAction(QIcon(":/images/play.png"),"Start",this);
    pStop = new QAction(QIcon(":/images/stop.png"),"Stop",this);
    pSave = new QAction(QIcon(":/images/save.png"),"Save",this);
    pStop->setDisabled(1);
    pSave->setDisabled(1);
    connect(pStart, SIGNAL(triggered()),this, SLOT(Start()));
    connect(pStop, SIGNAL(triggered()),this, SLOT(Stop()));
    connect(pSave, SIGNAL(triggered()),this, SLOT(Save()));
    pToolDevice->addAction(pStart);
    QWidget *WLabel6 = new QWidget;
    QLabel *label6 = new QLabel;
    label6->setText(" ");
    QHBoxLayout *pLabel6 = new QHBoxLayout;
    pLabel6->setSpacing(0);
    pLabel6->setMargin(0);
    pLabel6->addWidget(label6);
    WLabel6->setLayout(pLabel6);
    WLabel6->setMaximumHeight(20);
    pToolDevice->addWidget(WLabel6);
    pToolDevice->addAction(pStop);
    QWidget *WLabel7 = new QWidget;
    QLabel *label7 = new QLabel;
    label7->setText(" ");
    QHBoxLayout *pLabel7 = new QHBoxLayout;
    pLabel7->setSpacing(0);
    pLabel7->setMargin(0);
    pLabel7->addWidget(label7);
    WLabel7->setLayout(pLabel7);
    WLabel7->setMaximumHeight(20);
    pToolDevice->addWidget(WLabel7);
    pToolDevice->addAction(pSave);
    QWidget *WLabel8 = new QWidget;
    QLabel *label8 = new QLabel;
    label8->setText(" ");
    QHBoxLayout *pLabel8 = new QHBoxLayout;
    pLabel8->setSpacing(0);
    pLabel8->setMargin(0);
    pLabel8->addWidget(label8);
    WLabel8->setLayout(pLabel8);
    WLabel8->setMaximumHeight(20);
    pToolDevice->addWidget(WLabel8);

    QHBoxLayout *pEx1 = new QHBoxLayout;
    QWidget *pW1 = new QWidget;
    pEx1->setSpacing(0);
    pEx1->setMargin(0);
    pEx1->addStretch(1);
    pW1->setLayout(pEx1);
    pToolDevice->addWidget(pW1);

    pInfo = new QToolButton;
    pInfo->setIcon(QIcon(":/images/info.png"));
    //pInfo = new QAction(QIcon(":/images/info.png"),"Info",this);
    QFile file(":/qss/info.qss");
    file.open(QFile::ReadOnly);
    pInfo->setStyleSheet(file.readAll());
    connect(pInfo,SIGNAL(clicked()),this,SLOT(Info()));
    //pToolDevice->addAction(pInfo);
    pToolDevice->addWidget(pInfo);
    /***создание менеджера компоновки***/
    QHBoxLayout *pMain = new QHBoxLayout;
    //задание свойств менеджера компоновки
    pMain->setSpacing(0);
    pMain->setMargin(0);
    /***упаковка панели в менеджер***/
    pMain->addWidget(pToolDevice);
    setLayout(pMain); //установка лэйаута в Widget

    pComboNIT->setMinimumContentsLength(3);
    pComboNIT->clear();
    pComboNIT->addItems(*lstNIT);
}
void toolbar::Show()
{
    pToolDevice->show();
}
void toolbar::Hide()
{
    pToolDevice->hide();
}
void toolbar::ControlGroup(int mode)
{
    Control(mode);
    emit SetDisplayMainRegion(mode);
}
void toolbar::Start()
{
    DisabledToolbar(1);
    emit startDevice(pComboMode->currentIndex(),pTime->time(),pComboNIT->currentIndex(),pSpinBox2->currentIndex());
    emit setFlagSaveData();
}
void toolbar::Stop()
{
    emit stopDevice();
    DisabledToolbar(0);
}
void toolbar::Save()
{
    emit SaveData();
    //DisabledToolbar(0);
}

void toolbar::DisabledToolbar(bool f)
{
    if(f)
    {
        //label5->show();
        pSpinBox2->setDisabled(1);
        pComboMode->setDisabled(1);
        pTime->setDisabled(1);
        pComboNIT->setDisabled(1);
        pStart->setDisabled(1);
        pStop->setDisabled(0);
        pSave->setDisabled(1);
    }
    else
    {
        //label5->hide();
        pComboMode->setDisabled(0);
        pTime->setDisabled(0);
        Control(pComboMode->currentIndex());
        pStart->setDisabled(0);
        pStop->setDisabled(1);
        pSave->setDisabled(0);
    }
}
void toolbar::Control(int mode)
{
    switch(mode)
        {
            case 0:
                label2->clear();
                WLabel2->setMinimumWidth(140);
                pComboNIT->setMinimumWidth(30);
                label2->setText(tr("   Интервал усреднения, с: "));
               // pComboNIT->setMinimumContentsLength(3);
                pComboNIT->clear();
                pComboNIT->addItems(*lstNIT);
                pComboNIT->setCurrentIndex(0);
                break;
            case 1:
                label2->clear();
                WLabel2->setMinimumWidth(131);
                pComboNIT->setMinimumWidth(50);
                label2->setText(tr("      Предел измерения: "));
                //pComboNIT->setMinimumContentsLength(5);
                pComboNIT->clear();
                pComboNIT->addItems(*lstR);
                pComboNIT->setCurrentIndex(0);
                break;
            case 2:
                label2->clear();
                WLabel2->setMinimumWidth(140);
                pComboNIT->setMinimumWidth(30);
                label2->setText(tr("   Количество тел качения:"));
                //pComboNIT->setMinimumContentsLength(3);
                pComboNIT->clear();
                pComboNIT->addItems(*lstF);
                pComboNIT->setCurrentIndex(0);
                break;
            case 3:
                pComboNIT->setDisabled(0);
                pTime->setDisabled(0);
                pTime->setMinimumTime(QTime(0,0,1));
                pTime->setTime(QTime(0,0,1));
                pSpinBox2->setDisabled(0);
                //pSpinBox2->setValue(1);
                break;
            case 4:
                pComboNIT->setDisabled(0);
                pTime->setDisabled(0);
                pTime->setMinimumTime(QTime(0,0,1));
                pTime->setTime(QTime(0,0,1));
                pSpinBox2->setDisabled(1);
                pSpinBox2->clear();
                break;
            case 5:
                pComboNIT->setDisabled(1);
                pComboNIT->clear();
                pTime->setDisabled(0);
                pTime->setMinimumTime(QTime(0,0,1));
                pTime->setTime(QTime(0,0,1));
                pSpinBox2->setDisabled(0);
                //pSpinBox2->setValue(1);
                break;
             default:
                break;
        }
}
void toolbar::ErrorUSB()
{
    DisabledToolbar(0);
    pStart->setDisabled(0);
    pStop->setDisabled(1);
    pSave->setDisabled(1);
}
void toolbar::Info()
{
    About inf;
    QFile file(":/qss/zoombuttton.qss");
    file.open(QFile::ReadOnly);
    inf.setStyleSheet(file.readAll());
    inf.setMaximumHeight(170);
    inf.setMaximumWidth(400);
    inf.exec();
}
