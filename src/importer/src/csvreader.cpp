#include "csvreader.h"

csvReader::csvReader(const QString &filePath, const QString &separator, const QString &quote, const QString &codec)
    : separator(separator)
    , quote(quote)
    , codec(codec)
{
    setFilePath(filePath);
    import();
}

void csvReader::setFilePath(const QString filePath)
{
    importFile->setFileName(filePath);
}

void csvReader::import()
{
    //calling readToList and sends file and separator and quote and codec so it will extract data from csv
    dataList = readToList(*importFile,separator,quote,QTextCodec::codecForName(codec.toLatin1()));
    if (dataList.isEmpty()) success = false;
    for (int i=0;i<dataList.count();i++)
    {
        if (dataList.at(i).count() == 0) success = false;
    }
    //if the operation succeed so creates a table from the list of rows
    if (success) createTable();
}

void csvReader::createTable()
{
    // creating a table widget out of data extracted from the file
    fileTable = new QTableWidget;
    fileTable->setRowCount(dataList.count());
    // finding the maximum number of columns in the extracted data
    int maxCol = 1;
    for (int i = 0 ; i < fileTable->rowCount() ; i++){
        if (((QStringList)dataList.at(i)).count() > maxCol)
            maxCol = ((QStringList)dataList.at(i)).count();
    }
    // sets the column number of table to the maximum column count
    fileTable->setColumnCount(maxCol);
    // populating the whole table with the empty data items
    for (int i=0;i<fileTable->rowCount();i++)
    {
        for (int j=0;j<fileTable->columnCount();j++)
        {
            fileTable->setItem(i,j,new QTableWidgetItem(""));
        }
    }
    // populating the table with all of the real data items in the data list
    for (int i=0;i<fileTable->rowCount();i++)
    {
        for (int j=0;j<((QStringList)dataList.at(i)).count();j++)
        {
            fileTable->setItem(i,j,new QTableWidgetItem((QString)((QStringList)dataList.at(i)).at(j)));
        }
    }
}


inline bool openFile(const QString& filePath, QFile& file)
{
    file.setFileName(filePath);
    bool result = file.open(QIODevice::ReadOnly);
    if (false == result)
    {
        qDebug() << __FUNCTION__ << "Error - can't open file:" << filePath;
    }

    return result;
}

QList<QStringList> csvReader::readToList(const QString& filePath,
                                      const QString& separator,
                                      const QString& textDelimiter,
                                      QTextCodec* codec)
{
    QFile file;
    if (false == openFile(filePath, file))
    {
        return QList<QStringList>();
    }

    return readToList(file, separator, textDelimiter, codec);
}

QList<QStringList> csvReader::readToList(QIODevice &ioDevice,
                                      const QString &separator,
                                      const QString &textDelimiter,
                                      QTextCodec *codec)
{
    ReadToListProcessor processor;
    read(ioDevice, processor, separator, textDelimiter, codec);
    return processor.data;
}

/*! Function that really reads csv-data and transfer it's data to
* AbstractProcessor-based processor
* @input:
* \param ioDevice - IO Device containing the csv-formatted data
* \param processor - refernce to AbstractProcessor-based object
* \param separator - string or character that separate values in a row
* \param textDelimiter - string or character that enclose row elements
* \param codec - pointer to codec object that would be used for file reading
* \return bool result of read operation */
bool csvReader::read(QIODevice& ioDevice,
                         AbstractProcessor& processor,
                         const QString& separator,
                         const QString& textDelimiter,
                         QTextCodec* codec)
{
    if ( false == checkParams(separator) )
    {
        return false;
    }

    // Open IO Device if it was not opened
    if (false == ioDevice.isOpen() &&
            false == ioDevice.open(QIODevice::ReadOnly))
    {
        qDebug() << __FUNCTION__ << "Error - failed to open IO Device";
        return false;
    }

    QTextStream stream(&ioDevice);
    stream.setCodec(codec);

    // This list will contain elements of the row if its elements
    // are located on several lines
    QStringList row;

    ElementInfo elemInfo;
    bool result = true;
    while (false == stream.atEnd())
    {
        QString line = stream.readLine();
        processor.preProcessRawLine(line);
        QStringList elements = splitElements(
                  line, separator, textDelimiter, elemInfo);
        if (elemInfo.isEnded)
        {
            // Current row ends on this line. Check if these elements are
            // end elements of the long row
            if (row.isEmpty())
            {
                // No, these elements constitute the entire row
                if (false == processor.processRowElements(elements))
                {
                    result = false;
                    break;
                }
            }
            else
            {
                // Yes, these elements should be added to the row
                if (false == elements.isEmpty())
                {
                    row.last().append(elements.takeFirst());
                    row << elements;
                }

                if (false == processor.processRowElements(row))
                {
                    result = false;
                    break;
                }

                row.clear();
            }
        }
        else
        {
            // These elements constitute long row that lasts on several lines
            if (false == elements.isEmpty())
            {
                if (false == row.isEmpty())
                {
                    row.last().append(elements.takeFirst());
                }

                row << elements;
            }
        }
    }

    if (false == elemInfo.isEnded && false == row.isEmpty())
    {
        result = processor.processRowElements(row);
    }

    return result;
}

bool csvReader::checkParams(const QString& separator)
{
    if (separator.isEmpty())
    {
        qDebug() << __FUNCTION__ << "Error - separator could not be empty";
        return false;
    }

    return true;
}

QStringList csvReader::splitElements(const QString& line,
                                            const QString& separator,
                                            const QString& textDelimiter,
                                            ElementInfo& elemInfo)
{
    // If separator is empty, return whole line. Can't work in this
    // conditions!
    if (separator.isEmpty())
    {
        elemInfo.isEnded = true;
        return (QStringList() << line);
    }

    if (line.isEmpty())
    {
        // If previous row was ended, then return empty QStringList.
        // Otherwise return list that contains one element - new line symbols
        if (elemInfo.isEnded)
        {
            return QStringList();
        }
        else
        {
            return (QStringList() << LF);
        }
    }

    QStringList result;
    int pos = 0;
    while(pos < line.size())
    {
        if (elemInfo.isEnded)
        {
            // This line is a new line, not a continuation of the previous
            // line.
            // Check if element starts with the delimiter symbol
            int delimiterPos = line.indexOf(textDelimiter, pos);
            if (delimiterPos == pos)
            {
                pos = delimiterPos + textDelimiter.size();

                // Element starts with the delimiter symbol. It means that
                // this element could contain any number of double
                // delimiters and separator symbols. This element could:
                // 1. Be the first or the middle element. Then it should end
                // with delimiter and the seprator symbols standing next to each
                // other.
                int midElemEndPos = findMiddleElementPosition(
                                        line, pos, separator, textDelimiter);
                if (midElemEndPos > 0)
                {
                    int length = midElemEndPos - pos;
                    result << line.mid(pos, length);
                    pos = midElemEndPos +
                          textDelimiter.size() + separator.size();
                    continue;
                }

                // 2. Be The last element on the line. Then it should end with
                // delimiter symbol.
                if (isElementLast(line, pos, separator, textDelimiter))
                {
                    int length = line.size() - textDelimiter.size() - pos;
                    result << line.mid(pos, length);
                    break;
                }

                // 3. Not ends on this line
                int length = line.size() - pos;
                result << line.mid(pos, length);
                elemInfo.isEnded = false;
                break;
            }
            else
            {
                // Element do not starts with the delimiter symbol. It means
                // that this element do not contain double delimiters and it
                // ends at the next separator symbol.
                // Check if line contains separator symbol.
                int separatorPos = line.indexOf(separator, pos);
                if (separatorPos >= 0 )
                {
                    // If line contains separator symbol, then our element
                    // located between current position and separator
                    // position. Copy it into result list and move
                    // current position over the separator position.
                    result << line.mid(pos, separatorPos - pos);

                    // Special case: if line ends with separator symbol,
                    // then at the end of the line we have empty element.
                    if (separatorPos == line.size() - separator.size())
                    {
                        result << QString();
                    }

                    // Move the current position on to the next element
                    pos = separatorPos + separator.size();
                }
                else
                {
                    // If line do not contains separator symbol, then
                    // this element ends at the end of the string.
                    // Copy it into result list and exit the loop.
                    result << line.mid(pos);
                    break;
                }
            }
        }
        else
        {
            // This line is a continuation of the previous. Last element of the
            // previous line did not end. It started with delimiter symbol.
            // It means that this element could contain any number of double
            // delimiters and separator symbols. This element could:
            // 1. Ends somewhere in the middle of the line. Then it should ends
            // with delimiter and the seprator symbols standing next to each
            // other.
            int midElemEndPos = findMiddleElementPosition(
                                line, pos, separator, textDelimiter);
            if (midElemEndPos >= 0)
            {
                result << (LF + line.mid(pos, midElemEndPos - pos));
                pos = midElemEndPos + textDelimiter.size() + separator.size();
                elemInfo.isEnded = true;
                continue;
            }

            // 2. Ends at the end of the line. Then it should ends with
            // delimiter symbol.
            if (isElementLast(line, pos, separator, textDelimiter))
            {
                int length = line.size() - textDelimiter.size() - pos;
                result << (LF + line.mid(pos, length));
                elemInfo.isEnded = true;
                break;
            }

            // 3. Not ends on this line
            result << (LF + line);
            break;
        }
    }

    removeExtraSymbols(result, textDelimiter);
    return result;
}

int csvReader::findMiddleElementPosition(const QString& str,
                                              const int& startPos,
                                              const QString& separator,
                                              const QString& txtDelim)
{
    const int err = -1;
    if (str.isEmpty() ||
            startPos < 0 ||
            separator.isEmpty() ||
            txtDelim.isEmpty())
    {
        return err;
    }

    const QString elemEndSymbols = txtDelim + separator;
    int elemEndPos = startPos;
    while(elemEndPos < str.size())
    {
        // Find position of element end symbol
        elemEndPos = str.indexOf(elemEndSymbols, elemEndPos);
        if (elemEndPos < 0)
        {
            // This element could not be the middle element, becaise string
            // do not contains any end symbols
            return err;
        }

        // Check that this is really the end symbols of the
        // element and we don't mix up it with double delimiter
        // and separator. Calc number of delimiter symbols from elemEndPos
        // to startPos that stands together.
        int numOfDelimiters = 0;
        for (int pos = elemEndPos; startPos <= pos; --pos, ++numOfDelimiters)
        {
            QStringRef strRef = str.midRef(pos, txtDelim.size());
            if (QStringRef::compare(strRef, txtDelim) != 0)
            {
                break;
            }
        }

        // If we have odd number of delimiter symbols that stand together,
        // then this is the even number of double delimiter symbols + last
        // delimiter symbol. That means that we have found end position of
        // the middle element.
        if (numOfDelimiters % 2 == 1)
        {
            return elemEndPos;
        }
        else
        {
            // Otherwise this is not the end of the middle element and we
            // should try again
            elemEndPos += elemEndSymbols.size();
        }
    }

    return err;
}

bool csvReader::isElementLast(const QString& str,
                                  const int& startPos,
                                  const QString& separator,
                                  const QString& txtDelim)
{
    if (str.isEmpty() ||
            startPos < 0 ||
            separator.isEmpty() ||
            txtDelim.isEmpty())
    {
        return false;
    }

    // Check if string ends with text delimiter. If not, then this element
    // do not ends on this line
    if (false == str.endsWith(txtDelim))
    {
        return false;
    }

    // Check that this is really the end symbols of the
    // element and we don't mix up it with double delimiter.
    // Calc number of delimiter symbols from end
    // to startPos that stands together.
    int numOfDelimiters = 0;
    for (int pos = str.size() - 1; startPos <= pos; --pos, ++numOfDelimiters)
    {
        QStringRef strRef = str.midRef(pos, txtDelim.size());
        if (QStringRef::compare(strRef, txtDelim) != 0)
        {
            break;
        }
    }

    // If we have odd number of delimiter symbols that stand together,
    // then this is the even number of double delimiter symbols + last
    // delimiter symbol. That means that this element is the last on the line.
    if (numOfDelimiters % 2 == 1)
    {
        return true;
    }

    return false;
}

void csvReader::removeExtraSymbols(QStringList& elements,
                                       const QString& textDelimiter)
{
    if (elements.isEmpty())
    {
        return;
    }

    const QString doubleTextDelim = textDelimiter + textDelimiter;
    for (int i = 0; i < elements.size(); ++i)
    {
        QStringRef str(&elements.at(i));
        if (str.isEmpty())
        {
            continue;
        }

        int startPos = 0, endPos = str.size() - 1;

        // Find first non-space char
        for (;
             startPos < str.size() &&
                 str.at(startPos).category() == QChar::Separator_Space;
             ++startPos);

        // Find last non-space char
        for (;
             endPos >= 0 &&
                 str.at(endPos).category() == QChar::Separator_Space;
             --endPos);

        if (false == textDelimiter.isEmpty())
        {
            // Skip text delimiter symbol if element starts with it
            QStringRef strStart(&elements.at(i), startPos, textDelimiter.size());
            if ( strStart == textDelimiter)
            {
                startPos += textDelimiter.size();
            }

            // Skip text delimiter symbol if element ends with it
            QStringRef strEnd(&elements.at(i), endPos - textDelimiter.size() + 1,
                              textDelimiter.size());
            if (strEnd == textDelimiter)
            {
                endPos -= textDelimiter.size();
            }
        }

        if ( (0 < startPos || endPos < str.size() - 1) &&
             startPos <= endPos)
        {
            elements[i] = elements[i].mid(startPos, endPos - startPos + 1);
        }

        // Also replace double text delimiter with one text delimiter symbol
        elements[i].replace(doubleTextDelim, textDelimiter);
    }
}
