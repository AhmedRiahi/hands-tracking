#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->e = new Engine();
    this->c = new Configurator();
    this->staticSeg = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    exit(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    this->staticSeg = true;
    this->c->start();
}

void MainWindow::on_pushButton_clicked()
{
    if(this->staticSeg){
        this->e->setSergmentationValues(this->c->lowerY,this->c->upperY,this->c->lowerCR,this->c->upperCR,this->c->lowerCB,this->c->upperCB);
    }
    this->e->start();
}

void MainWindow::on_radioButton_4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir Video"), "/", tr("Video (*.avi *.mov)"));
    this->e->setVideo(fileName.toStdString());
}

void MainWindow::on_radioButton_3_clicked()
{
    this->e->setCam();
}

void MainWindow::on_radioButton_clicked()
{
    this->e->isDynamicSeg(false);
}

void MainWindow::on_radioButton_2_clicked()
{
     this->e->isDynamicSeg(true);
}
