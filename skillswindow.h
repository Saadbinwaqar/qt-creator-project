#ifndef SKILLSWINDOW_H
#define SKILLSWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>

class StudentMenu;  // ✅ Forward declaration (No #include "studentmenu.h" here)

namespace Ui {
class SkillsWindow;
}

class SkillsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SkillsWindow(StudentMenu *studentMenu, QWidget *parent = nullptr);
    ~SkillsWindow();
    void setRollNumber(const QString &rollNumber); // Setter function

private slots:
    void on_addSkillButton_clicked();
    void on_removeSkillButton_clicked();
    void on_back_clicked();

private:
    Ui::SkillsWindow *ui;
    QSqlDatabase mydb;
    QString rollNumber;
    StudentMenu *studentMenu;  // Pointer to the StudentMenu instance
    void loadSkills();
    void initializeDatabase();
    void connectButtons();
};

#endif // SKILLSWINDOW_H
