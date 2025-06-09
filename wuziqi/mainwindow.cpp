#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "button.h"
#include "qipan.h"


// 垂直布局

#include<QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("五子棋");
    // 设置窗口固定大小
    setFixedSize(800,650);
    // 创建棋盘对象
    m_qipan=new QiPan(this);
    // 创建按钮面板，传入棋盘指针
    m_button = new Button(m_qipan, this);
    // 全局样式表（主窗口及子控件）Add commentMore actions
    QString styleSheet = R"(
        /* 主窗口背景 */
        QMainWindow {
            background-color: #f8f9fa; /* 浅灰色背景 */
            border-radius: 8px;
            font-family: "思源黑体", sans-serif;
        }

        /* 按钮样式（通用） */
        QPushButton {
            background-color: #888888;
            color: white;
            border: none;
            padding: 8px 10px;
            border-radius: 5px;
            font-size: 14px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #45a049; /* 悬停时颜色加深 */Add commentMore actions
            cursor: pointer;
        }
        QPushButton:pressed {
            background-color: #3e8e41; /* 按下时颜色更深 */
            padding-left: 9px; /* 模拟按下位移 */
            padding-top: 9px;
        }

    )";
    setStyleSheet(styleSheet);

    // 设置布局
    QWidget* centerWidget=new QWidget(this);
    setCentralWidget(centerWidget);
    QVBoxLayout* layout = new QVBoxLayout(centerWidget);
    layout->addWidget(m_qipan);

    //连接信号和槽
    connect(m_button,&Button::doublePlayerClicked,
            m_qipan,&QiPan::startDoublePlayerGame);
    connect(m_button, &Button::computerPlayerClicked,
            m_qipan, &QiPan::startComputerGame);
    connect(m_button, &Button::resignClicked, m_qipan, &QiPan::handleResign);
}

MainWindow::~MainWindow()
{
    delete ui;
}
