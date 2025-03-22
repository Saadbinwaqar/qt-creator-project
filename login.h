#ifndef LOGIN_H
#define LOGIN_H

#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QDialog>

class MainWindow;  // Forward declaration to avoid circular dependency

namespace Ui {
class studentlogin;
}

class studentlogin : public QDialog
{
    Q_OBJECT

public:
    explicit studentlogin(QWidget *parent = nullptr);
    ~studentlogin();

    QSqlDatabase& getDatabase();  // Getter for `mydb`

private slots:
    void on_pushButton_clicked();  // Handles login
    void on_back_clicked();        // Handles back button

private:
    Ui::studentlogin *ui;
    QSqlDatabase mydb;
};

#endif // LOGIN_H
