#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include "mainwindow.h"

#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include <inttypes.h>
#include <stack>
using namespace QtCharts;

using namespace std;

struct Node

{

    char key[10000];
    char type ;
    unsigned long long size;
    Node * prev;
    vector<Node *> next;

};

namespace Ui {

class MainWindow;

}


class MainWindow : public QMainWindow

{

    Q_OBJECT


public:

    explicit MainWindow(QWidget *parent = 0);

    unsigned long long getDirSize(char* , struct Node*);

    vector<Node*> Traversal(Node * );

    void   DrawPieChart(Node*);


    ~MainWindow();


private slots:

    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    vector<Node*> temp ;
    Ui::MainWindow *ui;
    QPieSeries *series;
    stack <Node*> Back ;
    int counter ;
};


#endif // MAINWINDOW_H
