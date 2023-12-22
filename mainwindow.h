#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionWireFrame_triggered();

    void on_actionnonWireFrame_triggered();

    void on_actionclear_triggered();

    void on_actionMap_triggered();

    void on_actionMap2_triggered();

    void on_actionMap3_triggered();

    void on_actionMap4_triggered();

    void on_actionMap5_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
