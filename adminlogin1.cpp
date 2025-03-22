#include "adminlogin1.h"
#include "ui_adminlogin1.h"
#include <QMessageBox>
#include "mainwindow.h"  // Include MainWindow header

Adminlogin1::Adminlogin1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Adminlogin1),
    mainWindow(qobject_cast<MainWindow*>(parent))  // Capture the MainWindow pointer
{
    ui->setupUi(this);

    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/Users/HP/Desktop/deepse.db");

        if (!mydb.open()) {
            qDebug() << "Error connecting to database:" << mydb.lastError().text();
        } else {
            qDebug() << "Database connected successfully in Adminlogin1.";
        }
    } else {
        mydb = QSqlDatabase::database();
    }
}

Adminlogin1::~Adminlogin1()
{
    delete ui;
}

void Adminlogin1::on_pushButton_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if (!mydb.open()) {
        qDebug() << "Failed to open database";
        return;
    }

    QSqlQuery qry;
    qry.prepare("SELECT * FROM adminlogin WHERE username = :username AND password = :password");
    qry.bindValue(":username", username);
    qry.bindValue(":password", password);

    if (qry.exec()) {
        int count = 0;
        while (qry.next()) {
            count++;
        }

        if (count == 1) {
            QMessageBox::information(this, "Login Success", "Username and Password are correct.");

            Adminmenu *adminMenu = new Adminmenu(this);  // Pass 'this' so it can return to Adminlogin1
            adminMenu->show();
            this->hide();  // Hide Adminlogin1 instead of closing it

        } else {
            QMessageBox::warning(this, "Login Failed", "Username or Password is incorrect.");
        }
    } else {
        QMessageBox::critical(this, "Error", "Database query failed: " + qry.lastError().text());
    }
}

// Back Button Functionality: Return to MainWindow


void Adminlogin1::on_back_clicked()
{
    this->close();  // Close Adminlogin1

    if (mainWindow) {
        mainWindow->show();  // Show MainWindow again
    } else {
        qDebug() << "MainWindow pointer is null.";
    }
}

