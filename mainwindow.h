#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::map;
namespace Ui {
class MainWindow;
}

struct Student{
    QString id;
    QString name;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_freshButton_clicked();

    void on_studentButton_clicked();

    void on_importButton_clicked();

private:
    Ui::MainWindow *ui;
    map<QString, QString> loadCSVFile(QString fileName);
    vector<Student> loadCSVFileVector(QString fileName);
    Student splitCSVLine(QString lineStr);
    vector<Student> calculate(vector<Student>freshData, map<QString, QString>studentData, vector<Student> &collideFresh, vector<Student> &collideStudent, vector<Student> &repeat);
    vector<Student> newStudent(vector<Student>freshData, map<QString, QString>studentData, vector<Student> collideFresh, vector<Student> collideStudent, vector<Student> repeat);
    void importToCSV(QString fileName, vector<Student> import);
};

#endif // MAINWINDOW_H
