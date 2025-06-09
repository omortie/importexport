#ifndef LOGGER_H
#define LOGGER_H

#include "settings.h"
#include <QLoggingCategory>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

//! Logger class is used to generate log reports and save them in a log file, many operations like Insert, Update and Delete is supported by logger.
class Logger
{
public:
    explicit Logger(ImportSettings *settings = nullptr);
    ~Logger();

    /*! Initialize logger and open log file using Qt's logging framework */
    void initLog();
    /*! Flush log file and restore default message handler */
    void commitLog();

    void logInsert(const int &row);
    void logUpdate(const int &index,const int &row);
    void logRemove(const int &index,const int &row);
    void logSubmitStatus(const QString &subStat);
    void clearLog();
    QString getLog() const;

private:
    static void messageHandler(QtMsgType type,const QMessageLogContext &context,const QString &msg);

    ImportSettings *settings = nullptr;
    QFile logFile;
    QTextStream logStream;
    static QtMessageHandler previousHandler;
};

#endif // LOGGER_H
