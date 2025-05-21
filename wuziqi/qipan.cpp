#include "qipan.h"
#include<QPainter>

qipan::qipan(QWidget *parent)
    : QWidget{parent}
{}

// 绘画事件
void qipan::paintEvent(QPaintEvent *event)
{
    // 创建画家类
    QPainter painter(this);
    // 去除毛刺——抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    // 设置背景颜色
    QColor background = QColor(205, 170, 125);
    painter.fillRect(rect(),background);
    // 绘制棋盘
    for(int idx=0;idx<m_boardSize;idx++){
        // 绘制竖线
        painter.drawLine(m_margin+idx*m_cellSize,m_margin,
                         m_margin+idx*m_cellSize,m_margin+(m_boardSize-1)*m_cellSize);
        // 绘制横线
        painter.drawLine(m_margin,m_margin+idx*m_cellSize,
                         m_margin+(m_boardSize-1)*m_cellSize,m_margin+idx*m_cellSize);
    }


}
