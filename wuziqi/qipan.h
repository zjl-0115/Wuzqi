#ifndef QIPAN_H
#define QIPAN_H

#include <QWidget>
#include <QMouseEvent>
#include "stdShare.h"

class QiPan : public QWidget
{
    Q_OBJECT
public:
    explicit QiPan(QWidget *parent = nullptr);
protected:
    // 绘画事件
    void paintEvent(QPaintEvent* event) override;
    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent* event) override;
    // 鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override;

private:
    // 判断用户是否获胜
    bool isCheckWin(int row, int col, ROLE role);
    //切换玩家
    void switchPlayer();
    // 计算机落子
    void computerMove();

private:
    // 棋盘尺寸
    static const int m_boardSize = 15;
    // 方块格子尺寸
    static const int m_cellSize = 40;
    // 棋盘距离边缘的距离
    static const int m_margin = 20;

    // 棋盘上的点
    ROLE m_board[m_boardSize][m_boardSize];

    //游戏模式
    MODE m_gameMode;
    //当前玩家
    ROLE m_currentPlayer;

    // 悬停点坐标
    int m_hoverRow;
    int m_hoverCol;
    // 是否需要绘制悬停点
    bool m_hoverFlag;

signals:

public slots:
    void startDoublePlayerGame();  // 开始双人游戏
    void startComputerGame();      // 开始人机游戏
};

#endif // QIPAN_H
