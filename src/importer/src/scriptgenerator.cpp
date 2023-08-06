#include "scriptgenerator.h"

QString ScriptGenerator::getFullScript()
{
    Generate();
    return fullScript;
}

void ScriptGenerator::Generate()
{
    // populating fullScript string with the parameters stored in settings class
    fullScript = "";
    fullScript += "settings.setFileType('" + settings->getFileType() + "')\n";
    if (settings->getFileType() == "csv"){
        fullScript += "settings.setSeparator('" + settings->getSeparator() + "')\n";
        fullScript += "settings.setQuote('" + settings->getQuote() + "')\n";
    }
    fullScript += "settings.setFilePath('" + settings->getFilePath() + "')\n";
//    fullScript += "settings.convertFileToTable()\n";
    for (int i = 0;i < settings->getKeyMap().count() ; i++){
        fullScript += "settings.insertKey(" + QString::number(i)
                + "," + QString::number(settings->getKeyMap().value(i)) + ")\n";
    }
    fullScript += "settings.setSkipRows(" + QString::number(settings->getSkipRows()) + ")\n";
//    fullScript += "settings.setImportRowCount(t.rowCount)\n";
    QString key = settings->getModel()->headerData(settings->getKeyField(),Qt::Horizontal).toString();
    fullScript += "settings.setKeyField('" + key + "')\n";
    QString sqlMode = "";
    switch (settings->getSqlId()) {
    case -2:
        sqlMode = "addall";
        break;
    case -3:
        sqlMode = "addnew";
        break;
    case -4:
        sqlMode = "update";
        break;
    case -5:
        sqlMode = "updateadd";
        break;
    case -6:
        sqlMode = "delete";
        break;
    case -7:
        sqlMode = "deleteadd";
        break;
    default:
        break;
    }
    fullScript += "settings.setCusMode('" + sqlMode + "')\n";
}
