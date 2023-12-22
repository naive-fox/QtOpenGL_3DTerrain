#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionWireFrame_triggered()
{
    ui->openGLWidget->setWireFrame(true);
}

void MainWindow::on_actionnonWireFrame_triggered()
{
    ui->openGLWidget->setWireFrame(false);
}

void MainWindow::on_actionclear_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::None);
}

void MainWindow::on_actionMap_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Map);

}

void MainWindow::on_actionMap2_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Map2);
}

void MainWindow::on_actionMap3_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Map3);
}

void MainWindow::on_actionMap4_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Map4);
}

void MainWindow::on_actionMap5_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Map5);
}