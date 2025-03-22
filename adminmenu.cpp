#include "adminmenu.h"
#include "ui_adminmenu.h"
#include "adminlogin1.h"
#include <QMessageBox>

Adminmenu::Adminmenu(Adminlogin1 *loginWindow, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Adminmenu), adminLogin(loginWindow) // Store reference
{
    ui->setupUi(this);
    QStringList grades = {"A", "B", "C", "D", "F"};
    ui->comboBoxGrade->addItems(grades);

    // Connect database
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        mydb = QSqlDatabase::database();
    } else {
        qDebug() << "Database connection not found!";
    }

    connect(ui->pushButton, &QPushButton::clicked, this, &Adminmenu::insertGrade);
    connect(ui->lineEdit, &QLineEdit::editingFinished, this, &Adminmenu::fetchStudentDetails);
}

Adminmenu::~Adminmenu()
{
    delete ui;
}

void Adminmenu::fetchStudentDetails() {
    rollNumber = ui->lineEdit->text().trimmed();
    if (rollNumber.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a roll number.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT name, department, semester FROM Students WHERE roll_number = :roll_number");
    query.bindValue(":roll_number", rollNumber);

    if (query.exec() && query.next()) {
        studentName = query.value("name").toString();
        department = query.value("department").toString();
        semester = query.value("semester").toInt();

        ui->label_name->setText(studentName);
        ui->label_department->setText(department);
        ui->label_rollno->setText(rollNumber);
        ui->label_semester_2->setText(QString::number(semester));

        populateSubjects();
    } else {
        QMessageBox::warning(this, "Error", "Student not found.");
    }
}

void Adminmenu::populateSubjects() {
    ui->comboBoxSubject->clear();

    if (department.isEmpty() || semester <= 0) {
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT subject_name FROM Subjects WHERE department = :department AND semester = :semester");
    query.bindValue(":department", department);
    query.bindValue(":semester", semester);

    if (query.exec()) {
        while (query.next()) {
            ui->comboBoxSubject->addItem(query.value("subject_name").toString());
        }
    }
}

void Adminmenu::insertGrade() {
    QString subject = ui->comboBoxSubject->currentText();
    QString grade = ui->comboBoxGrade->currentText();

    if (subject.isEmpty() || grade.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a subject and enter a grade.");
        return;
    }

    // Check if a grade already exists for this subject and student
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT grade FROM Grades WHERE roll_number = :roll AND subject_name = :subject");
    checkQuery.bindValue(":roll", rollNumber);
    checkQuery.bindValue(":subject", subject);

    if (!checkQuery.exec()) {
        QMessageBox::warning(this, "Error", "Failed to check existing grade.");
        return;
    }

    if (checkQuery.next()) { // If a grade already exists
        QString existingGrade = checkQuery.value(0).toString();
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Grade Exists",
                                      "A grade (" + existingGrade + ") already exists for this subject. Do you want to update it?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Update existing grade
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE Grades SET grade = :grade WHERE roll_number = :roll AND subject_name = :subject");
            updateQuery.bindValue(":grade", grade);
            updateQuery.bindValue(":roll", rollNumber);
            updateQuery.bindValue(":subject", subject);

            if (updateQuery.exec()) {
                QMessageBox::information(this, "Success", "Grade updated successfully!");
            } else {
                QMessageBox::warning(this, "Error", "Failed to update grade.");
            }
        }
        return;
    }

    // If no existing grade, insert a new one
    QSqlQuery query;
    query.prepare("INSERT INTO Grades (roll_number, student_name, subject_name, grade) VALUES (:roll, :name, :subject, :grade)");
    query.bindValue(":roll", rollNumber);
    query.bindValue(":name", studentName);
    query.bindValue(":subject", subject);
    query.bindValue(":grade", grade);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Grade inserted successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to insert grade.");
    }
}


void Adminmenu::on_pushbutton_back_clicked()
{    if (adminLogin) {
        adminLogin->show();  // Show the login window again
    }
    this->hide();  // Hide Adminmenu instead of closing it

}


