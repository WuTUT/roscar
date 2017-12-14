#ifndef CARGUI_HPP
#define CARGUI_HPP

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <iostream>
#include "QNode.hpp"

using namespace std;
using namespace Qt;
class Cargui : public QWidget
{
    Q_OBJECT

public:
    Cargui(QWidget *parent = 0);
    ~Cargui();
    
private:

    QWidget* imageWidget;

    QGridLayout* mainLayout;

    QGridLayout* carcontrol_Layout;
    QPushButton* car_forward;
    QPushButton* car_back;
    QPushButton* car_left;
    QPushButton* car_right;

   
    QLabel* hostlabel;
    QLabel* masterlabel;
    QLineEdit* hosturl;
    QLineEdit* masterurl;
    QGridLayout* urlinfo_Layout;
    QPushButton* car_connect;

    QNode control_q;
    

public Q_SLOTS:
    void forward_clicked();
    void back_clicked();
    void left_clicked();
    void right_clicked();
    void connect_clicked();
};

#endif // CARGUI_HPP
