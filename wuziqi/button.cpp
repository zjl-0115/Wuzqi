#include "button.h"
#include"qipan.h"
#include<QPushButton>
#include<QFileDialog>
#include<QTimer>

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

    btn4=new QPushButton("保存对局",qipan);
    btn4->move(650,250);
    btn4->setFixedSize(80,40);

    btn5=new QPushButton("读取对局",qipan);
    btn5->move(650,300);
    btn5->setFixedSize(80,40);

    // 连接按钮点击信号到自定义信号
    connect(btn1, &QPushButton::clicked, this, &Button::doublePlayerClicked);
    connect(btn2, &QPushButton::clicked, this, &Button::computerPlayerClicked);
    connect(btn3, &QPushButton::clicked, parent, &QWidget::close);
    connect(btn4, &QPushButton::clicked, this, [=](){
        btn4->setText("正在保存");
        QString filePath=QFileDialog::getSaveFileName(
            qipan,"保存对局",QDir::homePath(),"五子棋对局文件(*.gobang)"
            );
        if(!filePath.isEmpty()){

            if (!filePath.endsWith(".gobang", Qt::CaseInsensitive)) {
                filePath += ".gobang";
            }

            if(qipan->saveGame(filePath)){
                btn4->setText("保存成功");
                QTimer::singleShot(1500,this, [this](){ btn4->setText("保存对局"); });
            }else {
                btn4->setText("保存失败");
                QTimer::singleShot(1500,this, [this](){ btn4->setText("保存对局"); });
            }
        }else {
             btn4->setText("保存对局");
        }
    });
    connect(btn5,&QPushButton::clicked, this,[=](){
        btn5->setText("正在读取");
        QString filePath=QFileDialog::getOpenFileName(
            qipan,"读取对局",QDir::homePath(),"五子棋对局文件(*.gobang *.txt)"
            );
        if(!filePath.isEmpty()){
            if(qipan->loadGame(filePath)){
                btn5->setText("读取成功");
                QTimer::singleShot(1500,this, [this](){ btn5->setText("读取对局"); });
            }else {
                btn5->setText("读取失败");
                QTimer::singleShot(1500,this, [this](){ btn5->setText("读取对局"); });
            }
        }else {
            btn5->setText("读取对局");
        }

    });





    }


}
