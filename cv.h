#ifndef CV_H
#define CV_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QComboBox>

namespace Ui {
class Cv;
}

class Cv : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cv(QWidget *parent = nullptr);
    ~Cv();
    void setStudentDetails(const QString &name, const QString &rollNumber, const QString &department, const QString &semester);
private:
    Ui::Cv *ui;
    void loadSkills(const QString &rollNumber);  // Function to fetch and display skills
    void loadDescription(const QString &rollNumber);
    QSqlDatabase mydb;
};

#endif // CV_H
