#ifndef LOGGER_H
#define LOGGER_H

#include "settings.h"

//! Logger class is used to generate log reports and save them in a log file, many operations like Insert, Update and Delete is supported by logger.
class Logger
{
public:
    /*! Logger class constructor will recieve settings parameters specially logActive and logFilePath to know the address of the log file
     * \param pointer to settings class that will store the parameters and settings related to the importing operation */
    Logger(ImportSettings *settings)
        :settings(settings) {}
    Logger() {}
    ~Logger() {}
    /*! This procedure is used to commit logs that has been generated, write them to the destination log file */
    void commitLog();
    /*! This procedure will generate the header information of the log file, as an initialization */
    void initLog();
    /*! Generator log for an insert operation */
    void logInsert(const int &row);
    /*! Generator log for an update operation
     * \param index as the index of item in the database that is going to be updated
     * \param row as the index of item in the source file and data that are going to be replaced as new data in the database */
    void logUpdate(const int &index,const int &row);
    /*! Generator log for remove operation
     * \param index as the index of the data item in the database to be deleted
     * \param row as the index  of the data item in the source file that has been found in the database */
    void logRemove(const int &index,const int &row);
    /*! Generating status of submit log in the log string in the end of the operation
     * \param subStat as the status of submit (successful or a failure) */
    void logSubmitStatus(const QString &subStat);
    /*! clears all of the log that is generated until now */
    void clearLog();
    /*! \return generated log */
    QString getLog() {return log;}

private:
    //! log string, all of the strings will append to this string, and this string will be used as the final generated string
    QString log = "";
    //! pointer to the import settings for accessing log file path and other parameters like request for logging boolean and data items in the table
    ImportSettings *settings;
};

#endif // LOGGER_H
