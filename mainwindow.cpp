#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QString>
#include <math.h>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Impedance Matching Calculator"); //Dat ten cho cua so chuong trinh

    //An cac hinh anh minh hoa cac loai phoi hop tro khang
    ui->L_A->hide();
    ui->L_B->hide();
    ui->L_C->hide();
    ui->L_D->hide();
    ui->Pi_B->hide();
    ui->Pi_D->hide();
    ui->T_A->hide();
    ui->T_C->hide();
    ui->Qinput->hide();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_CalculateButton_clicked()
{
    //Kiem tra xem da nhap het input hay chua, neu chua thi dua ra thong bao
    QMessageBox notFilled (QMessageBox::Information,"Oops!" ,"The input information has not been filled out completely. \n Please fill in all input parameters!",QMessageBox::Ok);
    if(ui->FLineEdit->text()==""||ui->RsLineEdit->text()==""||ui->RLLineEdit->text()==""||((ui->Qinput->text()=="") &&(ui->TypeBox1->currentIndex()!=0)) )
    {
        notFilled.exec();
        return;
    }
    //An cac hinh anh minh hoa cac loai phoi hop tro khang
    ui->L_A->hide();
    ui->L_B->hide();
    ui->L_C->hide();
    ui->L_D->hide();
    ui->Pi_B->hide();
    ui->Pi_D->hide();
    ui->T_A->hide();
    ui->T_C->hide();
    ui->Qoutput->hide();
    //Khai bao, khoi tao cac bien
    double Rs,RL;
    double F;
    double w;
    double Q;
    double L1,L2;
    double C1,C2;
    double Rv; //Gia tri tro khang ao
    F = ui->FLineEdit->text().toDouble()*pow(10,(3*(ui->HzComboBox->currentIndex()))); //Gan gia tri tan so bang gia tri nhap vao
    Rs = ui->RsLineEdit->text().toDouble()*pow(10,(3*(ui->OhmComboBox1->currentIndex())));
    RL = ui->RLLineEdit->text().toDouble()*pow(10,(3*(ui->OhmComboBox2->currentIndex())));
    w = F*2*M_PI; //Tinh tan so goc

    QString type; //Kieu mach phoi hop tro khang
    type = (ui->TypeBox1->currentText())+" "+ (ui->TypeBox2->currentText());

    if((type == "L DC Feed")&&(RL>Rs)){
        Q = sqrt((RL/Rs)-1);
        C1 = Q/(RL*w);
        L1 = (Q*Q)/((Q*Q+1)*C1*w*w);
        ui->LValueLabel->setText("L= "+QString::number(L1)+" H");
        ui->CValueLabel->setText("C= "+QString::number(C1)+" F");
        ui->Qoutput->setText("Q= "+QString::number(Q));
        ui->Qoutput->show();
        ui->L_A->show();
    }
    if((type == "L DC Feed")&&(RL<Rs)){
        Q = sqrt((Rs/RL)-1);
        L1 = Q*RL/w;
        C1 = (Q*Q)/((Q*Q+1)*L1*w*w);
        ui->LValueLabel->setText("L= "+QString::number(L1)+" H");
        ui->CValueLabel->setText("C= "+QString::number(C1)+" F");
        ui->Qoutput->setText("Q= "+QString::number(Q));
        ui->Qoutput->show();
        ui->L_B->show();
    }
    if((type == "L DC Block")&&(RL>Rs)){
        Q = sqrt((RL/Rs)-1);
        L1 = RL/(Q*w);
        C1 = (Q*Q+1)/(Q*Q*L1*w*w);
        ui->LValueLabel->setText("L= "+QString::number(L1)+" H");
        ui->CValueLabel->setText("C= "+QString::number(C1)+" F");
        ui->Qoutput->setText("Q= "+QString::number(Q));
        ui->Qoutput->show();
        ui->L_C->show();
    }
    if((type == "L DC Block")&&(RL<Rs)){
        Q = sqrt((Rs/RL)-1);
        C1 = 1/(Q*RL*w);
        L1 = (Q*Q+1)/(Q*Q*C1*w*w);
        ui->LValueLabel->setText("L= "+QString::number(L1)+" H");
        ui->CValueLabel->setText("C= "+QString::number(C1)+" F");
        ui->Qoutput->setText("Q= "+QString::number(Q));
        ui->Qoutput->show();
        ui->L_D->show();
    }
    if(type == "Pi DC Feed"){
        Q = ui->Qinput->text().toDouble();
        if(RL>Rs){
            Rv = RL/(Q*Q+1);
            C2 = Q/(RL*w);
            L2 = (Q*Rv)/w;
            Q = sqrt((Rs/Rv)-1);
            C1 = Q/(Rs*w);
            L1 = (Q*Rv)/w;
        }
        else{
            Rv = Rs/(Q*Q+1);
            C1 = Q/(Rs*w);
            L1 = (Q*Rv)/w;
            Q = sqrt((RL/Rv)-1);
            C2 = Q/(RL*w);
            L2 = (Q*Rv)/w;
        }
        ui->LValueLabel->setText("Ls= "+QString::number(L1+L2)+" H");
        ui->CValueLabel->setText("Cp1= "+QString::number(C1)+" F" + " Cp2= "+QString::number(C2)+" F");
        ui->Pi_B->show();
    }
    if(type == "Pi DC Block"){
        Q = ui->Qinput->text().toDouble();
        if(RL>Rs){
            Rv = RL/(Q*Q+1);
            C2 = 1/(Rv*Q*w);
            L2 = RL/(Q*w);
            Q = sqrt((Rs/Rv)-1);
            C1 = 1/(Rv*Q*w);
            L1 = Rs/(Q*w);
        }
        else{
            Rv = Rs/(Q*Q+1);
            C1 = 1/(Rv*Q*w);
            L1 = Rs/(Q*w);
            Q = sqrt((RL/Rv)-1);
            C2 = 1/(Rv*Q*w);
            L2 = RL/(Q*w);
        }
        ui->LValueLabel->setText("Lp1= "+QString::number(L1)+" H"+" Lp2= "+QString::number(L2)+" H");
        ui->CValueLabel->setText("Cs= "+QString::number((C1*C2)/(C1+C2))+" F");
        ui->Pi_D->show();
    }
    if(type == "T DC Feed"){
        Q = ui->Qinput->text().toDouble();
        if(RL<Rs){
            Rv = (Q*Q+1)*RL;
            C2 = Q/(Rv*w);
            L2 = (Q*RL)/w;
            Q = sqrt((Rv/Rs)-1);
            C1 = Q/(Rv*w);
            L1 = (Q*Rs)/w;
        }
        else{
            Rv = (Q*Q+1)*Rs;
            C1 = Q/(Rv*w);
            L1 = (Q*Rs)/w;
            Q = sqrt((Rv/RL)-1);
            C2 = Q/(Rv*w);
            L2 = (Q*RL)/w;
        }
        ui->LValueLabel->setText("Ls1= "+QString::number(L1)+" H"+" Ls2= "+QString::number(L2)+" H");
        ui->CValueLabel->setText("Cp= "+QString::number(C1+C2)+" F");
        ui->T_A->show();
    }
    if(type == "T DC Block"){
        Q = ui->Qinput->text().toDouble();
        if(RL<Rs){
            Rv = (Q*Q+1)*RL;
            C2 = 1/(Q*RL*w);
            L2 = Rv/(Q*w);
            Q = sqrt((Rv/Rs)-1);
            C1 = 1/(Q*Rs*w);
            L1 = Rv/(Q*w);
        }
        else{
            Rv = (Q*Q+1)*Rs;
            C1 = 1/(Q*Rs*w);
            L1 = Rv/(Q*w);
            Q = sqrt((Rv/RL)-1);
            C2 = 1/(Q*RL*w);
            L2 = Rv/(Q*w);
        }
        ui->LValueLabel->setText("Lp= "+QString::number((L1*L2)/(L1+L2))+" H");
        ui->CValueLabel->setText("Cs1= "+QString::number(C1)+" F" + " Cs2= "+QString::number(C2)+" F");
        ui->T_C->show();
    }

}


void MainWindow::on_TypeBox1_currentIndexChanged(int index)
{
    if(index == 0)
        ui->Qinput->hide();
    else
        ui->Qinput->show();
}

