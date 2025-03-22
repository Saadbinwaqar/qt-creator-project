
#include "recruitermenu.h"
#include "ui_recruitermenu.h"
#include "cv.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include "recruiterlogin.h"  // Include RecruiterLogin header

Recruitermenu::Recruitermenu(QWidget *parent, const QString &companyName)
    : QMainWindow(parent), ui(new Ui::Recruitermenu)
{
    ui->setupUi(this);
    setCompanyName(companyName);

    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("C:/Users/HP/Desktop/deepse.db");

    if (!mydb.open()) {
        qDebug() << "Error connecting to database:" << mydb.lastError().text();
    }

    ui->resultsTable->setCornerButtonEnabled(false);

    QHeaderView *header = ui->resultsTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setDefaultAlignment(Qt::AlignCenter);
    ui->resultsTable->verticalHeader()->setVisible(false);
    header->setFixedHeight(60);

    ui->resultsTable->setColumnCount(3);
    ui->resultsTable->setHorizontalHeaderLabels(QStringList() << "Roll Number" << "Name" << "Email");
    ui->resultsTable->resizeColumnsToContents();
    ui->resultsTable->resizeRowsToContents();
    ui->resultsTable->horizontalHeader()->setStretchLastSection(true);

    connect(ui->searchButton, &QPushButton::clicked, this, &Recruitermenu::searchSkills);

    connect(ui->resultsTable, &QTableWidget::cellClicked, this, [this](int row, int col) {
        if (col == 2) {  // Email column
            QTableWidgetItem *emailItem = ui->resultsTable->item(row, col);
            QString email = emailItem->text();
            if (!email.isEmpty()) {
                QDesktopServices::openUrl(QUrl("mailto:" + email));
            }
        } else if (col == 1) {  // Name column
            QTableWidgetItem *nameItem = ui->resultsTable->item(row, col);
            QTableWidgetItem *rollNumberItem = ui->resultsTable->item(row, 0);

            if (nameItem && rollNumberItem) {
                QString name = nameItem->text();
                QString rollNumber = rollNumberItem->text();

                QSqlQuery query;
                query.prepare("SELECT department, semester, email FROM Students WHERE roll_number = :rollNumber");
                query.bindValue(":rollNumber", rollNumber);

                if (query.exec() && query.next()) {
                    QString department = query.value(0).toString();
                    QString semester = query.value(1).toString();
                    QString email = query.value(2).toString();

                    Cv *cv = new Cv(this);
                    cv->show();
                    cv->setStudentDetails(name, rollNumber, department, semester);
                } else {
                    QMessageBox::warning(this, "Database Error", "Failed to fetch student details.");
                }
            }
        }
    });
}

Recruitermenu::~Recruitermenu()
{
    delete ui;
}

void Recruitermenu::setCompanyName(const QString &companyName)
{
    ui->label_companyName->setText(QString("Find Recruits For %1").arg(companyName));
}

void Recruitermenu::searchSkills()
{
    QString skill = ui->skillInput->text().trimmed();

    if (skill.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a skill to search.");
        return;
    }

    ui->resultsTable->setRowCount(0);  // Clear previous results

    QSqlQuery query;
    query.prepare("SELECT Students.roll_number, Students.name, Students.email "
                  "FROM Students "
                  "JOIN Skills ON Students.roll_number = Skills.roll_number "
                  "WHERE LOWER(Skills.skill_name) LIKE LOWER(:skill)");
    query.bindValue(":skill", "%" + skill + "%");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->resultsTable->insertRow(row);
        ui->resultsTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));  // Roll Number
        ui->resultsTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));  // Name

        QTableWidgetItem *emailItem = new QTableWidgetItem(query.value(2).toString());  // Email
        emailItem->setForeground(QBrush(Qt::blue));
        emailItem->setToolTip("Click to send an email");
        ui->resultsTable->setItem(row, 2, emailItem);

        row++;
    }

    if (row == 0) {
        QMessageBox::information(this, "No Results", "No students found with the specified skill.");
    }
}

void Recruitermenu::on_back_clicked()
{
    this->close();
    RecruiterLogin *loginWindow = new RecruiterLogin(this);  // Pass current widget as parent

    loginWindow->show();
}

