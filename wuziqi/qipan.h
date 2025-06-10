#ifndef QIPAN_H
#define QIPAN_H

#include <QWidget>
#include <QMouseEvent>
#include<QFile>
#include<QTextStream>
#include<QTimer>
#include "stdShared.h"

class QiPan : public QWidget
{
    Q_OBJECT
public:
    explicit QiPan(QWidget *parent = nullptr);
    //保存对局
    bool saveGame(const QString& filePath);
    //从文件读取对局
    bool loadGame(const QString& filePath);


protected:
    // 绘画事件
    void paintEvent(QPaintEvent* event) override;
    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent* event) override;
    // 鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override;

private:
    // 判断用户是否获胜
    bool isCheckWin(int row,int col,ROLE role);
    //自定义游戏结束对话框
    void showGameOverDialog(const QString& winner);
    //重置游戏
    void resetGame();
    //切换玩家
    void switchPlayer();
    // 计算机落子
    void computerMove();

    // 棋盘尺寸
    static const int m_boardSize=15;
    // 方块格子尺寸
    static const int m_cellSize=40;
    // 棋盘距离边缘的距离
    static const int m_margin=30;

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
    int m_difficulty; // 新增：难度参数（0=初级，1=高级）
    QPoint findBestMove(); // 新增：高级难度 AI 算法
    bool attackcheckLine(int row, int col, int dr, int dc, ROLE role, int count);    // 辅助判断连珠
    bool defencecheckLine(int row, int col, int dr, int dc, ROLE role, int count);
    bool checkObstruction(int row, int col, int dr, int dc, ROLE role);
    bool isOpenEnd(int row, int col, int dr, int dc, ROLE role);


signals:

public slots:
    void startDoublePlayerGame();  // 开始双人游戏
    //void startComputerGame();    // 开始人机游戏
    void startComputerGame(int difficulty); // 新增：带难度参数的开始函数
    void handleResign();
};

#endif // QIPAN_H
