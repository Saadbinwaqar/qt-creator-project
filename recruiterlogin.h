#ifndef RECRUITERLOGIN_H
#define RECRUITERLOGIN_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "signup.h"

class Recruitermenu;  // ✅ Forward declaration of Recruitermenu
class MainWindow;     // ✅ Forward declaration of MainWindow

namespace Ui {
class RecruiterLogin;
}

class RecruiterLogin : public QDialog
{
    Q_OBJECT

public:
    explicit RecruiterLogin(QWidget *parent = nullptr); // Constructor
    ~RecruiterLogin();                                  // Destructor

private slots:
    void on_pushButton_clicked(); // Handles login button click
    void on_signup_clicked();     // Handles signup button click
    void on_back_clicked();       // ✅ Handles back button click (returns to MainWindow)

private:
    Ui::RecruiterLogin *ui;       // Pointer to UI elements
    QSqlDatabase mydb;            // Database connection
    Recruitermenu *recruitermenu; // ✅ Pointer to Recruitermenu
};

#endif // RECRUITERLOGIN_H
