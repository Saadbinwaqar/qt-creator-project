#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <QMainWindow>
#include <QtSql>
#include "studentmenu.h"

class StudentMenu; // Forward declaration

namespace Ui {
class Description;
}

class Description : public QMainWindow
{
    Q_OBJECT

public:
    explicit Description(StudentMenu *studentMenu, const QString &rollNumber, QWidget *parent = nullptr);
    ~Description();

private slots:
    void on_back_clicked();
    void saveDescription();

private:
    Ui::Description *ui;
    QSqlDatabase mydb;
    QString studentRollNumber;
    StudentMenu *studentMenu; // Pointer to StudentMenu

    void loadDescription();
};

#endif // DESCRIPTION_H
