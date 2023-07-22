#include <QResizeEvent>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>

#include <opencv2/opencv.hpp>
#include <ros/ros.h>
#include <ros/package.h>
#include "cv_bridge/cv_bridge.h"
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);
    void imageCallback(const sensor_msgs::ImageConstPtr& msg, int index);

private:
    QGridLayout* gridLayout;
    QVBoxLayout* layout1;
    QVBoxLayout* layout2;
    QVBoxLayout* layout3;
    QVBoxLayout* layout4;
    QLabel *labels[4];
    QPixmap pixmaps[4];
    ros::NodeHandle nh;
    ros::Subscriber sub[4];
};


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // 设置窗口标题
    setWindowTitle("4 Images");

    // 创建4个QLabel并设置标题和图片
    labels[0] = new QLabel("Image 1");
    labels[0]->setPixmap(QPixmap("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/homo_adjust.png"));
    labels[0]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    labels[1] = new QLabel("Image 2");
    labels[1]->setPixmap(QPixmap("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/bird_view.png"));
    labels[1]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    labels[2] = new QLabel("Image 3");
    labels[2]->setPixmap(QPixmap("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/bluepoints.png"));
    labels[2]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    labels[3] = new QLabel("Image 4");
    labels[3]->setPixmap(QPixmap("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/images/redpoints.png"));
    labels[3]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

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
    layout1->addWidget(labels[0]);
    gridLayout->addLayout(layout1, 0, 0);

    layout2 = new QVBoxLayout;
    QLabel* title2 = new QLabel("Title 2");
    title2->setAlignment(Qt::AlignCenter);
    layout2->addWidget(title2);
    layout2->addWidget(labels[1]);
    gridLayout->addLayout(layout2, 0, 1);

    layout3 = new QVBoxLayout;
    QLabel* title3 = new QLabel("Title 3");
    title3->setAlignment(Qt::AlignCenter);
    layout3->addWidget(title3);
    layout3->addWidget(labels[2]);
    gridLayout->addLayout(layout3, 1, 0);

    layout4 = new QVBoxLayout;
    QLabel* title4 = new QLabel("Title 4");
    title4->setAlignment(Qt::AlignCenter);
    layout4->addWidget(title4);
    layout4->addWidget(labels[3]);
    gridLayout->addLayout(layout4, 1, 1);

    // 创建CentralWidget并设置为MainWindow的中心窗口部件
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    // 设置最小窗口尺寸
    setMinimumSize(800, 600);

    // 订阅图片消息
    sub[0] = nh.subscribe<sensor_msgs::Image>("/image_topic_0", 1, std::bind(&MainWindow::imageCallback, this, std::placeholders::_1, 0));
    sub[1] = nh.subscribe<sensor_msgs::Image>("/image_topic_1", 1, std::bind(&MainWindow::imageCallback, this, std::placeholders::_1, 1));
    sub[2] = nh.subscribe<sensor_msgs::Image>("/image_topic_2", 1, std::bind(&MainWindow::imageCallback, this, std::placeholders::_1, 2));
    sub[3] = nh.subscribe<sensor_msgs::Image>("/image_topic_3", 1, std::bind(&MainWindow::imageCallback, this, std::placeholders::_1, 3));

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

    QMainWindow::resizeEvent(event);

    // Scale the pixmaps and set them on the labels
    for (int i = 0; i < 4; i++) {
        if (!pixmaps[i].isNull()) {
            QPixmap scaledPixmap = pixmaps[i].scaled(labels[i]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            // QPixmap scaledPixmap = pixmaps[i].scaled(width, height, Qt::KeepAspectRatio);
            labels[i]->setPixmap(scaledPixmap);
        }
    }
}

void MainWindow::imageCallback(const sensor_msgs::ImageConstPtr& msg, int index)
{
    // Convert the image message to a QImage
    QImage qimage(msg->data.data(), msg->width, msg->height, QImage::Format_RGB888);

    // Convert the QImage to a QPixmap
    QPixmap pixmap = QPixmap::fromImage(qimage);

    // Store the pixmap and set it on the label
    pixmaps[index] = pixmap;
    labels[index]->setPixmap(pixmap);
}


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "qt_node");
    ros::NodeHandle nh;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
