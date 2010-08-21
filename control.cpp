#include "control.h"

control::control()
{
    MainRegion = new mainregion;

    DisplayNit = new Plotter;
    DisplayR = new Plotter;
    DisplayFsep = new Plotter;

    SettingsDisplayR = new PlotSettings;
    SettingsDisplayNit = new PlotSettings;
    SettingsDisplayFsep = new PlotSettings;

    CreateData();
    CreateThread();
}
void control::start(int mode, QTime time, int NIT, int sw)
{
    DeleteData();
    DeleteThread();
    CreateData();
    CreateThread();
    Mode=mode;
    float T_NIT=0;
    switch(Mode)
    {
        case 0:
            DisplayNit = MainRegion->Graph1;
            SettingsDisplayNit = MainRegion->settingsGraph1;
            break;
        case 1:
            DisplayR = MainRegion->Graph1;
            SettingsDisplayR = MainRegion->settingsGraph1;
            break;
        case 2:
            DisplayFsep = MainRegion->Graph1;
            SettingsDisplayFsep = MainRegion->settingsGraph1;
            break;
        case 3:
            DisplayNit = MainRegion->Graph1;
            SettingsDisplayNit = MainRegion->settingsGraph1;
            DisplayFsep = MainRegion->Graph2;
            SettingsDisplayFsep = MainRegion->settingsGraph2;
            break;
        case 4:
            DisplayNit = MainRegion->Graph1;
            SettingsDisplayNit = MainRegion->settingsGraph1;
            DisplayR = MainRegion->Graph2;
            SettingsDisplayR = MainRegion->settingsGraph2;
            break;
        case 5:
            DisplayR = MainRegion->Graph1;
            SettingsDisplayR = MainRegion->settingsGraph1;
            DisplayFsep = MainRegion->Graph2;
            SettingsDisplayFsep = MainRegion->settingsGraph2;
            break;
        default:
            break;
    }
    switch(NIT)
    {
        case 0:
            T_NIT=1.0;
            break;
        case 1:
            T_NIT=0.5;
            break;
        case 2:
            T_NIT=0.1;
            break;
        default:
            break;
    }
    pThreadGraph->SetConf(mode,time,T_NIT,sw);
    pAdapter->SetConfig(mode,time,T_NIT,sw);
    pUSB->SetConfiguration(mode,time,T_NIT,sw);
    pThreadGraph->start(QThread::IdlePriority);
    pAdapter->start(QThread::IdlePriority);
    pBuffer->start(QThread::InheritPriority);
    pUSB->start(QThread::InheritPriority);
}
void control::save()
{
    switch(Mode)
    {
        case 0:
            SaveData(QString(tr("Сохранение данных НИВ")),DATANIT);
            break;
        case 1:
            SaveData(QString(tr("Сохранение данных сопротивления")),DATAR);
            break;
        case 2:
            SaveData(QString(tr("Сохранение данных частоты сепаратора")),DATAFSEP);
            break;
        case 3:
            SaveData(QString(tr("Сохранение данных НИВ")),DATANIT);
            SaveData(QString(tr("Сохранение данных частоты сепаратора")),DATAFSEP);
            break;
        case 4:
            SaveData(QString(tr("Сохранение данных НИВ")),DATANIT);
            SaveData(QString(tr("Сохранение данных сопротивления")),DATAR);
            break;
        case 5:
            SaveData(QString(tr("Сохранение данных сопротивления")),DATAR);
            SaveData(QString(tr("Сохранение данных частоты сепаратора")),DATAFSEP);
            break;
        default:
            break;
    }
    emit SetFlagSaveData();
}
void control::stop()
{
    pUSB->stop();
    pUSB->quit();
   // ShowFullGraph();
}
void control::StopThreads()
{
    pBuffer->quit();
    pAdapter->quit();
    pThreadGraph->quit();
}
void control::EndAdapter()
{
    pAdapter->EndAdapter();
}
void control::EndGraph()
{
    pThreadGraph->EndThreadGraph();
}
void control::receiveError(QString str)
{
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
    emit Error();
    emit SetFlagSaveData();
}
void control::DeleteData()
{
    delete mut;
    delete mutG;

    delete dataNit;
    delete dataR;
    delete dataFsep;

    delete dataGNit;
    delete dataGR;
    delete dataGFsep;

    delete DATAR;
    delete DATANIT;
    delete DATAFSEP;

    delete DATAGR;
    delete DATAGNIT;
    delete DATAGFSEP;
    delete DATAGR2;
    delete DATAGNIT2;
    delete DATAGFSEP2;
}
void control::CreateData()
{
    mut = new QMutex;
    mutG = new QMutex;

    dataNit = new QQueue<BYTE>;
    dataR = new QQueue<BYTE>;
    dataFsep = new QQueue<BYTE>;

    dataGNit = new QQueue<QPointF>;
    dataGR = new QQueue<QPointF>;
    dataGFsep = new QQueue<QPointF>;

    DATAR = new QVector<QPointF>;
    DATANIT = new QVector<QPointF>;
    DATAFSEP = new QVector<QPointF>;

    DATAGR = new QVector<QPointF>;
    DATAGNIT = new QVector<QPointF>;
    DATAGFSEP = new QVector<QPointF>;
    DATAGR2 = new QVector<QPointF>;
    DATAGNIT2 = new QVector<QPointF>;
    DATAGFSEP2 = new QVector<QPointF>;
}
void control::DeleteThread()
{
    delete pUSB;
    delete pBuffer;
    delete pAdapter;
    delete pThreadGraph;
}
void control::CreateThread()
{
    pUSB = new USB;
    pBuffer = new buffer(*mut);
    pAdapter = new adapter(*mut,*mutG);
    pThreadGraph = new threadGraph(*DATAGNIT,*DATAGR,*DATAGFSEP,*DATAGNIT2,*DATAGR2,*DATAGFSEP2,*mutG);

    connect(pUSB,SIGNAL(addData(BYTE*,int)),pBuffer,SLOT(SlotData(BYTE*,int)));
    connect(pUSB,SIGNAL(addDataNit(BYTE*,int)),pBuffer,SLOT(SlotNIT(BYTE*,int)));
    connect(pUSB,SIGNAL(addDataR(BYTE*,int)),pBuffer,SLOT(SlotR(BYTE*,int)));
    connect(pUSB,SIGNAL(addDataFsep(BYTE*,int)),pBuffer,SLOT(SlotFsep(BYTE*,int)));
    connect(pUSB,SIGNAL(signalError(QString)),this,SLOT(receiveError(QString)));

    connect(pThreadGraph,SIGNAL(DrawNit(double,double,double,double,bool)),this,SLOT(DrawingNit(double,double,double,double,bool)));
    connect(pThreadGraph,SIGNAL(DrawR(double,double,double,double)),this,SLOT(DrawingR(double,double,double,double)));
    connect(pThreadGraph,SIGNAL(DrawFsep(double,double,double,double)),this,SLOT(DrawingFsep(double,double,double,double)));

    connect(pUSB,SIGNAL(endMeasurement()),pBuffer,SLOT(EndBuffer()));
    connect(pBuffer,SIGNAL(StopBuffer()),this,SLOT(EndAdapter()));
    connect(pAdapter,SIGNAL(StopAdapter()),this,SLOT(EndGraph()));
    connect(pThreadGraph,SIGNAL(StopGraph()),this,SLOT(StopThreads()));

    pBuffer->ConnectDataBuffer(*dataNit,*dataR,*dataFsep);
    pAdapter->ConnectDataAdapter(*dataNit,*dataR,*dataFsep,*dataGNit,*dataGR,*dataGFsep);
    pThreadGraph->ConnectDataGraph(*dataGNit,*dataGR,*dataGFsep,*DATANIT,*DATAR,*DATAFSEP);
}
void control::SaveData(QString str,QVector<QPointF> *DATA)
{
    QFileDialog *dialog = new QFileDialog;
    unsigned long size=DATA->size();
    QString fileName = dialog->getSaveFileName(MainRegion,str,"");
    if(fileName!="")
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        QPointF point;
        QProgressDialog *pprd = new QProgressDialog(tr("Сохранение даннах"),tr("&Отмена"),0,size);
        pprd->setMinimumDuration(0);
        pprd->setWindowTitle(tr("Подожди"));

        for(int i=0;i<size;++i)
        {
            point=DATA->at(i);
            out<<point.x()<<" "<<point.y()<<endl;
            pprd->setValue(i);
            qApp->processEvents();
            if(pprd->wasCanceled())break;
        }
        pprd->setValue(size);
        file.close();
        delete dialog;
        delete pprd;
    }
}
void control::DrawingNit(double minX,double minY,double maxX,double maxY,bool sw)
{
    SettingsDisplayNit->minX=minX;
    SettingsDisplayNit->minY=minY;
    SettingsDisplayNit->maxX=maxX;
    SettingsDisplayNit->maxY=maxY;
    DrawGraph(DisplayNit,SettingsDisplayNit,*DATAGNIT);
    //DATAGNIT->erase(0);
    //if(sw)DrawGraph(DisplayNit,SettingsDisplayNit,*DATAGNIT2);
    //else DrawGraph(DisplayNit,SettingsDisplayNit,*DATAGNIT);
}
void control::DrawingR(double minX,double minY,double maxX,double maxY)
{
    SettingsDisplayR->minX=minX;
    SettingsDisplayR->minY=minY;
    SettingsDisplayR->maxX=maxX;
    SettingsDisplayR->maxY=maxY;
    DrawGraph(DisplayR,SettingsDisplayR,*DATAGR);
    DATAGR->clear();
}
void control::DrawingFsep(double minX,double minY,double maxX,double maxY)
{
    SettingsDisplayFsep->minX=minX;
    SettingsDisplayFsep->minY=minY;
    SettingsDisplayFsep->maxX=maxX;
    SettingsDisplayFsep->maxY=maxY;
    DrawGraph(DisplayFsep,SettingsDisplayFsep,*DATAGFSEP);
    DATAGFSEP->clear();
}
void control::DrawGraph(Plotter *Graph,PlotSettings *SettingsGraph, QVector<QPointF> &data)
{
    Graph->setPlotSettings(*SettingsGraph);
    Graph->setCurveData(1,data);
}
