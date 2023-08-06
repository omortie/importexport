// Written by Mortie (sherafati.morteza@gmail.com) , contact me if you had any questions

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QTableWidget>
#include <QSettings>
#include <QXmlStreamReader>
#include <QtSql>
#include "csvreader.h"

/*! These two functions is for creating a customized format (xml) for QSettings class to read settings from .xml data files
 * It will specify the method of reading parameters from xml files and populates
 * a SettingsMap <string,string> once , then when user wants a setting it will search
 * in the constructed map to find the related value
 * \param device the IODevice sent by the QSettings based on the parameters of it's constructor
 * specifying the path of the xml file
 * \param map a QMap<QString,QString> sent by the QSetting to be populated by the values and the keys
 * of the respected xml file */
bool readXmlFile(QIODevice& device,QSettings::SettingsMap& map);
/*! in this application we don't need to write anything on xml files so we literally
 * cleared any command in this function , it always returns true */
bool writeXmlFile(QIODevice& device,const QSettings::SettingsMap& map);

//! Initializing the QSetting instance to extract data and parameters from .xml file
void initXml();

//! Defines the maximum number that QSpinBoxes can show in the components
#define maxSpinners 4000



//! ImportSettings is a setting-based class which saves the parameters and other data-related pointers for import component
class ImportSettings : public QObject
{
    Q_OBJECT

public:
    ImportSettings() {initXml();setParameters();}

    //Global Import Settings
    void setModel(QSqlQueryModel *imodel);
    void setFilePath(QString ifilePath) {filePath = ifilePath;}
    void setSeparator(QString isep) {sep = isep;}
    void setQuote(QString iquote) {quote = iquote;}
    void setSkipRows(int iskipNum) {skipNum = iskipNum;}
    void setCommitNumber(int icommitNumber) {commitNumber = icommitNumber;}
    void setImportRowCount(int irowCount) {rowCount = irowCount;}
    void setKeyMap(QMap<int,int> ikeys) {keys = ikeys;}
    void setKeyField(int ifield) {keyField=ifield;}
    void setFileType(QString ifileType) {fileType = ifileType;}
    void setDataTable(QTableWidget *itable) {table = itable;}
    void setSqlId(int checkedId) {sqlId=checkedId;}
    void setImportAll(bool iimportAll) {importAll = iimportAll;}
    void setCommitAfter(bool icommitAfter) {commitAfter = icommitAfter;}
    void setLogActive(bool ilogActive) {logActive = ilogActive;}
    void setLogFilePath(QString ilogPath) {logPath=ilogPath;}
    void setRwLog(bool irwLog) {rwLog = irwLog;}
    void setShowLog(bool ishowLog) {showLog = ishowLog;}
    void setCodec(QString icodec) {codec=icodec;}
    void setValid(bool iisValidForImport) {isValidForImport = iisValidForImport;}
    void setOperation(QString ioperation) {operation = ioperation;}
    QSqlQueryModel *getModel() {return model;}
    QString getFilePath() {return filePath;}
    QString getSeparator() {return sep;}
    QString getQuote() {return quote;}
    int getSkipRows() {return skipNum;}
    int getCommitNumber() {return commitNumber;}
    int getImportRowCount() {return rowCount;}
    QMap<int,int> getKeyMap() {return keys;}
    void insertKeyMap(int key,int value) {keys.insert(key,value);}
    int getKeyField() {return keyField;}
    QString getFileType() {return fileType;}
    bool convertFileToTable();
    QTableWidget *getDataTable() {return table;}
    int getSqlId() {return sqlId;}
    bool getImportAll() {return importAll;}
    bool getCommitAfter() {return commitAfter;}
    bool getLogActive() {return logActive;}
    bool getRwLog() {return rwLog;}
    bool getShowLog() {return showLog;}
    QString getLogFilePath() {return logPath;}
    QString getCodec() {return codec;}
    QString getOperation() {return operation;}
    bool getConverted() {return converted;}
    void setConverted(bool iconverted) {converted = iconverted;}
    void setTechnical(bool itechnical) {technical = itechnical;}
    bool getTechnical() {return technical;}
    QMap<int,QString> getHeaders() {return headers;}

    //! This function will set the parameters based on the xml extracted parameters (if they exist and not null)
    //! if the paramters were all set return True otherwise returns False
    bool setParameters();

    /*! This essential function will check all of the parameters and conditions neccesary for import operation
     * and if there would be no problem will return True , but if there were any problem will set a string as
     * error to debug the program
     * \return boolean as a flag of validation and ready to import or not */
    bool getValid(QString &errorStr);

private:
    //Initializations for import component GUI elements
    QString filePath="";
    QString sep = ",";
    QString quote = "\"";
    QString codec = "UTF-8";
    int skipNum = 0;
    int commitNumber = 100;
    int rowCount = 0;
    QMap<int,int> keys;
    int keyField=0;
    QString fileType = "csv";
    QTableWidget *table;
    int sqlId = -2;
    bool importAll = true;
    bool commitAfter = true;
    bool logActive = false;
    bool rwLog = false;
    bool showLog = false;
    QString logPath = "";
    bool isValidForImport = true;
    QSqlQueryModel *model;
    QString operation;
    bool converted = false;
    bool technical = false;
    QMap<int,QString> headers;
};

//! ExportSettings is a setting-based class which saves the parameters and other data-related pointers for export component
class ExportSettings
{
public:
    ExportSettings() {initXml();setParameters();}

    //Global Export Settings
    void setFilePath(QString ifilePath) {filePath = ifilePath;}
    void setFileType(QString ifileType) {fileType = ifileType;}
    void setOpenAfter(bool iopenAfter) {openAfter = iopenAfter;}
    void setGotoFirstRecord(bool igotoFirst) {gotoFirst = igotoFirst;}
    void setCurrentOnly(bool icurrentOnly) {currentOnly = icurrentOnly;}
    void setSkipRow(int iskipRow) {skipRow = iskipRow;}
    void setExportOnly(int iexportOnly) {exportOnly = iexportOnly;}
    void setExportAll(bool iexportAll) {exportAll = iexportAll;}
    void setSeparator(QString iseparator) {separator = iseparator;}
    void setQuote(QString iquote) {quote = iquote;}
    void setExportHeaders(bool iexportHeaders) {exportHeaders = iexportHeaders;}
    void setHeader(QString iHeader) {Header = iHeader;}
    void setFooter(QString iFooter) {Footer = iFooter;}
    QString getFilePath() {return filePath;}
    QString getFileType() {return fileType;}
    bool getOpenAfter() {return openAfter;}
    bool getGotoFirstRecord() {return gotoFirst;}
    bool getCurrentOnly() {return currentOnly;}
    int getSkipRow() {return skipRow;}
    int getExportOnly() {return exportOnly;}
    bool getExportAll() {return exportAll;}
    QString getSeparator() {return separator;}
    QString getQuote() {return quote;}
    bool getExportHeaders() {return exportHeaders;}
    QString getHeader() {return Header;}
    QString getFooter() {return Footer;}

private:
    bool setParameters(); //will set the parameters based on the xml extracted parameters (if they exist and not null)
    //Initializations for import component GUI elements
    QString filePath = "";
    QString fileType = "csv";
    bool openAfter = true;
    bool gotoFirst = false;
    bool currentOnly = false;
    int skipRow = 0;
    int exportOnly = 0;
    bool exportAll = true;
    QString separator = ",";
    QString quote = "\"";
    bool exportHeaders = false;
    QString Header = "";
    QString Footer = "";
};

/*! Wrapping Import Wizard Class to PythonQt Component so advanced user can command the shell from Python console
 * most of the functions from importer class and settings class can be wrapped here easily to give advanced access to the user */
class ImportSettingsWrapper : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    //! constructor of a new object as ImportWizard in python to use it as a reference to other functions and procedures
    ImportSettings *new_ImportSettings() {return new ImportSettings();}
    //! destructor of the created wrapped ImportWizard object in python
    void delete_ImportSettings(ImportSettings *o) {delete o;}
    //! wrapped class for setting import source file path for the importer class
    void setFilePath(ImportSettings *o,QString filePath) {o->setFilePath(filePath);}
    //! wrapped class for setting file type of the import source file for the importer class
    void setFileType(ImportSettings *o,QString fileType) {o->setFileType(fileType);}
    //! this wrapped procedure will set separator for .csv and .txt file types to parse them correctly according to the separator
    void setSeparator(ImportSettings *o,QString separator) {o->setSeparator(separator);}
    //! this wrapped will set the quote for .csv and .txt file types to parse them correctly according to the quote
    void setQuote(ImportSettings *o,QString quote) {o->setQuote(quote);}
    //! wrapped function to access to the table data items parsed from source file in python scripts to manipulate those data
    QTableWidget *getTable(ImportSettings *o) {return o->getDataTable();}
    //! wrapped procedure to set the number of rows to skip while importing in the import settings class
    void setSkipRows(ImportSettings *o,int skipRows) {o->setSkipRows(skipRows);}
    //! wrapped procedure to set the number of rows to import in the import settings class
    void setImportRowCount(ImportSettings *o,int importRow) {o->setImportRowCount(importRow);}
    //! wrapped function to set the mapping keys , relating source columns to destination fields
    void insertKey(ImportSettings *o,int key,int value) {o->insertKeyMap(key,value);}
    //! the procedure that has been wrapped to find the key field in database based on it's input string argument
    void setKeyField(ImportSettings *o,QString key) {o->setKeyField(o->getHeaders().key(key,0));}
    //! wrapped procedure to set the number of rows to commit each time in the settings class
    void setcommitAfterEdit(ImportSettings *o,int commitNumber) {o->setCommitNumber(commitNumber);}
    //! a wrapped procedure to set the mode of import operation in the import settings class based on the name of the operation
    void setCusMode(ImportSettings *o,QString Mode) {
        int cusMode=-2;
        if (Mode == tr("addall"))
            cusMode = -2;
        if (Mode == tr("addnew"))
            cusMode = -3;
        if (Mode ==  tr("update"))
            cusMode = -4;
        if (Mode ==  tr("updateadd"))
            cusMode = -5;
        if (Mode == tr("delete"))
            cusMode = -6;
        if (Mode == tr("deleteadd"))
            cusMode = -7;
        o->setSqlId(cusMode);
    }
    //! a function that has been wrapped ot access to the sql table model data items in the database to manipulate them by scripting
    QSqlQueryModel *getModel(ImportSettings *o) {return o->getModel();}
    //! a function that will do the parsing process of input file and will convert content of the file to a tablwidget based on the scripting requests
    //! \return boolean as if the process was accomplished successfuly or not */
    bool convertFileToTable(ImportSettings *o) {return o->convertFileToTable();}
};

#endif // SETTINGS_H
