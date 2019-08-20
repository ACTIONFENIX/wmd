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
    if (mode == Mode::show_chapters)
    {
        //clear_chapters();
        site = std::unique_ptr<MangaBase>(mf.from_url(ui->lineEdit->text().toStdString()));
        if (site)
        {
            try
            {
                auto ch_info = site->get_chapters_info();
                for (auto i: ch_info)
                {
                    add_chapter(i.fullname);
                }
            }
            catch (std::exception& e)
            {
                ErrorHandler err(e.what());
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
        try
        {
            for (size_t i = 0; i < chapters_list.size(); ++i)
            {
                if (chapters_list[i]->isChecked())
                {
                    site->download_chapters(i, i);
                    auto palette = chapters_list[i]->palette();
                    palette.setColor(QPalette::ColorRole::Window, Qt::green);
                    palette.setColor(QPalette::ColorRole::WindowText, Qt::green);
                    palette.setColor(QPalette::ColorRole::Button, Qt::green);
                    palette.setColor(QPalette::ColorRole::ButtonText, Qt::green);
                    chapters_list[i]->setPalette(palette);
                }
            }
        }
        catch (std::exception& e)
        {
            ErrorHandler err(e.what());
        }
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

void MainWindow::on_lineEdit_textChanged(const QString &)
{
    mode = Mode::show_chapters;
    ui->pushButton->setText("Find");
    clear_chapters();
}
