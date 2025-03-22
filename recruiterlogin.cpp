#include "recruiterlogin.h"
#include "ui_recruiterlogin.h"
#include "recruitermenu.h"
#include "mainwindow.h"  // ✅ Include MainWindow for back button functionality

// Constructor
RecruiterLogin::RecruiterLogin(QWidget *parent)
    : QDialog(parent), ui(new Ui::RecruiterLogin), recruitermenu(nullptr)
{
    ui->setupUi(this);

    // Initialize database connection
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("C:/Users/HP/Desktop/deepse.db");

    if (!mydb.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to database: " + mydb.lastError().text());
    }
}

// Destructor
RecruiterLogin::~RecruiterLogin()
{
    delete ui;
    if (mydb.isOpen()) {
        mydb.close();
    }
}

void RecruiterLogin::on_pushButton_clicked()
{
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT company FROM recruiterlogin WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        QString companyName = query.value(0).toString();

        // ✅ Create and show Recruitermenu
        recruitermenu = new Recruitermenu(nullptr, companyName);
        recruitermenu->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

void RecruiterLogin::on_signup_clicked()
{
    Signup *signupDialog = new Signup(this);
    signupDialog->setAttribute(Qt::WA_DeleteOnClose);  // Ensure it deletes itself when closed
    signupDialog->show();  // Keep the window open
}

void RecruiterLogin::on_back_clicked()
{
    this->hide();  // Hide the login window

    MainWindow *mainWindow = new MainWindow();  // Create a new MainWindow instance
    mainWindow->show();  // Show the main window
}
