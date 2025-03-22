#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QComboBox>

// Forward declare Adminlogin1 to avoid circular dependencies
class Adminlogin1;

namespace Ui {
class Adminmenu;
}

class Adminmenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Adminmenu(Adminlogin1 *loginWindow, QWidget *parent = nullptr);  // Pass Adminlogin1 pointer
    ~Adminmenu();

private slots:
    void fetchStudentDetails();
    void populateSubjects();
    void insertGrade();
    void on_pushbutton_back_clicked();

private:
    Ui::Adminmenu *ui;
    QSqlDatabase mydb;
    QString studentName;
    QString department;
    int semester;
    QString rollNumber;
    Adminlogin1 *adminLogin; // Pointer to Adminlogin1
};

#endif // ADMINMENU_H

