#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QGridLayout* gridLayout;
    QVBoxLayout* layout1;
    QVBoxLayout* layout2;
    QVBoxLayout* layout3;
    QVBoxLayout* layout4;
    QLabel* label1;
    QLabel* label2;
    QLabel* label3;
    QLabel* label4;
};

#endif // MAINWINDOW_H
