#ifndef CARGUI_H
#define CARGUI_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <iostream>
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
public Q_SLOTS:
    void forward_clicked();
    void back_clicked();
    void left_clicked();
    void right_clicked();
};

#endif // CARGUI_H
