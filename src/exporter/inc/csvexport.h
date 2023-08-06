/* this class is a sub-class of Exporter abstract class to implement the virtual abstract function xport()
 * SPECIALIZED FOR .csv export
 * two important parameters of every csv exporting procedures is separator and quote
 * separator: will separate the datas in a record so they can be recognized later when importing or for viewers
 * quote: it will encapsulate each data in two quotes like "Hello" has this type of quote <"> */

#ifndef CSVEXPORT_H
#define CSVEXPORT_H

#include <QtWidgets>
#include "exporter.h"

//! csvExport class will implement pure virtual procedure xport() of Exporter and will convert records and data in Exporter to the csv supported file type style
class csvExport: public Exporter
{
public:
    //! csvExport constructor will recieve the essential parameters required for csv export
    csvExport(QString filePath, int ibuffer, QString iseparator, QString iquote);
    csvExport(QString filePath, QString iseparator, QString iquote);
    csvExport(QString iseparator,QString iquote);
    csvExport() {buffer=100;separator=";";quote="\"";}
    /*! implemented procedure of csvExport is abstract xport() operation from Exporter abstract class
     * in the csvExport it will split the dataList with the separator and put them between quotes and will write them
     * into the files as strings for later use */
    void xport();

private:
    //! separator parameter for csv files exporting
    QString separator;
    //! quote parameter for csv files exporting
    QString quote;
};

#endif // CSVEXPORT_H
