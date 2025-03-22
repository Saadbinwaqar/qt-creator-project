#include "skillswindow.h"
#include "ui_skillswindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QSignalBlocker>
#include "studentmenu.h"

SkillsWindow::SkillsWindow(StudentMenu *menu, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SkillsWindow), studentMenu(menu)
{
    ui->setupUi(this);

    // Initialize the database connection
    initializeDatabase();

    // Connect UI buttons to their respective slots
    connectButtons();

    qDebug() << "SkillsWindow initialized.";
}

SkillsWindow::~SkillsWindow()
{
    delete ui;
    qDebug() << "SkillsWindow destroyed.";
}

void SkillsWindow::initializeDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        mydb = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/Users/HP/Desktop/deepse.db");

        if (!mydb.open()) {
            qDebug() << "Error connecting to database:" << mydb.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to connect to database.");
        }
    }
}

void SkillsWindow::connectButtons()
{
    // Prevent duplicate connections
    QObject::disconnect(ui->addSkillButton, nullptr, this, nullptr);
    QObject::disconnect(ui->removeSkillButton, nullptr, this, nullptr);
    QObject::disconnect(ui->back, nullptr, this, nullptr);

    // Connect buttons
    connect(ui->addSkillButton, &QPushButton::clicked, this, &SkillsWindow::on_addSkillButton_clicked);
    connect(ui->removeSkillButton, &QPushButton::clicked, this, &SkillsWindow::on_removeSkillButton_clicked);
    connect(ui->back, &QPushButton::clicked, this, &SkillsWindow::on_back_clicked);

    qDebug() << "Buttons connected successfully.";
}

void SkillsWindow::setRollNumber(const QString &rollNumber)
{
    this->rollNumber = rollNumber;
    qDebug() << "SkillsWindow received roll number:" << rollNumber;

    if (!mydb.isOpen()) {
        qDebug() << "Database is NOT open. Trying to reopen...";
        if (!mydb.open()) {
            qDebug() << "Failed to open database:" << mydb.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to open database.");
            return;
        }
        qDebug() << "Database reopened successfully.";
    }

    loadSkills();
}

void SkillsWindow::loadSkills()
{
    ui->listWidget->clear();
    ui->listWidget->blockSignals(true); // Prevent signals from triggering while loading

    qDebug() << "Loading skills for roll number:" << rollNumber;

    QSqlQuery query;
    query.prepare("SELECT skill_name FROM skills WHERE roll_number = :roll_number");
    query.bindValue(":roll_number", rollNumber);

    if (!query.exec()) {
        qDebug() << "Database Error: Failed to load skills: " << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to load skills.");
        return;
    }

    while (query.next()) {
        QString skill = query.value(0).toString();
        ui->listWidget->addItem(skill);
        qDebug() << "Loaded skill:" << skill;
    }

    ui->listWidget->clearSelection();
    ui->listWidget->blockSignals(false);
}

void SkillsWindow::on_addSkillButton_clicked()
{
    QString skill = ui->lineEdit->text().trimmed();
    qDebug() << "Entered skill:" << skill << "| Length:" << skill.length();

    if (skill.isEmpty()) {
        qDebug() << "Warning: Skill input is empty!";
        QMessageBox::warning(this, "Input Error", "Please enter a skill.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM skills WHERE roll_number = :roll_number AND skill_name = :skill_name");
    checkQuery.bindValue(":roll_number", rollNumber);
    checkQuery.bindValue(":skill_name", skill);

    if (checkQuery.exec() && checkQuery.next()) {
        qDebug() << "Warning: Skill already exists.";
        QMessageBox::warning(this, "Duplicate Skill", "This skill already exists.");
        return;
    }

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO skills (roll_number, skill_name) VALUES (:roll_number, :skill_name)");
    insertQuery.bindValue(":roll_number", rollNumber);
    insertQuery.bindValue(":skill_name", skill);

    if (!insertQuery.exec()) {
        qDebug() << "Error inserting skill:" << insertQuery.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to add skill.");
        return;
    }

    qDebug() << "Skill successfully added.";
    ui->lineEdit->clear();
    loadSkills();
}

void SkillsWindow::on_removeSkillButton_clicked()
{
    QSignalBlocker blocker(ui->removeSkillButton); // Prevent multiple triggers

    QListWidgetItem *selectedItem = ui->listWidget->currentItem();
    if (!selectedItem) {
        qDebug() << "No skill selected.";
        QMessageBox::warning(this, "Selection Error", "Please select a skill to remove.");
        return;
    }

    QString skill = selectedItem->text();
    qDebug() << "Selected skill for deletion:" << skill;

    if (QMessageBox::question(this, "Confirm Deletion",
                              "Are you sure you want to remove: " + skill + "?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        qDebug() << "Deletion canceled.";
        return;
    }

    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM skills WHERE roll_number = :roll_number AND skill_name = :skill_name");
    deleteQuery.bindValue(":roll_number", rollNumber);
    deleteQuery.bindValue(":skill_name", skill);

    if (!deleteQuery.exec()) {
        qDebug() << "Error deleting skill:" << deleteQuery.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to remove skill.");
        return;
    }

    delete ui->listWidget->takeItem(ui->listWidget->row(selectedItem));
    qDebug() << "Skill successfully removed.";
    QMessageBox::information(this, "Success", "Skill removed successfully.");
}

void SkillsWindow::on_back_clicked()
{
    this->hide();
    qDebug() << "SkillsWindow hidden, returning to StudentMenu.";

    if (studentMenu) {
        studentMenu->show();
        qDebug() << "StudentMenu displayed.";
    } else {
        qDebug() << "Error: StudentMenu instance is null. Creating new instance.";
        studentMenu = new StudentMenu(nullptr);
        studentMenu->show();
    }
}
