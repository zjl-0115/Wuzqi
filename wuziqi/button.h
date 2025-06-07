#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include<QPushButton>
#include <QMessageBox>

class QiPan;

class Button : public QWidget
{
    Q_OBJECT
public:
    explicit Button(QiPan *qipan,QWidget *parent = nullptr);
    void showDifficultyDialog(); // 新增：显示难度选择界面

private:
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
signals:
    void doublePlayerClicked();
    //void computerPlayerClicked();
    void computerPlayerClicked(int difficulty); // 新增：传递难度参数（0=初级，1=高级）

};

#endif // BUTTON_H
