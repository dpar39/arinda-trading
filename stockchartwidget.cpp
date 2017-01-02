#include "stockchartwidget.h"
#include <QPainter>

StockChartWidget::StockChartWidget(QWidget *parent) : QWidget(parent)
{
    m_numDays = 90; // 3 months for now
}

void StockChartWidget::renderData(std::shared_ptr<StockData> stockData)
{
    m_stockData = stockData;
    repaint();
}

int StockChartWidget::numDays() const
{
    return m_numDays;
}

void StockChartWidget::setNumDays(int value)
{
    m_numDays = value;
    repaint();
}

void StockChartWidget::calculateDataLimits()
{
    m_absoluteHighPrice = 0.0;
    m_absoluteLowPrice = 1E42;
    m_absoluteMaxVolume = 0.0;

    auto days = 0;
    auto it = m_stockData->entries().begin();
    auto itEnd = m_stockData->entries().end();
    while (it != itEnd && days < m_numDays)
    {
        const StockEntry & e = *it;
        if (e.low < m_absoluteLowPrice)
        {
            m_absoluteLowPrice = e.low;
        }
        if (e.high > m_absoluteHighPrice)
        {
            m_absoluteHighPrice = e.high;
        }
        if (e.volume > m_absoluteMaxVolume)
        {
            m_absoluteMaxVolume = e.volume;
        }
        it++;
        days++;
    }
}

void StockChartWidget::drawTickAndLabel(double value, QPainter &p, double labelY)
{
    auto xLabelStart = m_axisLabelXOffset - s_axisTickWidth/2;
    auto xLabelEnd = m_axisLabelXOffset + s_axisTickWidth/2;

    const int labelOffset = 3;
    const int pricePrecision = 2;
    const int labelSize = 100;
    p.setPen(Qt::black);
    p.drawLine(xLabelStart, labelY, xLabelEnd, labelY);

    auto strLabel = labelFromValue(value);

    p.drawText(QRectF(xLabelEnd + labelOffset, labelY-labelSize/2, labelSize, labelSize), Qt::AlignVCenter, strLabel);
}

QString StockChartWidget::labelFromValue(double value)
{
    QString suffix = "";
    if (value > 1E6)
    {
        value /= 1E6;
        suffix = "m";
    }
    else if (value > 1E3)
    {
        value /= 1E3;
        suffix = "k";
    }
    auto strLabel = QString::number(value, 'f', 2) + suffix;
    return strLabel;
}

void StockChartWidget::drawAxisLabelsAndTicks(QPainter &p)
{
    const int numPriceLabels = 5;
    for (int labelIdx = 0; labelIdx < numPriceLabels; labelIdx++)
    {
        auto priceLabel = m_absoluteLowPrice + labelIdx*(m_absoluteHighPrice-m_absoluteLowPrice)/(numPriceLabels-1);
        auto labelY = priceValueToYCoordinate(priceLabel);
        drawTickAndLabel(priceLabel, p, labelY);
    }

    const int numVolumeLabels = 3;
    for (int labelIdx = 0; labelIdx < numVolumeLabels; labelIdx++)
    {
        auto volumeLabel = labelIdx*(m_absoluteMaxVolume)/(numVolumeLabels-1);
        auto labelY = volumeValueToYCoordinate(volumeLabel);
        drawTickAndLabel(volumeLabel, p, labelY);
    }
}

void StockChartWidget::drawPriceAndVolumeData(QPainter &p)
{
    const auto openCloseLengthPix = 3;
    QPen openClosePen(Qt::black, 2);
    QPen lowHighBarPen(Qt::black, 3);
    QPen volumeBarPen(Qt::black, 3);

    auto dayIndex = 0;
    auto it = m_stockData->entries().begin();
    auto itEnd = m_stockData->entries().end();
    const auto yZeroVolume = volumeValueToYCoordinate(0);
    while (it != itEnd && dayIndex < m_numDays)
    {
        const StockEntry & e = *it;
        auto x = dayIndexToXCoordinate(dayIndex);

        auto yOpen = priceValueToYCoordinate(e.open);
        auto yClose = priceValueToYCoordinate(e.close);
        auto yLow = priceValueToYCoordinate(e.low);
        auto yHigh = priceValueToYCoordinate(e.high);

        p.setPen(openClosePen);
        p.drawLine(x - openCloseLengthPix, yOpen, x, yOpen);
        p.drawLine(x + openCloseLengthPix, yClose, x, yClose);

        p.setPen(lowHighBarPen);
        p.drawLine(x, yLow, x, yHigh);

        auto yVolume = volumeValueToYCoordinate(e.volume);

        p.setPen(volumeBarPen);
        p.drawLine(x, yZeroVolume, x, yVolume);

        it++;
        dayIndex++;
    }
}

void StockChartWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    if (m_stockData == nullptr)
    {
        return;
    }

    auto volumePriceDivPix = height()*s_verticalDivisionRatio;
    m_minMaxPriceDivPix = volumePriceDivPix  - 2*s_marginPix;
    m_maxVolumeDivPix = height()*(1- s_verticalDivisionRatio) - 2*s_marginPix;

    m_axisLabelXOffset = width() - s_axisLabelWidthPix;
    m_samplesDivPix = m_axisLabelXOffset - 2*s_marginPix;

    // Draw dividing line and chart border
    p.setPen(Qt::darkGray);
    p.setBrush(Qt::transparent);
    p.drawRect(0, 0, width()-1, height()-1);

    p.setPen(Qt::gray);
    p.drawLine(0, volumePriceDivPix, width(), volumePriceDivPix );

    // Draw axis line
    p.drawLine(m_axisLabelXOffset, 0, m_axisLabelXOffset, height()-1);

    // Calculate data limits
    calculateDataLimits();

    // Draw Price and Volume axis labels and ticks
    drawAxisLabelsAndTicks(p);

    // Draw price and volume data
    drawPriceAndVolumeData(p);
}

double StockChartWidget::priceValueToYCoordinate(double price)
{
    auto y = s_marginPix + (m_absoluteHighPrice - price)/(m_absoluteHighPrice - m_absoluteLowPrice)*m_minMaxPriceDivPix;
    return y;
}

double StockChartWidget::volumeValueToYCoordinate(double volume)
{
    auto y = height() - s_marginPix - volume/m_absoluteMaxVolume*m_maxVolumeDivPix;
    return  y;
}

double StockChartWidget::dayIndexToXCoordinate(int dayIndex)
{
    return s_marginPix + m_samplesDivPix * dayIndex/(m_numDays-1);
}
