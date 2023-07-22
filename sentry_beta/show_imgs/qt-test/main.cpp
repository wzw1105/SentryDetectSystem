#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
//#include <QApplication>
//#include <QWidget>
//#include <QHBoxLayout>
//#include <QLabel>
//#include <QPixmap>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

//    // 创建主窗口
//    QWidget *window = new QWidget();
//    window->setWindowTitle("Display Four Images");

//    // 创建水平布局
//    QHBoxLayout *layout = new QHBoxLayout();

//    // 加载并显示第一张图片
//    QPixmap image1("/Users/chenxiuyue/Desktop/graduate-projects/yolo-codes/robot_sentry/src/sentry_beta/show_imgs/qt-test/background.png");
//    QLabel *label1 = new QLabel();
//    label1->setPixmap(image1);
//    layout->addWidget(label1);

//    // 加载并显示第二张图片
//    QPixmap image2(":/path/to/image2.png");
//    QLabel *label2 = new QLabel();
//    label2->setPixmap(image2);
//    layout->addWidget(label2);

//    // 加载并显示第三张图片
//    QPixmap image3(":/path/to/image3.png");
//    QLabel *label3 = new QLabel();
//    label3->setPixmap(image3);
//    layout->addWidget(label3);

//    // 加载并显示第四张图片
//    QPixmap image4(":/path/to/image4.png");
//    QLabel *label4 = new QLabel();
//    label4->setPixmap(image4);
//    layout->addWidget(label4);

//    // 将水平布局设置为主窗口的布局
//    window->setLayout(layout);

//    // 显示主窗口
//    window->show();

//    return a.exec();
//}
