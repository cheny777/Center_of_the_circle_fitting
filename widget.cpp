#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("圆心拟合工具");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::FitCenterByLeastSquares(std::map<int, std::vector<double> > mapPoint, std::vector<double> &centerP, double &radius)
{
    double sumX = 0, sumY = 0;
    double sumXX = 0, sumYY = 0, sumXY = 0;
    double sumXXX = 0, sumXXY = 0, sumXYY = 0, sumYYY = 0;

    for (std::map<int, std::vector<double>>::iterator it = mapPoint.begin(); it != mapPoint.end(); ++it)
    {
        std::vector<double> p = it->second;

        sumX += p[0];
        sumY += p[1];
        sumXX += p[0] * p[0];
        sumYY += p[1] * p[1];
        sumXY += p[0] * p[1];
        sumXXX += p[0] * p[0] * p[0];
        sumXXY += p[0] * p[0] * p[1];
        sumXYY += p[0] * p[1] * p[1];
        sumYYY += p[1] * p[1] * p[1];
    }

    int pCount = mapPoint.size();
    double M1 = pCount * sumXY - sumX * sumY;
    double M2 = pCount * sumXX - sumX * sumX;
    double M3 = pCount * (sumXXX + sumXYY) - sumX * (sumXX + sumYY);
    double M4 = pCount * sumYY - sumY * sumY;
    double M5 = pCount * (sumYYY + sumXXY) - sumY * (sumXX + sumYY);

    double a = (M1 * M5 - M3 * M4) / (M2*M4 - M1 * M1);
    double b = (M1 * M3 - M2 * M5) / (M2*M4 - M1 * M1);
    double c = -(a * sumX + b * sumY + sumXX + sumYY) / pCount;

    //圆心XY 半径
    double xCenter = -0.5*a;
    double yCenter = -0.5*b;
    radius = 0.5 * sqrt(a * a + b * b - 4 * c);
    centerP[0] = xCenter;
    centerP[1] = yCenter;
}

void Widget::on_pushButton_clicked()
{
    m_c.clear();
    m_p.clear();

    QString ss =  ui->textEdit->toPlainText();
    ss.append("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    std::string st = ss.toStdString();
    std::cout<<st<<std::endl;

    //std::stringstream os(st);
    int nums = 0;
    const char *strdata = st.c_str();

        std::string ls;
    while( 1 )
    {
        char s= *strdata;
        ls.push_back(s);
        if (*strdata == '\n')
        {
            if (ls.size()<=3)
            {
                break;
            }

            std::stringstream os(ls);
            double s1=0,s2=0;
            char c1,c2;
            os>>s1>>c1>>s2>>c2;
            if (c1 != ',')
            {
                break;
            }
            qDebug()<<s1<<" "<<s2;

            std::vector<double> fd;
            fd.push_back(s1);
            fd.push_back(s2);
            m_p[nums] = fd;
            nums++;
            ls.clear();
        }
        if (*strdata == '\0')
        {
            if (ls.size()<=3)
            {
                break;
            }
            if (ls[0] == '\0')
            {
                break;;
            }
            std::stringstream os(ls);
            double s1=0,s2=0;
            char c1,c2;
            os>>s1>>c1>>s2>>c2;
            if (c1 != ',')
            {
                break;
            }
            qDebug()<<s1<<" "<<s2;

            std::vector<double> fd;
            fd.push_back(s1);
            fd.push_back(s2);
            m_p[nums] = fd;
            nums++;
            ls.clear();
            break;;
        }

        strdata++;
    }

    m_c.push_back(0);
    m_c.push_back(0);
    FitCenterByLeastSquares(m_p,m_c,m_r);

    ui->label->setText(QString("圆心坐标为 x:%1 ,y:%2").arg(m_c[0]).arg(m_c[1]));
    ui->label_2->setText((QString("半径为：%1").arg(m_r)));
}
