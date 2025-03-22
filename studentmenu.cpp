#include "studentmenu.h"
#include "ui_studentmenu.h"
#include "login.h"
#include "studentmenu1.h"
#include <QMessageBox>

StudentMenu::StudentMenu(studentlogin *loginWindow, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StudentMenu), loginPage(loginWindow), skillsWindow(nullptr)
{
    ui->setupUi(this);
}

StudentMenu::~StudentMenu()
{
    delete ui;
    if (skillsWindow) {
        delete skillsWindow;
    }
}

void StudentMenu::setStudentDetails(const QString &name, const QString &rollNumber, const QString &semester, const QString &department)
{
    ui->label_name->setText(name);
    ui->label_rollNumber->setText(rollNumber);
    ui->label_semester->setText(semester);
    ui->label_department->setText(department);
}

void StudentMenu::on_pushButton_clicked()
{
    QString rollNumber = ui->label_rollNumber->text();
    StudentMenu1 *studentMenu1 = new StudentMenu1(this, loginPage);
    studentMenu1->setRollNumber(rollNumber);
    this->hide();
    studentMenu1->show();
}

void StudentMenu::on_skillsButton_clicked()
{
    QString rollNumber = ui->label_rollNumber->text();
    qDebug() << "Opening SkillsWindow with roll number:" << rollNumber;

    // Create SkillsWindow only if it doesn't already exist
    if (!skillsWindow) {
        skillsWindow = new SkillsWindow(this);
        qDebug() << "SkillsWindow created.";
    }

    // Set the roll number and show the window
    skillsWindow->setRollNumber(rollNumber);
    qDebug() << "Roll number set in SkillsWindow.";

    skillsWindow->show();
    this->hide();
    qDebug() << "StudentMenu hidden, SkillsWindow should be visible.";
}


void StudentMenu::on_pushButton_description_clicked()
{
    QString rollNumber = ui->label_rollNumber->text();
    Description *description = new Description(this, rollNumber);
    hide();
    description->show();
}

void StudentMenu::on_backbutton_clicked()
{
    this->hide();

    if (loginPage) {
        QSqlDatabase &db = loginPage->getDatabase();
        if (!db.isOpen()) {
            qDebug() << "Database was closed. Reopening...";
            if (!db.open()) {
                qDebug() << "Failed to open database:" << db.lastError().text();
                QMessageBox::critical(this, "Database Error", "Failed to reconnect to the database.");
            }
        }
        loginPage->show();
    } else {
        loginPage = new studentlogin();
        loginPage->show();
    }
}


