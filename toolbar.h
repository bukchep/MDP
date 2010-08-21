#ifndef TOOLBAR_H
#define TOOLBAR_H
#include <QtGui>
#include "about.h"
#include "packet.h"

class toolbar: public QWidget
{
    Q_OBJECT
 public:
    fff PAC;
    QToolBar *pToolDevice;

    QComboBox *pComboMode;
    QTimeEdit *pTime;
    QComboBox *pComboNIT;

    QSpinBox *pSpinBox;
    QComboBox *pSpinBox2;

    QStringList *lstNIT;
    QStringList *lstF;
    QStringList *lstR;

    QWidget *WLabel2;

    QLabel *label0;
    QLabel *label2;

    QAction *pStart;
    QAction *pStop;
    QAction *pSave;

    //QAction *pInfo;
    QToolButton *pInfo;

    QLabel *label5;

    void DisabledToolbar(bool f);

    toolbar();
    void Control(int mode);

signals:
    void SwowToolBar();
    void HideToolBar();
    void SetDisplayMainRegion(int mode);
    void startDevice(int mode, QTime time, int NIT, int sw);
    void stopDevice();
    void SaveData();
    void setFlagSaveData();

public slots:
    void Show();
    void Hide();
    void ControlGroup(int mode);
    void Start();
    void Stop();
    void Save();
    void ErrorUSB();
    void Info();
};
#endif // TOOLBAR_H
