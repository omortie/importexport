/* this class is written for exporting the input data (usually from databases) to other types of files like .csv
 * the procedure of eporting is done in 4 steps explained below:
 * 1. getting the input raw record as QStringList and put it in a QList variable (dataList) containing
 * many other QStringLists as records , this is done by the addRecord(QString) procedure.
 * 2. program will control if the number of records in QList will pass the buffer size so it will export
 * the cached data automatically
 * 3. if the xport() virtual abstract procedure has been called from somewhere it will decide which sub-classes of the
 * Exporter class meant by the user so the xport() operation will be done based on the option selected and the type file
 * 4. the output data based on the sub-class selected will be written into the filePath and the operation will is completed then. */

//Written By Mortie : if you had any question contact sherafati.morteza@gmail.com

#ifndef EXPORTER_H
#define EXPORTER_H

#include <QtWidgets>

//! Exporter class is an abstract class for performing export operation, it will recieve Data-to-Export as records and saves them to a list (dataList)
class Exporter
{
public:
    /*! Exporter class is an abstract class for performing export operation, it will recieve
     * Data-to-Export as records and saves them to a list (dataList) and when the number of records
     * passes the buffer number it will automatically exports the data to the selected file
     * \param filePath as a string is the address of the output file which data will save on
     * \param buffer an integer number which specify the buffer size and if the number of buffer
     * exceeds it will automatically exports data onto the file (default size of buffer is 100) */
    Exporter(const QString filePath, const int ibuffer = 100);
    Exporter(){}
    /*! sets the file path of the output file instead of specifying it in the constructor */
    void setFile(const QString filePath);
    /*! the operation of adding data to the exporter class will be done in addRecord and it will save it
     * in the list dataList
     * \param newRecord as a string list is the value list (strings) that are extracted from anywhere (models for
     * example) and will be stored in the internal list of Exporter */
    void addRecord(QStringList newRecord);
    //! sets the buffer size of the Exporter class
    void setBuffer(int ibuffer);
    //! for closing the opened file after writing the data into it from external command (exportDialog)
    void closeFile() {xportFile->close();}
    /*! all of the exporting operation will be done in xport() procedure as this is a special procedure
     * based on the type of the export file so this is a virtual abstract function that will be
     * implemented by sub-classes for example csvExport class for it's specific exporting operation */
    void virtual xport() = 0;

protected:
    //! a list of string list that contains the records data from database nested in a whole list of data to be converted by different exporters
    QList <QStringList> dataList;
    //! pointer to thee export file, saving the information of the file for the future uses
    QFile *xportFile;
    //! this buffer integer is using to buffering the export operation and also intermediate a sudden operation on the
    //! exporter to the destination file
    int buffer;
    //! this stream interface is used to stream the output data in the destination file and to stream the output data in any format
    QTextStream *out;
};
#endif // EXPORTER_H
