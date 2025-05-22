#include "qipan.h"
#include<QPainter>
#include<QDebug>
#include<QMessageBox>

QiPan::QiPan(QWidget *parent)
    : QWidget{parent},
    m_hoverRow(-1),
    m_hoverCol(-1),
    m_hoverFlag(false)
{
    for(int idx=0;idx<m_boardSize;idx++){
        for(int jdx=0;jdx<m_boardSize;jdx++){
            m_board[idx][jdx]=EMPTY;
        }
    }
    // 设置鼠标追踪
    setMouseTracking(true);
}

// 绘画事件
void QiPan::paintEvent(QPaintEvent *event)
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
        painter.drawLine(m_margin, m_margin+idx*m_cellSize,
                         m_margin+(m_boardSize-1)*m_cellSize,m_margin+idx*m_cellSize);
    }

    // 绘制悬停点
    if(m_hoverFlag==true&&
       m_hoverRow>=0&& m_hoverRow<=m_boardSize&&
       m_hoverCol>=0&&m_hoverCol<=m_boardSize&&
       m_board[m_hoverRow][m_hoverCol]==EMPTY)
    {
        // 设置笔刷
        painter.setBrush(Qt::red);
        painter.drawEllipse(QPoint(m_margin+m_hoverRow*m_cellSize,m_margin+m_hoverCol*m_cellSize),
                            m_cellSize/6,m_cellSize/6);
    }

    // 绘制棋子
    for(int r=0;r<m_boardSize;r++){
        for(int c=0;c<m_boardSize;c++){
            if(m_board[r][c]==PLAYER){
                // 设置笔刷
                painter.setBrush(Qt::black);
                painter.drawEllipse(QPoint(m_margin+r*m_cellSize,m_margin+c*m_cellSize),
                                    m_cellSize/3,m_cellSize/3);
            }
            else if(m_board[r][c]==COMPUTER){
                // 设置笔刷
                painter.setBrush(Qt::white);
                painter.drawEllipse(QPoint(m_margin+r*m_cellSize,m_margin+c*m_cellSize),
                                    m_cellSize/3,m_cellSize/3);
            }
            else{

            }
        }
    }
}

// 鼠标移动事件
void QiPan::mouseMoveEvent(QMouseEvent *event)
{
    int row = (event->position().x()-m_margin+(m_cellSize/2))/m_cellSize;
    int col = (event->position().y()-m_margin+(m_cellSize/2))/m_cellSize;


    if(row>=0&&row<=m_boardSize&&col>=0&&col<=m_boardSize){

        if(m_hoverRow!=row||m_hoverCol!=col){
            m_hoverRow=row;
            m_hoverCol=col;

            m_hoverFlag=true;

            // 手动触发绘画事件
            update();
        }
    }
    else{
        m_hoverFlag=false;

        // 不在棋盘时清空悬停点坐标
        m_hoverRow=m_hoverCol=-1;
        // 手动触发绘画事件
        update();
    }
}

// 鼠标点击事件
void QiPan::mousePressEvent(QMouseEvent *event)
{
    int row = (event->position().x()-m_margin+(m_cellSize/2))/m_cellSize;
    int col = (event->position().y()-m_margin+(m_cellSize/2))/m_cellSize;

    // 判断鼠标点击是否在棋盘内部
    if(row>=0&&row<=m_boardSize&&
        col>=0&&col<=m_boardSize&&
        m_board[row][col]==EMPTY)
    {
        // 插入数据
        m_board[row][col]=PLAYER;

        // 手动更新绘画事件
        update();

        // 判断玩家是否获胜
       bool res=isCheckWin(row,col,PLAYER);
        if(res==true)
       {
           QMessageBox::information(this,"游戏结束","玩家获胜");
        }
    }
}

// 判断用户是否获胜
bool QiPan::isCheckWin(int row,int col,ROLE role)
{
    // 四个方向
    int direction[4][2]=
    {
        {1,0},
        {0,1},
        {1,-1},
        {1,1},
        };


    bool is=false;
    // 遍历每个方向
    for(int direct=0;direct<4;direct++)
    {
        int cnt=1;

        // 每一个方向探测
        for(int idx=1;idx<5;idx++)
        {
            // 下一个点的坐标
            int nextRow=row+direction[direct][0]*idx;
            int nextCol=col+direction[direct][1]*idx;

            if(nextRow>=0&&nextRow<=m_boardSize&&
                nextCol>=0&&nextCol<=m_boardSize&&
                m_board[nextRow][nextCol]==role)
            {
                cnt++;
            }
            else
            {
                break;
            }
        }

        // 相反方向
        for(int idx=1;idx<5;idx++)
        {
            // 下一个点的坐标
            int nextRow=row-direction[direct][0]*idx;
            int nextCol=col-direction[direct][1]*idx;

            if(nextRow>=0&&nextRow<=m_boardSize&&
                nextCol>=0&&nextCol<=m_boardSize&&
                m_board[nextRow][nextCol]==role)
            {
                cnt++;
            }
            else
            {
                break;
            }
        }

        if(cnt==5){
            is=true;
            break;
        }
    }

    if(is){
        return true;
    }
    else{
        return false;
    }
}
