#ifndef RECRUITERMENU_H
#define RECRUITERMENU_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QBrush>
#include <QMessageBox>
#include "recruiterlogin.h"  // Include RecruiterLogin header

namespace Ui {
class Recruitermenu;
}

class Recruitermenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Recruitermenu(QWidget *parent = nullptr, const QString &companyName = "");
    ~Recruitermenu();

    void setCompanyName(const QString &companyName);  // Set the label with the company name

private slots:
    void searchSkills();      // Handles skill search functionality
    void on_back_clicked();   // Handles back button functionality

private:
    Ui::Recruitermenu *ui;    // Pointer to UI elements
    QSqlDatabase mydb;        // Database connection
};

#endif // RECRUITERMENU_H


