#ifndef STUDENTMENU1_H
#define STUDENTMENU1_H

#include <QMainWindow>
#include <QtSql>
#include <login.h>
class StudentMenu;  // ✅ Forward declaration

namespace Ui {
class StudentMenu1;
}

class StudentMenu1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentMenu1(StudentMenu *studentMenu, studentlogin *loginPage, QWidget *parent = nullptr);
     // Pass StudentMenu reference
    ~StudentMenu1();
    void setRollNumber(const QString &rollNumber);

private slots:
    void on_back_clicked();

private:
    Ui::StudentMenu1 *ui;
    QSqlDatabase mydb;
    QString storedRollNumber;
    StudentMenu *studentMenu;  // Pointer to StudentMenu
    void fetchGrades(const QString &rollNumber);
    studentlogin *loginPage;

};

#endif // STUDENTMENU1_H
