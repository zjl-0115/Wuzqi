#ifndef QIPAN_H
#define QIPAN_H

#include <QWidget>

class qipan : public QWidget
{
    Q_OBJECT
public:
    explicit qipan(QWidget *parent = nullptr);
protected:
    // 绘画事件
    void paintEvent(QPaintEvent* event) override;
signals:
    public slots:
private:
    // 棋盘尺寸
    static const int m_boardSize=15;
    // 方块格子尺寸
    static const int m_cellSize=40;
    // 棋盘距离边缘的距离
    static const int m_margin=20;
};

#endif // QIPAN_H
