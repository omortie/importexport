#include "logger.h"
#include <QDesktopServices>
#include <QDebug>
#include <QDateTime>

QtMessageHandler Logger::previousHandler = nullptr;

static QFile *g_logFile = nullptr;

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!g_logFile || !g_logFile->isOpen())
        return;
    QTextStream out(g_logFile);
    out << QDateTime::currentDateTime().toString(Qt::ISODate) << " ";
    switch (type) {
    case QtDebugMsg: out << "[DEBUG] "; break;
    case QtInfoMsg: out << "[INFO]  "; break;
    case QtWarningMsg: out << "[WARN]  "; break;
    case QtCriticalMsg: out << "[ERROR] "; break;
    case QtFatalMsg: out << "[FATAL] "; break;
    }
    Q_UNUSED(context)
    out << msg << '\n';
    out.flush();
}

Logger::Logger(ImportSettings *settings)
    : settings(settings)
{
}

Logger::~Logger()
{
    commitLog();
}

void Logger::commitLog()
{
    if (g_logFile) {
        g_logFile->close();
        delete g_logFile;
        g_logFile = nullptr;
        if (previousHandler)
            qInstallMessageHandler(previousHandler);
        previousHandler = nullptr;
    }
}

// In this procedures log string will be manipulated with various operations details (main operation log reports)
void Logger::initLog() {
    if (!settings || !settings->getLogActive())
        return;

    g_logFile = new QFile(settings->getLogFilePath());
    if (!g_logFile->open(settings->getRwLog() ? (QIODevice::ReadWrite | QIODevice::Truncate)
                                         : (QIODevice::ReadWrite | QIODevice::Append)))
    {
        delete g_logFile;
        g_logFile = nullptr;
        return;
    }

    previousHandler = qInstallMessageHandler(Logger::messageHandler);
    qInfo().noquote() << "General Options:";
    qInfo().noquote() << "------";
    qInfo().noquote() << "Skipped Records:" << settings->getSkipRows();
    qInfo().noquote() << "Imported Records:" << settings->getImportRowCount() - 1;
    qInfo().noquote() << "Key Field:" << settings->getModel()->headerData(settings->getKeyField(), Qt::Horizontal).toString();
    qInfo().noquote() << "------";
}

void Logger::logInsert(const int &row) {
    qInfo().noquote() << "Inserting file data at index:" << row
                      << "- Inserted at the end of the list at index:" << settings->getModel()->rowCount();
}

void Logger::logUpdate(const int &index, const int &row) {
    qInfo().noquote() << "Updating database data at index:" << index
                      << "with file data at row" << row;
}

void Logger::logRemove(const int &index, const int &row) {
    qInfo().noquote() << "Deleting database data at index:" << index
                      << "because we found the same file data at row" << row;
}

void Logger::logSubmitStatus(const QString &subStat) {
    qInfo().noquote() << "";
    qInfo().noquote() << "Submit Status:";
    qInfo().noquote() << "------";
    qInfo().noquote() << subStat;
    qInfo().noquote() << "------";
}

void Logger::clearLog() {
    if (g_logFile)
        g_logFile->resize(0);
}

QString Logger::getLog() const
{
    if (!g_logFile)
        return QString();
    g_logFile->flush();
    return QString::fromUtf8(g_logFile->readAll());
}
