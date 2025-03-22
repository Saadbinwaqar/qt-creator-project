#ifndef SIGNUP_H
#define SIGNUP_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class Signup;
}

class Signup : public QMainWindow
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

private slots:
    void on_registerButton_clicked();


private:
    Ui::Signup *ui;
    QSqlDatabase mydb;
};

#endif // SIGNUP_H
