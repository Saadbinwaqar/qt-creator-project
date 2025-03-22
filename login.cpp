#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include "studentmenu.h"
#include <QDebug>
#include "mainwindow.h"

studentlogin::studentlogin(QWidget *parent)
    : QDialog(parent), ui(new Ui::studentlogin)
{
    ui->setupUi(this);

    // Check if the database connection exists
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        mydb = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        mydb = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        mydb.setDatabaseName("C:/Users/HP/Desktop/deepse.db");

        if (!mydb.open()) {
            qDebug() << "Error connecting to database:" << mydb.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to connect to database.");
        } else {
            qDebug() << "Database connected successfully!";
        }
    }
}

studentlogin::~studentlogin()
{
    delete ui;
}

QSqlDatabase& studentlogin::getDatabase() {
    return mydb;
}
void studentlogin::on_pushButton_clicked()
{
    if (!mydb.isOpen()) {
        qDebug() << "Database was closed. Reopening...";
        if (!mydb.open()) {
            qDebug() << "Failed to open database:" << mydb.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to open database.");
            return;
        }
    }

    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    QSqlQuery qry;
    qry.prepare("SELECT user_id FROM Users WHERE username = :username AND password = :password");
    qry.bindValue(":username", username);
    qry.bindValue(":password", password);

    if (!qry.exec()) {
        qDebug() << "Query execution failed:" << qry.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to execute query.");
        return;
    }

    if (qry.next()) {
        QString user_id = qry.value(0).toString();
        qDebug() << "Login successful! User ID:" << user_id;

        QSqlQuery studentQry;
        studentQry.prepare("SELECT name, roll_number, semester, department FROM Students WHERE user_id = :user_id");
        studentQry.bindValue(":user_id", user_id);

        if (!studentQry.exec()) {
            qDebug() << "Student query failed:" << studentQry.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to retrieve student details.");
            return;
        }

        if (studentQry.next()) {
            QString name = studentQry.value(0).toString();
            QString rollNumber = studentQry.value(1).toString();
            QString semester = studentQry.value(2).toString();
            QString department = studentQry.value(3).toString();
            qDebug() << "Student found: " << name << rollNumber << semester << department;

            // Open the StudentMenu window and pass the details
            StudentMenu *studentMenu = new StudentMenu(this);
            studentMenu->setStudentDetails(name, rollNumber, semester, department);
            studentMenu->show();

            this->hide();
        } else {
            QMessageBox::warning(this, "Error", "Student details not found.");
        }
    } else {
        qDebug() << "Login failed: Invalid username or password.";
        QMessageBox::warning(this, "Login Failed", "Username or Password is incorrect.");
    }
}

void studentlogin::on_back_clicked()
{
    this->hide();  // Hide the login window

    MainWindow *mainWindow = new MainWindow();  // Create new MainWindow instance
    mainWindow->show();  // Show the main window
}

