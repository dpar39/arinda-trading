#ifndef STOCKDATA_H
#define STOCKDATA_H

#include <QDate>
#include <vector>

#include <memory>

struct StockEntry
{
    QDate date;
    double open;
    double low;
    double high;
    double close;
    double volume;
};

class StockData
{
private:
    std::vector<StockEntry> m_historicalData;

    StockData();
public:
    static std::shared_ptr<StockData> fromCsvByteArray(const QByteArray &csvData);

    const std::vector<StockEntry> &entries() const { return m_historicalData; }
};

#endif // STOCKDATA_H
