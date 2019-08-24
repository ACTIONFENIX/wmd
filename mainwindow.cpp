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
    QObject::connect(this, SIGNAL(signalUpdateChaptersPalette(unsigned int)), this, SLOT(updateChaptersPalette(unsigned int)));
    QObject::connect(this, SIGNAL(signalErrorOccured(const QString&)), this, SLOT(showError(const QString&)));
    QObject::connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    QObject::connect(ui->actionExit, &QAction::triggered, this, exit);
}

MainWindow::~MainWindow()
{
    th.detach();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (mode == Mode::show_chapters)
    {
        site = std::unique_ptr<MangaBase>(mf.from_url(ui->lineEdit->text().toStdString()));
        if (site)
        {
            try
            {
                site->set_location("Downloads");
                auto ch_info = site->get_chapters_info();
                for (auto i: ch_info)
                {
                    add_chapter(i.fullname);
                }
            }
            catch (std::exception& e)
            {
                ErrorHandler err(e.what());
                return;
            }
            mode = Mode::download_chapters;
            ui->pushButton->setText("Download");
        }
        else
        {
            ErrorHandler err("Site is not supported.");
        }
    }
    else
    {
        th.detach();
        th = std::thread(&MainWindow::download_chapters, this);
    }
}

void MainWindow::clear_chapters()
{
    for (auto i: chapters_list)
    {
        i->setParent(nullptr);
        delete i;
    }
    chapters_list.clear();
}

void MainWindow::add_chapter(const std::string &chapter_name)
{
    chapters_list.push_back(new QCheckBox(chapter_name.c_str()));
    ui->scrollAreaWidgetContents->layout()->addWidget(chapters_list.back());
}

void MainWindow::download_chapters()
{
    try
    {
        for (size_t i = 0; i < chapters_list.size(); ++i)
        {
            if (chapters_list[i]->isChecked())
            {
                site->download_chapter(i);
                emit signalUpdateChaptersPalette(i);
            }
        }
    }
    catch (std::exception& e)
    {
        emit signalErrorOccured(QString(e.what()));
    }
}

void MainWindow::on_lineEdit_textChanged(const QString &)
{
    mode = Mode::show_chapters;
    ui->pushButton->setText("Find");
    clear_chapters();
}

void MainWindow::updateChaptersPalette(unsigned int i)
{
    auto palette = chapters_list[i]->palette();
    palette.setColor(QPalette::ColorRole::Window, Qt::green);
    palette.setColor(QPalette::ColorRole::WindowText, Qt::green);
    palette.setColor(QPalette::ColorRole::Button, Qt::green);
    palette.setColor(QPalette::ColorRole::ButtonText, Qt::green);
    chapters_list[i]->setPalette(palette);
    chapters_list[i]->setDown(false);
}

void MainWindow::showError(const QString &str)
{
    ErrorHandler err(str.toStdString());
}

void MainWindow::showAbout()
{
    QMessageBox msg_box;
    msg_box.setWindowTitle("About");
    msg_box.setText("WMD - Wind Manga Downloader v0.1\nSource: https://github.com/ACTIONFENIX/wmd");
    msg_box.exec();
}
