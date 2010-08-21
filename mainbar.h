#ifndef MAINBAR_H
#define MAINBAR_H
#include <QtGui>

class mainbar: public QWidget
{
    Q_OBJECT
public:
    QToolBar *pToolMain;
    QAction *pHideToolDevice;
    QAction *pShowToolDevice;
   // QAction *pInfo;
    QHBoxLayout *pEx;
    QWidget *pW;

    mainbar();
signals:
    void SwowToolBar();
    void HideToolBar();

public slots:
    void Show();
    void Hide();
    //void Info();
};

#endif // MAINBAR_H
