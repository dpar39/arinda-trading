#include "stockdata.h"

#include <limits>

StockData::StockData()
{
}

std::shared_ptr<StockData> StockData::fromCsvByteArray(const QByteArray &csvData)
{
    try
    {
        QString data = QString::fromUtf8(csvData);
        std::shared_ptr<StockData> stockData = std::shared_ptr<StockData>(new StockData);
        for (auto &line : data.split(QRegExp("[\r\n]"),QString::SkipEmptyParts))
        {
            // Date	Open High Low Close Volume AdjClose
            QStringList values = line.split(QChar(','), QString::SkipEmptyParts);

            if (values.length() != 7)
            {
                throw std::runtime_error("CSV data should have 7 fields");
            }

            if (line.contains("Date"))
            {
                // Ignore header line
                continue;
            }

            StockEntry e;
            e.date =  QDate::fromString(values[0], "yyyy-MM-dd");
            e.open = values[1].toDouble();
            e.high = values[2].toDouble();
            e.low = values[3].toDouble();
            e.close = values[4].toDouble();
            e.volume = values[5].toDouble();

            stockData->m_historicalData.push_back(e);
        }
        return stockData;
    }
    catch(...)
    {
        // TODO: We need better error handling to alert the user
        return nullptr;
    }
}
