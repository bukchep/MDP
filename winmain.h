#ifndef WINMAIN_H
#define WINMAIN_H
#include <QtGui>
#include "mainbar.h"
#include "toolbar.h"
#include "mainregion.h"
#include "control.h"
#include "windows.h"

class winmain: public QWidget
{
    Q_OBJECT
 protected:
    virtual void closeEvent(QCloseEvent *event);
 private:
    bool askOnClose();
    bool flagSaveData;
 public:
    control *Control;
    toolbar *ToolBar;
    mainbar *MainBar;
    winmain();
 public slots:
    void SetFlagSave();
    void SetFlagStart();
};

#endif // WINMAIN_H
