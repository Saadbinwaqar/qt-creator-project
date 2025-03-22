#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>
#include <QDebug>

Signup::Signup(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Signup)
{
    ui->setupUi(this);

    // Initialize database connection
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("C:/Users/HP/Desktop/deepse.db");

    if (!mydb.open()) {
        qDebug() << "Error connecting to database:" << mydb.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to connect to database.");
    }
}

Signup::~Signup()
{
    delete ui;
}

void Signup::on_registerButton_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    QString company = ui->lineEdit_company->text().trimmed();
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    // Validate input fields
    if (name.isEmpty() || company.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    // Check if username already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT username FROM recruiterlogin WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Error", "Username already exists. Please choose a different username.");
        return;
    }

    // Insert new recruiter with fixed role as "Recruiter"
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO recruiterlogin (name, company, username, password, role) VALUES (:name, :company, :username, :password, :role)");
    insertQuery.bindValue(":name", name);
    insertQuery.bindValue(":company", company);
    insertQuery.bindValue(":username", username);
    insertQuery.bindValue(":password", password);
    insertQuery.bindValue(":role", "Recruiter"); // Fixed role

    if (!insertQuery.exec()) {
        qDebug() << "Error inserting into recruiterlogin table: " << insertQuery.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to register account: " + insertQuery.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Account registered successfully! You can now log in.");

    // Close signup window and return to login
    this->close();
}
