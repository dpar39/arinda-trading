#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "filedownloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_drawChartButton_clicked();

    void onDataDownloaded();

private:
    Ui::MainWindow *ui;

    FileDownloader* m_downloader;

    static QString s_urlBase;
};

#endif // MAINWINDOW_H
