#include "about.h"

About::About()
{
    QVBoxLayout *pBox = new QVBoxLayout;
    pBox->setSpacing(5);
    pBox->setMargin(5);

    QGridLayout *pMain = new QGridLayout;
    pMain->setSpacing(0);
    pMain->setMargin(0);
    QPushButton *Button = new QPushButton("OK");
    connect(Button,SIGNAL(clicked()),SLOT(accept()));
    Button->setMaximumWidth(27);
    Button->setMaximumHeight(27);
    Button->setMinimumHeight(27);
    Button->setMinimumWidth(27);

    QIcon icon(":/images/info.png");
    setWindowIcon(icon);
    setWindowTitle("MDP");
    QString str;

    QLabel *label1 = new QLabel;
    str.append(tr("<p><font face=\"Arial\" size=\"4\" color =\"#121B58\"><b>MDP 1.0</b></font></p>"));
    label1->setText(str);
    pMain->addWidget(label1,0,0,1,2,Qt::AlignCenter);

    str.clear();
    QLabel *label2 = new QLabel;
    str.append(tr("<font face=\"Arial\" size=\"3\" color =\"#626B73\"> Основана на Qt 4.5.3 (32bit)</font>"));
    label2->setText(str);
    pMain->addWidget(label2,1,0,1,2,Qt::AlignCenter);

    str.clear();
    QLabel *label3 = new QLabel;
    str.append(tr("<p><font face=\"Arial\" size=\"3\" color =\"#182472\"> <br>Программа предназначена для регистрации параметров подшипника:</br>"));
    str.append(tr("<br>  - нормированного интегрального времени микроконтактирования (НИВ) "));
    str.append(tr("<br>  - активного сопротивления"));
    str.append(tr("<br>  - частоты вращения сепаратора<br></font></p>"));
    label3->setText(str);
    pMain->addWidget(label3,2,0,1,2,Qt::AlignLeft);

    str.clear();
    QLabel *label4 = new QLabel;
    str.append(tr("<font face=\"Arial\" size=\"2\" color =\"#626B73\">Разработчик: Чернышов В.Н."));
    str.append(tr("<br>Об ошибках в работе программы сообщать: bukchep@gmail.com </font>"));
    label4->setText(str);
    pMain->addWidget(label4,3,0,Qt::AlignLeft);

    pMain->addWidget(Button,3,1,Qt::AlignRight);
    pBox->addLayout(pMain);
    setLayout(pBox);
}
