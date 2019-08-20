#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <memory>
#include <vector>
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

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &);

private:
    void clear_chapters();

    void add_chapter(const std::string& chapter_name);

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
};

#endif // MAINWINDOW_H
