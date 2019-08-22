#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <memory>
#include <vector>
#include <thread>
#include "mangafactory.h"
#include "mangabase.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    //signal emitted when chapter is downloaded
    void signalUpdateChaptersPalette(unsigned int);

    //signal emitted when downloading thread caught an exception
    void signalErrorOccured(const QString&);

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &);

    //make checkbox chapter text green when chapter is downloaded
    void updateChaptersPalette(unsigned int i);

    //show exception error occured in downloading thread
    void showError(const QString &);

private:
    //remove all checkboxes
    void clear_chapters();

    //add new checkbox
    void add_chapter(const std::string& chapter_name);

    void download_chapters();

private:
    enum class Mode
    {
        show_chapters,
        download_chapters
    };

private:
    Ui::MainWindow *ui;
    std::vector<QCheckBox*> chapters_list;
    Mode mode = Mode::show_chapters;
    MangaFactory mf;
    std::unique_ptr<MangaBase> site;
    std::thread th{[](){}};
};

#endif // MAINWINDOW_H
