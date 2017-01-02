#ifndef STOCKCHARTWIDGET_H
#define STOCKCHARTWIDGET_H

#include <QWidget>
#include "stockdata.h"

class StockChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StockChartWidget(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);
signals:

public slots:
    void renderData(std::shared_ptr<StockData> stockData);

public:

    int numDays() const;
    void setNumDays(int value);

private:
    int m_numDays;

    std::shared_ptr<StockData> m_stockData;

    void calculateDataLimits();

    double dayIndexToXCoordinate(int dayIndex);

    double priceValueToYCoordinate(double price);

    double volumeValueToYCoordinate(double volume);

private:
    const double s_axisLabelWidthPix = 55.0;
    const double s_verticalDivisionRatio = 2.0/3.0;
    const double s_marginPix = 15;

    const int s_axisTickWidth = 10;

    double m_absoluteHighPrice;
    double m_absoluteLowPrice;
    double m_absoluteMaxVolume;

    double m_axisLabelXOffset;

    double m_minMaxPriceDivPix;
    double m_maxVolumeDivPix;
    double m_samplesDivPix;

    void drawTickAndLabel(double value, QPainter &p, double labelY);

    static QString labelFromValue(double value);
    void drawAxisLabelsAndTicks(QPainter &p);
    void drawPriceAndVolumeData(QPainter &p);
};

#endif // STOCKCHARTWIDGET_H
