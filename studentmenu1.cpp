#include "studentmenu1.h"
#include "ui_studentmenu1.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include "studentmenu.h"
#include <login.h>

StudentMenu1::StudentMenu1(StudentMenu *menu, studentlogin *login, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StudentMenu1), studentMenu(menu), loginPage(login)

{
    ui->setupUi(this);


    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        mydb = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/Users/HP/Desktop/deepse.db");
        if (!mydb.open()) {
            qDebug() << "Error connecting to database:" << mydb.lastError().text();
        }
    }
    ui->tableWidget_grades->setColumnCount(3);
    ui->tableWidget_grades->setHorizontalHeaderLabels({"Subject", "Teacher", "Grade"});


    ui->tableWidget_grades->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget_grades->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_grades->verticalHeader()->setDefaultSectionSize(30);
    ui->tableWidget_grades->horizontalHeader()->setFixedHeight(35);
    ui->tableWidget_grades->verticalHeader()->setVisible(false);
    ui->tableWidget_grades->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_grades->setSelectionMode(QAbstractItemView::NoSelection);

    ui->tableWidget_grades->setShowGrid(false);
    ui->tableWidget_grades->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    QPalette p = ui->tableWidget_grades->palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    ui->tableWidget_grades->setPalette(p);

    ui->tableWidget_grades->setStyleSheet(
        "QTableWidget { background-color: black; color: white; gridline-color: pink; font-size: 14px; border: 2px solid pink; }"
        "QHeaderView::section { background-color: #ff5c93; color: white; font-weight: bold; border: 1px solid pink; padding: 5px; }"
        );
}

StudentMenu1::~StudentMenu1()
{
    delete ui;
}

void StudentMenu1::setRollNumber(const QString &rollNumber) {
    storedRollNumber = rollNumber;
    ui->label_rollNumber->setText("Roll Number: " + rollNumber);
    fetchGrades(storedRollNumber);
}

void StudentMenu1::fetchGrades(const QString &rollNumber) {
    QSqlQuery query;
    query.prepare(
        "SELECT g.subject_name, MAX(g.grade), MAX(s.teacher_name) "
        "FROM Grades g "
        "LEFT JOIN Subjects s ON g.subject_name = s.subject_name "
        "WHERE g.roll_number = :roll_number "
        "GROUP BY g.subject_name"
        );
    query.bindValue(":roll_number", rollNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to fetch grades: " + query.lastError().text());
        return;
    }

    ui->tableWidget_grades->clearContents();
    ui->tableWidget_grades->setRowCount(0);

    int row = 0;
    while (query.next()) {
        QString subjectName = query.value(0).toString();
        QString grade = query.value(1).toString();
        QString teacherName = query.value(2).toString();

        ui->tableWidget_grades->insertRow(row);
        ui->tableWidget_grades->setItem(row, 0, new QTableWidgetItem(subjectName));
        ui->tableWidget_grades->setItem(row, 1, new QTableWidgetItem(teacherName));
        ui->tableWidget_grades->setItem(row, 2, new QTableWidgetItem(grade));
        row++;
    }

    if (row == 0) {
        QMessageBox::information(this, "No Grades", "No grades found for the given roll number.");
    }
}

void StudentMenu1::on_back_clicked()
{
    this->hide();  // Hide StudentMenu1

    if (studentMenu) {
        studentMenu->show();  // Show existing StudentMenu instance
    } else {
        studentMenu = new StudentMenu(loginPage);  // Pass the loginPage instance
        studentMenu->show();
    }
}
