#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <math.h>
#include <map>
#include <vector>
#include <QDebug>
#include <iostream>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    void FitCenterByLeastSquares(std::map<int, std::vector<double>> mapPoint, std::vector<double> &centerP, double &radius);


    /*半径*/
    double m_r;
    /*圆心*/
    std::vector<double> m_c;
    /*数据*/
    std::map<int, std::vector<double>> m_p;
};
#endif // WIDGET_H
