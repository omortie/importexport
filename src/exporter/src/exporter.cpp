#include "exporter.h"

Exporter::Exporter(const QString filePath,const int ibuffer)
    :buffer(ibuffer)
{
    xportFile = new QFile(filePath);
    xportFile->open(QIODevice::WriteOnly|QIODevice::Truncate);
    out=new QTextStream(xportFile);
    out->setCodec("UTF-8"); // Default codec for now is Unicode UTF-8 , maybe get developed in the future
}

void Exporter::setFile(const QString filePath)
{
    if(xportFile->fileName() == filePath)
        return;

    if(xportFile->isOpen())
        xportFile->close();

    xportFile->setFileName(filePath);
}

void Exporter::addRecord(QStringList newRecord)
{
    dataList.append(newRecord);
    //perform the export operation if number of records passes the buffer size
    if(dataList.length() > buffer)
    {
        xport();
        dataList.clear();
    }
}

void Exporter::setBuffer(int ibuffer)
{
    buffer=ibuffer;
}
