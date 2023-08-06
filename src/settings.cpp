#include "settings.h"

//xml Port
//Initializing the QSetting instance to extract data and parameters from .xml file
static QSettings::Format xmlFormat;
static QSettings *settings;
void initXml()
{
    // Customizing the QSettings and preparing it to read xml file formats and extract
    // default settings parameters , it will send the functions readXmlFile
    // and writeXmlFile to the QSettings registerFormat
    xmlFormat = QSettings::registerFormat("xml",readXmlFile,writeXmlFile);
    // Sets the folder to search for desired xml file ($$PWD/Xml)
    QSettings::setPath(xmlFormat,QSettings::UserScope,"Xml");
    // Creates a QSettings based on the initialized settings and customized xml format
    settings = new QSettings(xmlFormat,QSettings::UserScope,"Pars Process","ImportExport");
}

/*! It will specify the method of reading parameters from xml files and populates
 * a SettingsMap <string,string> once , then when user wants a setting it will search
 * in the constructed map to find the related value
 * \param device : the IODevice sent by the QSettings based on the parameters of it's constructor
 * specifying the path of the xml file
 * \param map : a QMap<QString,QString> sent by the QSetting to be populated by the values and the keys of the respected xml file */
bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QXmlStreamReader xmlReader(&device);

    QString currentElementName;
    while(!xmlReader.atEnd())
    {
        xmlReader.readNext();
        while(xmlReader.isStartElement())
        {
            if(xmlReader.name() == "ImportExport")
            {
                xmlReader.readNext();
                continue;
            }
            if(!currentElementName.isEmpty())
                currentElementName += "/";
            currentElementName += xmlReader.name().toString();
            xmlReader.readNext();
        }
        if(xmlReader.isEndElement()){
            currentElementName.clear();
            continue;
        }
        if(xmlReader.isCharacters() && !xmlReader.isWhitespace())
        {
            QString key = currentElementName;
            QString value = xmlReader.text().toString();
            map[key] = value;
            currentElementName.clear();
        }
    }
    if(xmlReader.hasError())
        return false;
    return true;
}

/*! in this application we don't need to write anything on xml files so we literally
 * cleared any command in this function , it always returns true */
bool writeXmlFile(QIODevice& device,const QSettings::SettingsMap &map)
{
    Q_UNUSED(device)
    Q_UNUSED(map)

    return true;
}

void ImportSettings::setModel(QSqlQueryModel *imodel)
{
    model = imodel;
    for (int i=0;i<model->columnCount();i++){
        headers.insert(i,model->headerData(i,Qt::Horizontal).toString());
    }
}

bool ImportSettings::convertFileToTable()
{
    if (getFilePath() == "") return false;
    QFile file(getFilePath());
    if (!file.exists()) return false;
    //using the appropriate reader classes to parse file
    bool result = false;
    if (getFileType() == "csv"){
        csvReader reader(getFilePath(),getSeparator(),getQuote(),getCodec());
        setDataTable(reader.getTable());
        result = reader.getSuccess();
    }
    setConverted(result);
    return result;
}

/*! perform setting all of the extracted parameters from xml file has been set successfully to their own correspondent variables
 * \return true if all setting operations were successfull or vise versa */
bool ImportSettings::setParameters()
{
    if(!settings->value("filePath").isNull()) setFilePath(settings->value("filePath").toString());
    if(!settings->value("separator").isNull())setSeparator(settings->value("separator").toString());
    if(!settings->value("quote").isNull()) setQuote(settings->value("quote").toString());
    if(!settings->value("skipRows").isNull()) setSkipRows(settings->value("skipRows").toInt());
    if(!settings->value("commitNumber").isNull()) setCommitNumber(settings->value("commitNumber").toInt());
    if(!settings->value("importRowCount").isNull()) setImportRowCount(settings->value("importRowCount").toInt());
    if(!settings->value("fileType").isNull()) setFileType(settings->value("fileType").toString());
    if(!settings->value("sqlID").isNull()) setSqlId(settings->value("sqlID").toInt());
    if(!settings->value("importAll").isNull()) setImportAll(settings->value("importAll").toBool());
    if(!settings->value("commitAfter").isNull()) setCommitAfter(settings->value("commitAfter").toBool());
    if(!settings->value("logActive").isNull()) setLogActive(settings->value("logActive").toBool());
    if(!settings->value("logFilePath").isNull()) setLogFilePath(settings->value("logFilePath").toString());
    if(!settings->value("rwLog").isNull()) setRwLog(settings->value("rwLog").toBool());
    if(!settings->value("showLog").isNull()) setShowLog(settings->value("showLog").toBool());
    if(!settings->value("codec").isNull()) setCodec(settings->value("codec").toString());
    if(!settings->value("Technical").isNull()) setTechnical(settings->value("Technical").toBool());
    return true;
}

bool ImportSettings::getValid(QString &errorStr)
{
    errorStr = "";
    if (!converted){
        errorStr = "Source file couldn't be parsed , please select another file or check the contents of the file";
        return false;
    }
    int emptyFields = 0;
    for (int i = 0 ; i < keys.count() ; i++){
        if (keys.value(i) == -1) emptyFields++;
    }
    if (keys.count() - emptyFields < model->columnCount())
    {
        errorStr = "The number of columns being mapped is less than the number of database fields";
        return false;
    }
    return true;
}

//! Setting the extracted parameters from .xml file to the parameters in the settings class
bool ExportSettings::setParameters()
{
    if(!settings->value("filePath").isNull()) setFilePath(settings->value("filePath").toString());
    if(!settings->value("separator").isNull()) setSeparator(settings->value("separator").toString());
    if(!settings->value("quote").isNull()) setQuote(settings->value("quote").toString());
    if(!settings->value("skipRows").isNull()) setSkipRow(settings->value("skipRows").toInt());
    if(!settings->value("fileType").isNull()) setFileType(settings->value("fileType").toString());
    if(!settings->value("openAfter").isNull()) setOpenAfter(settings->value("openAfter").toBool());
    if(!settings->value("gotoFirst").isNull()) setGotoFirstRecord(settings->value("gotoFirst").toBool());
    if(!settings->value("currentOnly").isNull()) setCurrentOnly(settings->value("currentOnly").toBool());
    if(!settings->value("exportOnly").isNull()) setExportOnly(settings->value("exportOnly").toInt());
    if(!settings->value("exportAll").isNull()) setExportAll(settings->value("exportAll").toBool());
    if(!settings->value("exportHeaders").isNull()) setExportHeaders(settings->value("exportHeaders").toBool());
    if(!settings->value("Header").isNull()) setHeader(settings->value("Header").toString());
    if(!settings->value("Footer").isNull()) setFooter(settings->value("Footer").toString());
    return true;
}
