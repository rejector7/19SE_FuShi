#pragma warning (disable:4819)
//for unicode
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "iostream"
#include "QFileDialog"
#include "QTextStream"
#include "QFile"
#include "QStringList"
#include "vector"
#include "map"
#include "string"
#include "fstream"
/*
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <QGridLayout>
#include <QPushButton>
*/

using std::string;
using namespace::std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_freshButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Csv files(*.csv)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.exec();

    QString freshFileName=dialog.selectedFiles().first();
    ui->freshFileName->setText(freshFileName);
}

void MainWindow::on_studentButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Csv files(*.csv)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.exec();

    QString studentFileName=dialog.selectedFiles().first();
    ui->studentFileName->setText(studentFileName);
}



void MainWindow::on_importButton_clicked()
{
    QString freshFileName=ui->freshFileName->text();
    QString studentFileName=ui->studentFileName->text();

    vector<Student> freshData=loadCSVFileVector(freshFileName);
   // vector<Student> studentData=loadCSVFile(studentFileName);
    map<QString, QString> studentData=loadCSVFile(studentFileName);

    vector<Student> collideFresh;
    vector<Student> collideStudent;
    vector<Student> repeat;
    vector<Student> import=calculate(freshData, studentData, collideFresh, collideStudent, repeat);

    importToCSV(studentFileName, import);


    QString fresh;
    QString student;
    for(unsigned int i=0;i<repeat.size();++i)
    {
        fresh=fresh+repeat[i].id+" "+repeat[i].name+"\n";
    }
    student=fresh;
    for(unsigned int i=0;i<collideFresh.size();++i)
    {
        fresh=fresh+collideFresh[i].id+" "+collideFresh[i].name+"\n";
    }
    for(unsigned int i=0;i<collideStudent.size();++i)
    {
        student=student+collideStudent[i].id+" "+collideStudent[i].name+"\n";
    }
    ui->freshEdit->setText(fresh);
    ui->studentEdit->setText(student);

    QString info;
    info=info+"FreshFile:"+freshFileName+" and studentFileName:"+studentFileName+":\n";

    qlonglong total=qlonglong(repeat.size()+collideFresh.size());

    QString line1=QString("repeat or collide: %1 people\n").arg(total);
    QString line2=QString("repeat: %1 people\n").arg(repeat.size());
    QString line3=QString("collide: %1 people\n").arg(collideFresh.size());
    qlonglong succeed=qlonglong(freshData.size())-total;
    QString line4=QString("repeat: %1 people\n").arg(succeed);
    info=info+line1+line2+line3+line4;
    ui->textBrowser->setText(info);

}
map<QString, QString> MainWindow::loadCSVFile(QString fileName)
{
    QFile dataFile(fileName);
    dataFile.open(QIODevice::ReadWrite);

    map<QString, QString> dataMap;
    //dataVector.resize(1000);
    QTextStream dataStream(&dataFile);

    QStringList qs;
    Student temp;
    for(QString lineStr;!dataStream.atEnd();)
    {
        lineStr=dataStream.readLine();
        temp=splitCSVLine(lineStr);
        dataMap[temp.id]=temp.name;
    }
    dataFile.close();


    return dataMap;
}

vector<Student> MainWindow::loadCSVFileVector(QString fileName)
{
    QFile dataFile(fileName);
    dataFile.open(QIODevice::ReadWrite);

    vector<Student> dataVector;
    //dataVector.resize(1000);
    QTextStream dataStream(&dataFile);

    QStringList qs;
    for(QString lineStr;!dataStream.atEnd();)
    {
        lineStr=dataStream.readLine();
        dataVector.push_back(splitCSVLine(lineStr));

    }
    dataFile.close();


    return dataVector;
}

Student MainWindow::splitCSVLine(QString lineStr)
{
    QStringList lineStrList=lineStr.split(",");
    Student oneStudent;
    oneStudent.id=lineStrList[0];
    oneStudent.name=lineStrList[1];

    return oneStudent;
}

vector<Student> MainWindow::calculate(vector<Student>freshData, map<QString, QString>studentData, vector<Student> &collideFresh, vector<Student> &collideStudent, vector<Student> &repeat)
{
    QString tempId;
    Student tempStudent;
    vector<Student> newStudent;
    for(unsigned int i=0;i<freshData.size();++i)
    {
        tempId=freshData[i].id;
        if(studentData.find(tempId)!=studentData.end())
        {
            tempStudent.id=tempId;
            tempStudent.name=freshData[i].name;
            if(freshData[i].name==studentData[tempId])
            {
                repeat.push_back(tempStudent);
            }
            else
            {
                collideFresh.push_back(tempStudent);
                tempStudent.name=studentData[tempId];
                collideStudent.push_back(tempStudent);
            }
        }
        else
        {
            newStudent.push_back(freshData[i]);
        }
    }
    return freshData;
}


void MainWindow::importToCSV(QString fileName, vector<Student>import)
{
    QFile dataFile(fileName);
    dataFile.open(QIODevice::ReadWrite);

    //vector<Student> dataVector;
    //dataVector.resize(1000);
    QDataStream dataStream(&dataFile);
    QString temp;
    QChar* buf;
    /*
    for(unsigned int i=0;i<import.size();++i)
    {
        temp=import[i].id+","+import[i].name+"\n";
        temp.setRawData(buf,temp.length());
        dataStream.writeRawData(temp.length());
    }
    */
    dataFile.close();


    return;
}
