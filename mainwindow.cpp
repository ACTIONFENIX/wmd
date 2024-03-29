#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "errorhandler.h"
#include <QDialog>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("wmd");
    options_dialog = new QDialog(this);
    options_ui.setupUi(options_dialog);
    setupConnections();
    mf.set_location("Downloads");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    QObject::connect(this, SIGNAL(signalUpdateChaptersPalette(unsigned int)), this, SLOT(updateChaptersPalette(unsigned int)));
    QObject::connect(this, SIGNAL(signalErrorOccured(const QString&)), this, SLOT(showError(const QString&)));
    QObject::connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::showOptions);
    QObject::connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    QObject::connect(ui->actionExit, &QAction::triggered, this, exit);
    QObject::connect(options_ui.pushButton, &QPushButton::clicked, [this]()
    {
        auto dir = QFileDialog::getExistingDirectory(options_ui.widget, "Directory", "");
        if (!dir.isEmpty())
        {
            options_ui.label->setText(QString("Download to:\n") + dir);
        }
    });
}

void MainWindow::on_pushButton_clicked()
{
    if (m_downloading.valid() && m_downloading.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
    {
        QMessageBox msg_box;
        msg_box.setText("There is already a downloading. Abort it?");
        msg_box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg_box.setDefaultButton(QMessageBox::Ok);
        if (msg_box.exec() == QMessageBox::Cancel)
        {
            return;
        }
        mf.stop();
        m_downloading.get();
        clear_chapters();
    }
    if (mode == Mode::show_chapters)
    {
        try
        {
            mf.set_url(ui->lineEdit->text().toStdString());
            auto ch_info = mf.get_chapters_info();
            for (auto& i: ch_info)
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
        m_downloading = std::async(std::launch::async, &MainWindow::download_chapters, this);
    }
}

void MainWindow::hide_chapters()
{
    for (auto i: chapters_list)
    {
        i->hide();
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
            if (!mf.is_stopped() && chapters_list[i]->isChecked() && chapters_list[i]->palette().color(QPalette::ColorRole::WindowText) != Qt::green)
            {
                mf.download_chapter(i);
                if (!mf.is_stopped())
                {
                    emit signalUpdateChaptersPalette(i);
                }
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
    //clear_chapters();
    hide_chapters();
}

void MainWindow::updateChaptersPalette(unsigned int i)
{
    if (i < chapters_list.size())
    {
        auto palette = chapters_list[i]->palette();
        palette.setColor(QPalette::ColorRole::Window, Qt::green);
        palette.setColor(QPalette::ColorRole::WindowText, Qt::green);
        palette.setColor(QPalette::ColorRole::Button, Qt::green);
        palette.setColor(QPalette::ColorRole::ButtonText, Qt::green);
        chapters_list[i]->setPalette(palette);
    }
}

void MainWindow::showError(const QString &str)
{
    ErrorHandler err(str.toStdString());
}

void MainWindow::showOptions()
{
    if (options_dialog->exec() == QDialog::Accepted)
    {
        mf.set_location(options_ui.label->text().toStdString().c_str() + std::string("Download to:\n").size());
        mf.set_compressed(options_ui.checkBox->isChecked());
    }
}

void MainWindow::showAbout()
{
    QMessageBox msg_box;
    msg_box.setWindowTitle("About");
    msg_box.setText("WMD - Wind Manga Downloader v0.12\nSource: https://github.com/ACTIONFENIX/wmd");
    msg_box.exec();
}
