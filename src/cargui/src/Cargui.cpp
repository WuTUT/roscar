#include "../include/cargui/Cargui.hpp"

Cargui::Cargui(QWidget *parent)
    : QWidget(parent)
{
    car_forward=new QPushButton();
    car_forward->setText("Forward");
    car_back=new QPushButton();
    car_back->setText("Back");
    car_left=new QPushButton();
    car_left->setText("Left");
    car_right=new QPushButton();
    car_right->setText("Right");

    QObject::connect(car_forward,SIGNAL(clicked()),this,SLOT(forward_clicked()));
    QObject::connect(car_back,SIGNAL(clicked()),this,SLOT(back_clicked()));
    QObject::connect(car_left,SIGNAL(clicked()),this,SLOT(left_clicked()));
    QObject::connect(car_right,SIGNAL(clicked()),this,SLOT(right_clicked()));
    carcontrol_Layout=new QGridLayout();
    carcontrol_Layout->addWidget(car_forward,0,1,1,1);
    carcontrol_Layout->addWidget(car_left,1,0,1,1);
    carcontrol_Layout->addWidget(car_back,1,1,1,1);
    carcontrol_Layout->addWidget(car_right,1,2,1,1);
    mainLayout=new QGridLayout(this);

    imageWidget=new QWidget();
    mainLayout->setMargin(0);
    mainLayout->addWidget(imageWidget,0,0,5,5);
    mainLayout->addLayout(carcontrol_Layout,4,4,2,2);
     this->setLayout(mainLayout);
    setMinimumSize(1500,1000);
    setMaximumSize(1500,1000);
}

void Cargui::forward_clicked(){
    cout<<'f'<<endl;
}
void Cargui::back_clicked(){
    cout<<'b'<<endl;
}
void Cargui::right_clicked(){
    cout<<'r'<<endl;
}
void Cargui::left_clicked(){
    cout<<'l'<<endl;
}

Cargui::~Cargui()
{

}