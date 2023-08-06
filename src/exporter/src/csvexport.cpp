#include "csvexport.h"

csvExport::csvExport(QString filePath, int ibuffer, QString iseparator, QString iquote)
    : Exporter(filePath,ibuffer)
{
    separator=iseparator;
    quote=iquote;
}

csvExport::csvExport(QString filePath, QString iseparator, QString iquote)
    : Exporter(filePath)
{
    separator=iseparator;
    quote=iquote;
}

csvExport::csvExport(QString iseparator, QString iquote)
{
    separator=iseparator;
    quote=iquote;
}

void csvExport::xport()
{
    QString outputData="";
    for(int i=0;i<dataList.length();i++)
    {
        outputData += quote;
        outputData += dataList.at(i).join(quote+separator+quote);
        outputData += quote + "\n";
    }
    *out<<outputData;
}
