#include "cv.h"
#include "ui_cv.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Cv::Cv(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Cv)
{
    ui->setupUi(this);
    ui->textEditDescription->setReadOnly(true);

    // Setup the table for skills
    ui->skillsTable->setColumnCount(1);
    ui->skillsTable->horizontalHeader()->setVisible(false);  // Hide the column header
    ui->skillsTable->verticalHeader()->setVisible(false);    // Hide row numbers
    ui->skillsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->skillsTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->skillsTable->setFocusPolicy(Qt::NoFocus);
    ui->skillsTable->setShowGrid(false); // Optional: Hide grid lines for a cleaner look
    ui->skillsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

Cv::~Cv()
{
    delete ui;
}

void Cv::setStudentDetails(const QString &name, const QString &rollNumber, const QString &department, const QString &semester)
{
    ui->labelName->setText(name);
    ui->labelRollNumber->setText(rollNumber);
    ui->labelDepartment->setText(department);
    ui->labelSemester->setText(semester);

    // Load and display the skills
    loadSkills(rollNumber);

    loadDescription(rollNumber);
}

void Cv::loadSkills(const QString &rollNumber)
{
    ui->skillsTable->setRowCount(0);  // Clear previous data

    QSqlQuery query;
    query.prepare("SELECT skill_name FROM Skills WHERE roll_number = :rollNumber");
    query.bindValue(":rollNumber", rollNumber);

    if (!query.exec()) {
        qDebug() << "Error fetching skills:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        QString skill = query.value(0).toString();
        ui->skillsTable->insertRow(row);
        ui->skillsTable->setItem(row, 0, new QTableWidgetItem(skill));
        row++;
    }

    if (row == 0) {
        qDebug() << "No skills found for roll number:" << rollNumber;
    }
}

void Cv::loadDescription(const QString &rollNumber)
{
    QSqlQuery query;
    query.prepare("SELECT description FROM Students WHERE roll_number = :rollNumber");
    query.bindValue(":rollNumber", rollNumber);

    if (!query.exec()) {
        qDebug() << "Error fetching description:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        QString description = query.value(0).toString();
        ui->textEditDescription->setPlainText(description);
    } else {
        qDebug() << "No description found for roll number:" << rollNumber;
        ui->textEditDescription->setPlainText("No description available.");
    }
}
