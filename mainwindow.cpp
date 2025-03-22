#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "adminlogin1.h"
#include "recruiterlogin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    loginPage(nullptr), adminlogin1(nullptr), recruiterLogin(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete loginPage;
    delete adminlogin1;
    delete recruiterLogin;
}

void MainWindow::on_pushButton_clicked()
{
    hide();
    if (!loginPage) {
        loginPage = new studentlogin(this);  // Corrected class name
    }
    loginPage->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    hide();
    if (!adminlogin1) {
        adminlogin1 = new Adminlogin1(this);  // Corrected class name
    }
    adminlogin1->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    hide();
    if (!recruiterLogin) {
        recruiterLogin = new RecruiterLogin(this);  // Corrected class name
    }
    recruiterLogin->show();
}
