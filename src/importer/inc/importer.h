/* this class has been written to do the main duty of Importing datas from files to the databases
 * it will recieve two input parameters :
 * 1. filePath as QString that will save the input file address for opening and extracting the desired data
 * 2. database model as QSqlTableModel to access to the database table and the related data and also to apply changes to the
 * database records (inserting, deleting or updating records) */

//Written by Mortie : contact sherafati.morteza@gmail.com if you had any questions.

#ifndef IMPORTER_H
#define IMPORTER_H

#include <QtWidgets>
#include <QtSql>
#include "logger.h"
#include "settings.h"

 //! Imports the data from the table widget to the database (select,update or delete) procedures
class Importer : public QObject
{
public:
    // with this enumeration we can specify the main importing mode for modifying database records
    enum Mode
    {
        InsertAll = 0x0,
        InsertNew = 0x1,
        Update = 0x2,
        UpdateOrInsert = 0x4,
        Delete = 0x8,
        DeleteOrInsert = 0x16
    };
    Q_DECLARE_FLAGS(Modes,Mode)
    /*! Imports the data from the table widget to the database (select,update or delete) procedures
     *  \param model the QSqlTableModel connected to the database and will represend the database status */
    Importer(QSqlTableModel *model);
    Importer();
    /*! Sets the database model and will create a QSortFilterProxyModel to perform the search operation
     * \param model as a sql table model that has the connection to the database and access to all of it's table items */
    void setModel(QSqlTableModel *imodel);
    /*! the execy function will do the main duty in importing the extracted datas from file into the database
     * the user should specify 1 input described below:
     * Modes that will decide the import mode for modifying database tables
     * all other necessary parameters will be extracted from internal ImportSettings class populated during GUI procedures
     * \param Modes is the mode of import operation for example insert, delete, update or a combination of them
     * \return the result will be false if the operation was unsuccessfull and return true if it succeeds */
    bool execy(Importer::Modes mod);
    /*! Submit the changes of the model to the database, a built-in function to submit the model to the database
     * it will also checks for the validity of import after analyzing user input parameters and parsed data from source file
     *  and then do logs after import
     * \param err a string argument that will be set if there were an error occured in the process of submitting changes
     * \return the boolean returns the true if the process has been accomplished successsfully or vise versa */
    bool submit(QString &err);
    /*! this function will take the whole generated log from the logger class and returns it as a string
     * \return returns the whole generated string */
    QString getLog() {return logger->getLog();}

    //! pointer to the import settings class
    ImportSettings *settings = new ImportSettings;
    //! pointer to the logger class to manage the log generating and it's result string
    Logger *logger;

private:
    /*! This function is for finding a specific record in the database and its related row number
     * if it exists in the model then the correspondence row number will be returned
     * otherwise -1 will be returned , by this function the component can determine if a record
     * exist in the database or not this function will return the row number index of the record
     * which has the value in it's field index in database
     * \param value the string that we are looking for in the database
     * \param fieldIndex the index of the field that usually is key field so the search will be done
     * on the data of that filed
     * \return the number of the row of the related found record if the value has been found
     * returns -1 if the value has not been found */
    int Finder(const QString value,const int fieldIndex);
    /*! Inserts the (index) row of the table widget data to the last of the records of the database
     * \param index the row index of the record from table widget that is selected to be inserted */
    bool insertRecord(const int &index);
    /*! Updates the record in the (row) index of the model with the data in the (index) of the table widget
     * \param index the row number of the record in the table widget
     * \param row the row number of the record from model that is going to be updated
     * \return the boolean returns true if the update operation was successful and vise versa */
    bool updateRecord(const int &index,const int &row);

    //! initializing a hash list based on the row number and key field data text and provide a fast searching dictionary
    //! for searching a specific value
    //! \param fieldIndex as the key field index in the database
    void initHash(int fieldIndex);

    //! the index number of the field that has been set to be the key field
    int keyField;
    //! to store the data of the record (the record that is going to be inserted or updated) and for building our own records
    //! based on the default database records with it's correspondent field names
    QSqlRecord record;
    //! model as a sql table model that has the connection to the database and access to all of it's table items
    QSqlTableModel *model;
    //! this QMap is a key parameter that will define the relation between the fields in the database to the fields in the file data
    QMap<int,int> key;
    //! integer that is defining the end point of our import operation
    int endpoint;
    //! integer that is defining as the number of rows that are going to be skipped in the import operation
    int skip;
    //! a QHash dictionary for faster search in the database datas
    QHash<QString,int> hash;

protected:
    //! this is also a fileTable that stores the data items parsed from the source file
    QTableWidget *fileTable;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Importer::Modes)

#endif // IMPORTER_H
