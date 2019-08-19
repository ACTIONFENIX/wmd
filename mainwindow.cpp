#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "errorhandler.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("wmd");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    site = std::unique_ptr<MangaBase>(mf.from_url(ui->lineEdit->text().toStdString()));
    if (site)
    {
        try
        {
            site->download_chapters(0, -1);
        }
        catch (std::exception& e)
        {
            ErrorHandler err(e.what());
        }
    }
    else
    {
        ErrorHandler err("Site is not supported.");
    }
}
