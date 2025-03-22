#ifndef STUDENTMENU_H
#define STUDENTMENU_H

#include <QMainWindow>
#include "studentmenu1.h"
#include "skillswindow.h"
#include "description.h"
#include "login.h"  // Include login header



namespace Ui {
class StudentMenu;
}

class StudentMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentMenu(studentlogin *loginWindow, QWidget *parent = nullptr); // Accept login pointer
    ~StudentMenu();

    void setStudentDetails(const QString &name, const QString &rollNumber, const QString &semester, const QString &department);

private slots:
    void on_pushButton_clicked();              // Open StudentMenu1
    void on_skillsButton_clicked();            // Open SkillsWindow
    void on_pushButton_description_clicked();  // Open Description Window
    void on_backbutton_clicked();              // Go back to Login

private:
    Ui::StudentMenu *ui;
    studentlogin *loginPage;  // Pointer to login window
    QString studentName, studentRoll, studentSemester, studentDepartment;
    SkillsWindow *skillsWindow = nullptr; // Pointer to SkillsWindow to reuse instance
};

#endif // STUDENTMENU_H

