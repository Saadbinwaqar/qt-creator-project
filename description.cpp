#include "description.h"
#include "ui_description.h"
#include "studentmenu.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QMessageBox>

Description::Description(StudentMenu *menu, const QString &rollNumber, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Description), studentMenu(menu), studentRollNumber(rollNumber)
{
    ui->setupUi(this);


    // Use the existing database connection
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        mydb = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        qDebug() << "Database connection not found!";
    }

    loadDescription();
    connect(ui->saveButton, &QPushButton::clicked, this, &Description::saveDescription);
}

Description::~Description()
{
    delete ui;
}

void Description::loadDescription()
{
    QSqlQuery query;
    query.prepare("SELECT description FROM Students WHERE roll_number = :rollNumber");
    query.bindValue(":rollNumber", studentRollNumber);

    if (query.exec() && query.next()) {
        QString description = query.value(0).toString();
        ui->textEdit_description->setPlainText(description);
    } else {
        qDebug() << "Failed to fetch description:" << query.lastError().text();
    }
}

void Description::saveDescription()
{
    QString newDescription = ui->textEdit_description->toPlainText();
    QSqlQuery query;
    query.prepare("UPDATE Students SET description = :description WHERE roll_number = :rollNumber");
    query.bindValue(":description", newDescription);
    query.bindValue(":rollNumber", studentRollNumber);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Description updated successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to update description: " + query.lastError().text());
    }
}

void Description::on_back_clicked()
{
    this->hide();
    qDebug() << "Description window hidden, returning to StudentMenu.";

    if (studentMenu) {
        studentMenu->show();
        qDebug() << "StudentMenu displayed.";
    } else {
        qDebug() << "Error: StudentMenu instance is null. Creating new instance.";
        studentMenu = new StudentMenu(nullptr);
        studentMenu->show();
    }
}

