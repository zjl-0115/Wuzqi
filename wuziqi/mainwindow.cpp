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
}

MainWindow::~MainWindow()
{
    delete ui;
}
