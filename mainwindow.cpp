#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filedownloader.h"
#include "stockdata.h"


#include <QDebug>

QString MainWindow::s_urlBase = "http://ichart.finance.yahoo.com/table.csv?s=%1";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_downloader(new FileDownloader(this))

{
    ui->setupUi(this);
    m_downloader->connect(m_downloader, SIGNAL(downloaded()), this, SLOT(onDataDownloaded()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_drawChartButton_clicked()
{
    // Start downloading the data
    auto symbol = ui->symbolLineEdit->text();
    auto queryUrl = QUrl(s_urlBase.arg(symbol));
    m_downloader->download(queryUrl);
}

void MainWindow::onDataDownloaded()
{
    auto csvData = m_downloader->downloadedData();

    auto stockData = StockData::fromCsvByteArray(csvData);

    ui->stockChart->renderData(stockData);

    auto updateMsg = stockData == nullptr
            ? "Unable to download stock data."
            : "Data downloaded successfully.";
    ui->statusBar->showMessage(updateMsg, 3000);
}
