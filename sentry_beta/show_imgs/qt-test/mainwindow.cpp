#include "mainwindow.h"
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置窗口标题
    setWindowTitle("4 Images");

    // 创建4个QLabel并设置标题和图片
    label1 = new QLabel("Image 1");
    label1->setPixmap(QPixmap("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/homo_adjust.png"));
    label1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    label2 = new QLabel("Image 2");
    label2->setPixmap(QPixmap("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/bird_view.png"));
    label2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    label3 = new QLabel("Image 3");
    label3->setPixmap(QPixmap("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/bluepoints.png"));
    label3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    label4 = new QLabel("Image 4");
    label4->setPixmap(QPixmap("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/redpoints.png"));
    label4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    // 创建QGridLayout并设置行列数
    gridLayout = new QGridLayout;
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(10, 10, 10, 10);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 1);

    // 将4个QLabel添加到QVBoxLayout中并加入QGridLayout
    layout1 = new QVBoxLayout;
    QLabel* title1 = new QLabel("Title 1");
    title1->setAlignment(Qt::AlignCenter);
    layout1->addWidget(title1);
    layout1->addWidget(label1);
    gridLayout->addLayout(layout1, 0, 0);

    layout2 = new QVBoxLayout;
    QLabel* title2 = new QLabel("Title 2");
    title2->setAlignment(Qt::AlignCenter);
    layout2->addWidget(title2);
    layout2->addWidget(label2);
    gridLayout->addLayout(layout2, 0, 1);

    layout3 = new QVBoxLayout;
    QLabel* title3 = new QLabel("Title 3");
    title3->setAlignment(Qt::AlignCenter);
    layout3->addWidget(title3);
    layout3->addWidget(label3);
    gridLayout->addLayout(layout3, 1, 0);

    layout4 = new QVBoxLayout;
    QLabel* title4 = new QLabel("Title 4");
    title4->setAlignment(Qt::AlignCenter);
    layout4->addWidget(title4);
    layout4->addWidget(label4);
    gridLayout->addLayout(layout4, 1, 1);

    // 创建CentralWidget并设置为MainWindow的中心窗口部件
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    // 设置最小窗口尺寸
    setMinimumSize(800, 600);
}


MainWindow::~MainWindow()
{
    delete gridLayout;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // 计算每个QLabel的大小并设置
    int margin = qMax(gridLayout->contentsMargins().left(), gridLayout->contentsMargins().right());
    int spacing = gridLayout->spacing();

    int width = (event->size().width() - margin * 2 - spacing) / 2;
    int height = (event->size().height() - margin * 2 - spacing) / 2;

    QPixmap pixmap1("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/homo_adjust.png");
    QPixmap pixmap2("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/bird_view.png");
    QPixmap pixmap3("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/bluepoints.png");
    QPixmap pixmap4("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/redpoints.png");

    label1->setPixmap(pixmap1.scaled(width, height, Qt::KeepAspectRatio));
    label2->setPixmap(pixmap2.scaled(width, height, Qt::KeepAspectRatio));
    label3->setPixmap(pixmap3.scaled(width, height, Qt::KeepAspectRatio));
    label4->setPixmap(pixmap4.scaled(width, height, Qt::KeepAspectRatio));
}
