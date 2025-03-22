#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <login.h>
#include <adminlogin1.h>
#include <recruiterlogin.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    studentlogin *loginPage;
    Adminlogin1 *adminlogin1;
    RecruiterLogin *recruiterLogin;
};

#endif // MAINWINDOW_H
