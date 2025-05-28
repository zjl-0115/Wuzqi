#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qipan.h"
#include <QMainWindow>
#include"button.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // 棋盘类
    QiPan * m_qipan;
    // 按钮类
    Button * m_button;
};
#endif // MAINWINDOW_H
