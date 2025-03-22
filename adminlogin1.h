#ifndef ADMINLOGIN1_H
#define ADMINLOGIN1_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <adminmenu.h>

class MainWindow;  // Forward declaration of MainWindow

namespace Ui {
class Adminlogin1;
}

class Adminlogin1 : public QDialog
{
    Q_OBJECT

public:
    explicit Adminlogin1(QWidget *parent = nullptr);
    ~Adminlogin1();

private slots:
    void on_pushButton_clicked();
    void on_back_clicked();  // Slot for back button functionality

private:
    Ui::Adminlogin1 *ui;
    QSqlDatabase mydb;
    Adminmenu *adminmenu;
    MainWindow *mainWindow;  // Pointer to MainWindow for back navigation
};

#endif // ADMINLOGIN1_H
