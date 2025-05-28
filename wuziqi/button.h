#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include<QPushButton>

class QiPan;

class Button : public QWidget
{
    Q_OBJECT
public:
    explicit Button(QiPan *qipan,QWidget *parent = nullptr);

private:
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
signals:
    void doublePlayerClicked();
    void computerPlayerClicked();

};

#endif // BUTTON_H
