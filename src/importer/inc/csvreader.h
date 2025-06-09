#ifndef CSVREADER_H
#define CSVREADER_H
//Written by Mortie : contact sherafati.morteza@gmail.com if you had any questions.

#include <QTableWidget>
#include <QFile>
#include <QStringConverter>
#include <QDebug>

const QString DOUBLE_QUOTE("\"");
const QString CR("\r");
const QString LF("\n");

//! Preprocess one raw line from a file
class AbstractProcessor
{
public:
    explicit AbstractProcessor() {}
    virtual ~AbstractProcessor() {}

    /*! Preprocess one raw line from a file
     * \param line raw line from a file */
    virtual void preProcessRawLine(QString& line)
    {
        // Here you can edit line
        Q_UNUSED(line);
    }

    /*! Process one row worth of elements
     * \param elements list of row elements
     * \return bool True if elements was processed successfully, False in case
     * of error. If process() return False, the csv-file will be stopped
     * reading */
    virtual bool processRowElements(const QStringList& elements) = 0;
};

//! ReadToListProcessor - processor that saves rows of elements to list.
class ReadToListProcessor : public AbstractProcessor
{
public:
    QList<QStringList> data;
    virtual bool processRowElements(const QStringList& elements)
    {
        data << elements;
        return true;
    }
};

struct ElementInfo
{
public:
    ElementInfo() : isEnded(true) {}
    bool isEnded;
};

//! Constructors of reader
class csvReader : public QObject
{
public:
    /*! Constructors of reader
     * Read csv-file and creates a table widget populated from data extracted from csv file
     * \param filePath string with absolute path to csv-file
     * \param separator string or character that separate elements in a row
     * \param quote string or character that enclose each element in a row
     * \param codec pointer to codec name that would be used for file reading */
    csvReader(const QString &filePath, const QString &separator, const QString &quote, const QString &codec);
    csvReader(const QString ifilePath);
    csvReader() {}
    /*! \param separator parameter that has been set and used to parse source file data */
    void setSeparator(const QString iseparator) {separator=iseparator;}
    /*! \param quote parameter that has been set and used to parse source file data */
    void setQuote(const QString iquote) {quote=iquote;}
    /*! sets the file path to the importFile to be used as IODevice later by reader
     * \param filePath string with absolute path to csv-file */
    void setFilePath(const QString filePath);
    /*! \return separator parameter that has been set and used to parse source file data */
    QString getSeparator() {return separator;}
    /*! \return quote parameter that has been set and used to parse source file data */
    QString getQuote() {return quote;}
    /*! \return path string parameter to the source file data */
    QString getFilePath() {return filePath;}
    /*! Import and parse data from csv file */
    void import();
    /*! \return useful parsed data stored in the reader as a table widget (separated data items) */
    QTableWidget *getTable() {return fileTable;}
    bool getSuccess() {return success;}
private:
    //! private internal procedure for creating a table widget from dataList
    void createTable();
    /*! Read csv-file and save it's data as strings to QList<QStringList>
    * - \param filePath string with absolute path to csv-file
    * - \param separator string or character that separate elements in a row
    * - \param textDelimiter string or character that enclose each element in a row
    * - \param codec pointer to codec object that would be used for file reading
    * - \return QList<QStringList> list of values (as strings) from csv-file In case of
    * error will return empty QList<QStringList>. */
    static QList<QStringList> readToList(const QString& filePath,
                    const QString& separator = QString(","),
                    const QString& textDelimiter = QString("\""),
                                         QStringConverter::Encoding codec = QStringConverter::Utf8);

    /*! Read csv-formatted data from IO Device and save it
    * as strings to QList<QStringList> */
    static QList<QStringList> readToList(QIODevice& ioDevice,
                    const QString& separator = QString(","),
                    const QString& textDelimiter = QString("\""),
                                         QStringConverter::Encoding codec = QStringConverter::Utf8);
    /*! Function that really reads csv-data and transfer it's data to
     * AbstractProcessor-based processor */
    static bool read(QIODevice& ioDevice,
                     AbstractProcessor& processor,
                     const QString& separator,
                     const QString& textDelimiter,
                     QStringConverter::Encoding codec);
    /*! Check if file path and separator are valid
    * \param separator - string or character that separate values in a row
    * \return bool - True if file path and separator are valid, otherwise False */
    static bool checkParams(const QString& separator);
    /*! Split string to elements
    * \param line - string with data
    * \param separator - string or character that separate elements
    * \param textDelimiter - string that is used as text delimiter
    * \return QStringList - list of elements */
    static QStringList splitElements(const QString& line,
                                     const QString& separator,
                                     const QString& textDelimiter,
                                     ElementInfo& elemInfo);
    /*! Try to find end position of first or middle element
    * \param str - string with data
    * \param startPos - start position of the current element in the string
    * \param separator - string or character that separate elements
    * \param textDelimiter - string that is used as text delimiter
    * \return int - end position of the element or -1 if this element is not first or middle */
    static int findMiddleElementPosition(const QString& str,
                                          const int& startPos,
                                          const QString& separator,
                                          const QString& txtDelim);
    /*! Check if current element is the last element
    // \param str - string with data
    // \param startPos - start position of the current element in the string
    // \param separator - string or character that separate elements
    // \return bool - True if the current element is the last element of the string, False otherwise */
    static bool isElementLast(const QString& str,
                              const int& startPos,
                              const QString& separator,
                              const QString& txtDelim);
    /*! Remove extra symbols (spaces, text delimeters...)
    * \param elements - list of row elements
    * \param textDelimiter - string that is used as text delimiter */
    static void removeExtraSymbols(QStringList& elements,
                                   const QString& textDelimiter);

    //! essential paramteres for parsing csv files
    QString separator,quote,filePath;
    //! produced table widget out of extracted data from file
    QTableWidget *fileTable;
    //! codec that has been used to read and parse the file
    QString codec;
    //! a falg for determining that the file and it's data has been successfully parsed into a table widget
    bool success = true;
    //! this QList<QStringList> will hold the QString datas which is extracted from the file by import() procedure
    QList<QStringList> dataList;
    //! the file that the extracted data is going to be extracted from
    QFile *importFile = new QFile;
};
#endif // CSVREADER_H
