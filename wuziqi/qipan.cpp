#include "qipan.h"
#include <QPainter>

Qipan::Qipan(QWidget *parent)
    : QWidget{parent}
{

}

// 绘画事件
void Qipan::paintEvent(QPaintEvent *event)
{
    // 创建画家类
    QPainter painter(this);
    // 去除毛刺——抗锯齿

}
