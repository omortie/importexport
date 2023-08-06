#include "logger.h"
#include <QDesktopServices>

void Logger::commitLog()
{
    // Checks if the request for logging is active or not
    if (settings->getLogActive())
    {
        // in the case of request for logging, it will open the log file
        QFile *logFile = new QFile(settings->getLogFilePath());
        if (logFile->open((settings->getRwLog()) ? (QIODevice::WriteOnly) : (QIODevice::Append)))
        {
            // writer streamer for writing log information in the log file
            QTextStream out(logFile);
            out<<log;
            logFile->close(); // closing the log file after commiting each log
        }
    }
}

// In this procedures log string will be manipulated with various operations details (main operation log reports)
void Logger::initLog() {
    log += "General Options:\n------\nSkipped Records: "  + QString::number(settings->getSkipRows()) + "\nImported Records: "
            + QString::number(settings->getImportRowCount()-1)
            +"\nKey Field: "+settings->getModel()->headerData(settings->getKeyField(),Qt::Horizontal).toString()+"\n------\n";
}

void Logger::logInsert(const int &row) {
    log += "\nInserting file data at index : " + QString::number(row);
    log += " - Inserted at the end of the list at index : " + QString::number(settings->getModel()->rowCount());
}

void Logger::logUpdate(const int &index, const int &row) {
    log += "\nUpdating database data at index : "+ QString::number(index) +" with file data at row "+QString::number(row);
}

void Logger::logRemove(const int &index, const int &row) {
    log += "\nDeleting database data at index : "+ QString::number(index) +" becuase we found the same file data at row "+QString::number(row);
}

void Logger::logSubmitStatus(const QString &subStat) {
    log += "\n\nSubmit Status:\n------\n" + subStat + "\n------\n";
}

void Logger::clearLog() {
    log = "";
}
