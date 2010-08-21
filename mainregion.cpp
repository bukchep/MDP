#include "mainregion.h"

mainregion::mainregion()
{
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);
    QFile file(":/qss/zoombuttton.qss");
    file.open(QFile::ReadOnly);

    /***создание плотера***/
    Graph1 = new Plotter;
    Graph1->setStyleSheet(file.readAll());
    settingsGraph1 = new PlotSettings;
    settingsGraph2 = new PlotSettings;
    settingsGraph1->maxY=settingsGraph1->maxX=5;
    settingsGraph2->maxY=settingsGraph2->maxX=5;
    settingsGraph1->minY=settingsGraph1->minX=0;
    settingsGraph2->minY=settingsGraph2->minX=0;
    settingsGraph1->numYTicks=settingsGraph1->numXTicks=10;
    settingsGraph2->numYTicks=settingsGraph2->numXTicks=10;
    Graph1->setPlotSettings(*settingsGraph1);
    //connect(Graph,SIGNAL(Zoom()),this,SLOT(Zoom()));
    /***создание плотера 2***/
    Graph2 = new Plotter;
    Graph2->setStyleSheet(file.readAll());
    Graph2->setPlotSettings(*settingsGraph2);
    //connect(Graph2,SIGNAL(Zoom()),this,SLOT(Zoom()));
    Graph2->hide();
 /***создание менеджера компоновки***/
    pMain = new QVBoxLayout;
    pMain->setSpacing(0);
    pMain->setMargin(0);
   /* QSplitter *pspl = new QSplitter(Qt::Vertical);
    QPalette *palmain = new QPalette;
    palmain->setColor(pspl->backgroundRole(),"#b2c1d9");//
    pspl->setPalette(*palmain);
    pspl->setAutoFillBackground(true);*/
    pMain->addWidget(Graph1);
    pMain->addWidget(Graph2);
    //pMain->addWidget(pspl);
    setLayout(pMain);
}

void mainregion::SetDisplayMainRegion(int select)
{
    switch(select)
    {
        case 0:
            Graph2->hide();
            break;
        case 1:
            Graph2->hide();
            break;
        case 2:
            Graph2->hide();
            break;
        case 3:
            Graph2->show();
            break;
        case 4:
            Graph2->show();
            break;
        case 5:
            Graph2->show();
            break;
        default:
            break;
    }
}
