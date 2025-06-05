#include "qipan.h"
#include "stdShared.h"
#include<QPainter>
#include<QDebug>
#include <QPushButton>
#include<QMessageBox>
#include <QRandomGenerator>
#include <qapplication.h>


    // 快捷键
    // ctrl+f  查找
    // F4      .cpp和.h切换
    // F1      帮助文档


QiPan::QiPan(QWidget *parent)
    : QWidget{parent},
    m_hoverRow(-1),
    m_hoverCol(-1),
    m_hoverFlag(false),
    m_gameMode(NONE),
    m_currentPlayer(PLAYER)
{
    for(int idx=0;idx<m_boardSize;idx++){
        for(int jdx=0;jdx<m_boardSize;jdx++){
            m_board[idx][jdx]=EMPTY;
        }
    }
    // 设置鼠标追踪
    setMouseTracking(true);

}
//保存对局
bool QiPan::saveGame(const QString& filePath){
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"无法打开文件："<<filePath;
        return false;
    }

    QTextStream out(&file);
    //保存游戏模式
    out<<"Mode:"<<(m_gameMode==SINGLE_PLAYER ? "SINGLE":"DOUBLE")<<"\n";
    //保存当前玩家
    if(m_gameMode==DOUBLE_PLAYER)
        out<<"CurrentPlayer:"<<(m_currentPlayer==PLAYER?"PLAYER1":"PLAYER2")<<"\n";
    else
        out<<"CurrentPlayer:"<<(m_currentPlayer==PLAYER?"PLAYER":"COMPUTER")<<"\n";
    //保存棋盘大小
    out<<"BoardSize:"<<m_boardSize<<"\n";
    //保存棋盘状态
    out<<"Board:\n";
    for(int i=0;i<m_boardSize;i++){
        for(int j=0;j<m_boardSize;j++){
            out<<static_cast<int>(m_board[i][j])<<" ";
        }
        out<<"\n";
    }

    file.close();
    return true;

}

//读取对局
bool QiPan::loadGame(const QString& filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"无法打开文件"<<filePath;
        return false;
    }

    QTextStream in(&file);
    QString line;

    for(int i=0; i<m_boardSize; i++){
        for(int j=0; j<m_boardSize; j++){
            m_board[i][j] = EMPTY;
        }
    }
    //读取游戏模式
    line=in.readLine();
    if(!line.startsWith("Mode:")){
        qDebug()<<"无效的文件格式：找不到模式信息";
        file.close();
        return false;
    }
    QString modeStr=line.split(":").at(1).trimmed();
    m_gameMode=(modeStr == "SINGLE") ? SINGLE_PLAYER : DOUBLE_PLAYER;

    // 读取当前玩家
    line = in.readLine();
    if(!line.startsWith("CurrentPlayer:")){
        qDebug()<<"无效的文件格式：找不到当前玩家信息";
        file.close();
        return false;
    }
    QString playerStr = line.split(":").at(1).trimmed();
    if(m_gameMode == DOUBLE_PLAYER){
        m_currentPlayer = (playerStr == "PLAYER1") ? PLAYER : PLAYER2;
    } else {
        m_currentPlayer = (playerStr == "PLAYER") ? PLAYER : COMPUTER;
    }

    // 读取棋盘大小(m_boardSize为静态常量整数，暂时不改动）
   /* line = in.readLine();
    if(line.startsWith("BoardSize:")){
        int size = line.split(":").at(1).trimmed().toInt();
        if(size > 0 && size <= 20) // 确保棋盘大小合理
            m_boardSize = size;
    }*/

    //找到棋盘数据开始行
    while(!in.atEnd()){
        line = in.readLine();
        if(line.startsWith("Board:"))
            break;
    }

    //读取棋盘状态
    for(int i=0;i<m_boardSize&&!in.atEnd();i++){
        line = in.readLine();
        QStringList tokens = line.split(" ");
        for(int j=0; j<m_boardSize && j<tokens.size(); j++){
            int value = tokens[j].toInt();
            if(value >= EMPTY && value <= COMPUTER)
                m_board[i][j] = static_cast<ROLE>(value);
        }
    }
    file.close();
    update(); // 更新界面显示
    return true;
}

// 开始双人游戏
void QiPan::startDoublePlayerGame()
{
    // 重置棋盘
    for(int idx=0;idx<m_boardSize;idx++){
        for(int jdx=0;jdx<m_boardSize;jdx++){
            m_board[idx][jdx]=EMPTY;
        }
    }

    m_gameMode = DOUBLE_PLAYER;
    m_currentPlayer = PLAYER; // 玩家1先手
    update();
}

// 开始人机游戏
void QiPan::startComputerGame()
{
    // 重置棋盘
    for(int idx=0;idx<m_boardSize;idx++){
        for(int jdx=0;jdx<m_boardSize;jdx++){
            m_board[idx][jdx]=EMPTY;
        }
    }

    m_gameMode = SINGLE_PLAYER;
    m_currentPlayer = PLAYER; // 玩家先手
    update();
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
       m_gameMode!=NONE&&
       m_hoverRow>=0&& m_hoverRow<m_boardSize&&
       m_hoverCol>=0&&m_hoverCol<m_boardSize&&
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
            else if(m_board[r][c]==COMPUTER||m_board[r][c]==PLAYER2){
                // 设置笔刷
                painter.setBrush(Qt::white);
                painter.drawEllipse(QPoint(m_margin+r*m_cellSize,m_margin+c*m_cellSize),
                                    m_cellSize/3,m_cellSize/3);
            }
            else{

            }
        }
    }


    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12));
    // 显示对局模式
    QString modeText;
    if(m_gameMode == DOUBLE_PLAYER) modeText="对局模式：双人对战";
    else if(m_gameMode == SINGLE_PLAYER) modeText="对局模式：人机对战";
    else modeText="请选择对局模式";
    //QString modeText = m_gameMode == DOUBLE_PLAYER ? "对局模式：双人对战" : "对局模式：人机对战";
    painter.drawText(10, 20, modeText);

    // 显示当前玩家
    QString currentPlayerText;
    if (m_gameMode == DOUBLE_PLAYER) {
        currentPlayerText = m_currentPlayer == PLAYER ? "当前：玩家1 (黑棋)" : "当前：玩家2 (白棋)";
    } else if (m_gameMode == SINGLE_PLAYER) {
        currentPlayerText = m_currentPlayer == PLAYER ? "当前：玩家 (黑棋)" : "当前：电脑 (白棋)";
    }
    //QString currentPlayerText = m_currentPlayer == PLAYER ? "当前：玩家1 (黑棋)" : "当前：玩家2 (白棋)";
    painter.drawText(200, 20, currentPlayerText);
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
    //双人对战
    if(m_gameMode==DOUBLE_PLAYER) {
    int row = (event->position().x()-m_margin+(m_cellSize/2))/m_cellSize;
    int col = (event->position().y()-m_margin+(m_cellSize/2))/m_cellSize;

    // 判断鼠标点击是否在棋盘内部
    if(row>=0&&row<m_boardSize&&
        col>=0&&col<m_boardSize&&
        m_board[row][col]==EMPTY)
      {
        // 插入数据
        m_board[row][col]=m_currentPlayer;

        // 手动更新绘画事件
        update();

        // 判断玩家是否获胜
       bool res=isCheckWin(row,col,m_currentPlayer);
        if(res==true)
       {
           QString winner=(m_currentPlayer==PLAYER)?"玩家1获胜":"玩家2获胜";
           showGameOverDialog(winner);
           return;
        }
        //切换玩家
        switchPlayer(); 
      }
    }
    //人机对战
    else if (m_gameMode == SINGLE_PLAYER && m_currentPlayer == PLAYER) {
        int row = (event->position().x() - m_margin + (m_cellSize / 2)) / m_cellSize;
        int col = (event->position().y() - m_margin + (m_cellSize / 2)) / m_cellSize;

        if (row >= 0 && row < m_boardSize &&
            col >= 0 && col < m_boardSize &&
            m_board[row][col] == EMPTY)
        {
            m_board[row][col] = m_currentPlayer;
            update();

            bool res = isCheckWin(row, col, m_currentPlayer);
            if (res == true)
            {
                showGameOverDialog("人机获胜！");
                return;
            }
            switchPlayer();
            computerMove();
        }
    }

}

//切换玩家
void QiPan::switchPlayer()
{
    if (m_gameMode == DOUBLE_PLAYER) {
        m_currentPlayer = (m_currentPlayer == PLAYER) ? PLAYER2 : PLAYER;
    } else if (m_gameMode == SINGLE_PLAYER) {
        m_currentPlayer = (m_currentPlayer == PLAYER) ? COMPUTER : PLAYER;
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

        if(cnt>=5){
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

//游戏结束对话框
void QiPan::showGameOverDialog(const QString& winner) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("游戏结束");
    msgBox.setText(winner);
    msgBox.setIcon(QMessageBox::Information);
    //自定义按钮
    QPushButton* retryBtn=msgBox.addButton("再来一局",QMessageBox::ActionRole);
    QPushButton* closeBtn=msgBox.addButton("退出游戏",QMessageBox::RejectRole);
    msgBox.setDefaultButton(retryBtn);

    msgBox.setDetailedText("详细信息：当前对局已结束");
    msgBox.exec();

    if (msgBox.clickedButton()==retryBtn) {
        resetGame(); // 重新开始游戏
    } else if (msgBox.clickedButton() == closeBtn) {
        // 退出游戏（可根据需求关闭窗口或应用程序）
        QApplication::closeAllWindows();
        /*if (QWidget *parentWidget = qobject_cast<QWidget*>(parent())) {
            parentWidget->close();  // 关闭父窗口
        } else {
            close(); // 关闭当前棋盘窗口
        }*/
    }

}

//重置棋盘
void QiPan::resetGame() {
    //重置棋盘
    for(int i=0;i<m_boardSize;i++){
        for(int j=0;j<m_boardSize;j++){
            m_board[i][j]=EMPTY;
        }
    }

    if (m_gameMode == DOUBLE_PLAYER) {
        m_currentPlayer = PLAYER; // 玩家1先手
    } else if (m_gameMode == SINGLE_PLAYER) {
        m_currentPlayer = PLAYER; // 玩家先手
    }

    update();
}


//计算机随机落子
void QiPan::computerMove() {
    // 收集所有空位置
    QVector<QPoint> emptyPositions;
    for (int i = 0; i < m_boardSize; ++i) {
        for (int j = 0; j < m_boardSize; ++j) {
            if (m_board[i][j] == EMPTY) {
                emptyPositions.append(QPoint(i, j));
            }
        }
    }
    // 若有可落子位置
    if (!emptyPositions.isEmpty()) {
        // 生成随机索引
        int index = QRandomGenerator::global()->bounded(emptyPositions.size());
        QPoint pos = emptyPositions[index];
        int row = pos.x();
        int col = pos.y();
        // 落子
        m_board[row][col] = COMPUTER;
        update();
        // 检查胜负
        bool res = isCheckWin(row, col, COMPUTER);
        if (res) {
            showGameOverDialog("人机获胜！");
            return;
        }
        switchPlayer();
    }
}
