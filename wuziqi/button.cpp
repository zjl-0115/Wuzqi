#include "button.h"
#include"qipan.h"
#include<QPushButton>

Button::Button(QiPan* qipan, QWidget *parent)
    : QWidget{parent}
{
    if(qipan){
    // 绘制按钮
    btn1=new QPushButton("双人对战",qipan);
    btn1->move(650,100);
    btn1->setFixedSize(80,40);

    btn2=new QPushButton("人机对战",qipan);//人机对战添加难度选择
    btn2->move(650,150);
    btn2->setFixedSize(80,40);

    btn3=new QPushButton("结束游戏",qipan);
    btn3->move(650,200);
    btn3->setFixedSize(80,40);

    // 连接按钮点击信号到自定义信号
    connect(btn2, &QPushButton::clicked, this, &Button::doublePlayerClicked);
    connect(btn2, &QPushButton::clicked, this, &Button::computerPlayerClicked);
    connect(btn3, &QPushButton::clicked, parent, &QWidget::close);

    }


}
