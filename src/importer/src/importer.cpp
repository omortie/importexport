#include "importer.h"

Importer::Importer(QSqlTableModel *model)
{
    setModel(model);
    settings->setModel(model);
    logger = new Logger(settings);
}

Importer::Importer()
{
}

void Importer::setModel(QSqlTableModel *imodel)
{
    model=imodel;
}

bool Importer::execy(Importer::Modes mod)
{
    // Takes the most important settings and parameters necessary for import operation from ImportSettings class
    // that populated while the ImportWizard was running
    key = settings->getKeyMap();
    skip = settings->getSkipRows();
    endpoint = settings->getImportRowCount();
    keyField = settings->getKeyField();
    fileTable = settings->getDataTable();
    bool result = false;
    logger->initLog();

    // Fetches the model until the end of it if the records number exceeds 256 rows
    while(model->canFetchMore())
        model->fetchMore();

    initHash(keyField);

    // Will determine the mode of import by input parameter : mod
    switch (mod) {
    case InsertAll:
        for(int i=skip;i<endpoint;i++)
            insertRecord(i);
        result = true;
        break;
    case InsertNew:
        for(int i=skip;i<endpoint;i++)
        {
            int existIndex=Finder(fileTable->item(i,key.value(keyField))->text(),keyField);
            if (existIndex != -1)
                continue;
            insertRecord(i);
        }
        result = true;
        break;
    case Update:
        for(int i=skip;i<endpoint;i++)
        {
            int existIndex=Finder(fileTable->item(i,key.value(keyField))->text(),keyField);
            if (existIndex != -1)
                updateRecord(i,existIndex);
        }
        result = true;
        break;
    case UpdateOrInsert:
        for(int i=skip;i<endpoint;i++)
        {
            int existIndex=Finder(fileTable->item(i,key.value(keyField))->text(),keyField);
            if(existIndex != -1)
                updateRecord(i,existIndex);
            else
                insertRecord(i);
        }
        result = true;
        break;
    case Delete:
        for(int i=skip;i<endpoint;i++)
        {
            int existIndex = Finder(fileTable->item(i,key.value(keyField))->text(),keyField);
            if (existIndex != -1 && model->removeRow(existIndex))
                    logger->logRemove(existIndex,i);

        }
        result = true;
        break;
    case DeleteOrInsert:
        for(int i=skip;i<endpoint;i++)
        {
            int existIndex=Finder(fileTable->item(i,key.value(keyField))->text(),keyField);
            if (existIndex != -1 && model->removeRow(existIndex))
                    logger->logRemove(existIndex,i);
            else
                insertRecord(i);
        }
        result = true;
        break;
    default:
        result = false;
        break;
    }
    return result;
}

bool Importer::submit(QString &err)
{
    bool result;
    if (!settings->getValid(err))
    {
        logger->logSubmitStatus(err);
        result = false;
    }
    else
    {
        if (model->submitAll())
        {
            logger->logSubmitStatus("Submit Succeeded");
            result = true;
        }
        else
        {
            logger->logSubmitStatus(model->lastError().text());
            result = false;
        }
    }
    // commit logs to the file if the options were selected by user
    logger->commitLog();
    return result;
}

int Importer::Finder(const QString value, const int fieldIndex)
{
    // Using QHash faster search for searching on the database indexes
    Q_UNUSED(fieldIndex)
    if (hash.contains(value)){
        return hash.value(value);
    }
    else return -1;
}

bool Importer::insertRecord(const int &row)
{
    record=model->record(); // takes the field names of the model and populated the record by them
    record.setGenerated(0,false); // the first field is auto-increment provided by DBMS
    for(int j=0;j<=model->columnCount();j++)
    {
        if(key.value(j)==-1) record.setValue(model->headerData(j,Qt::Horizontal).toString(),"");
        else
            record.setValue(model->headerData(j,Qt::Horizontal).toString(),fileTable->item(row,key.value(j))->text());
    }
    if(model->insertRecord(-1,record)) // appends the record at the end of the model
    {
        logger->logInsert(row);
        return true;
    }
    return false;
}

bool Importer::updateRecord(const int &row, const int &index)
{
    record=model->record(); // takes the field names of the model and populated the record by them
    record.setGenerated(0,false); // the first field is auto-increment provided by DBMS
    for(int j=0;j<=model->columnCount();j++)
    {
        if(key.value(j)==-1)
            record.setValue(model->headerData(j,Qt::Horizontal).toString(),"");
        else
            record.setValue(model->headerData(j,Qt::Horizontal).toString(),fileTable->item(row,key.value(j))->text());
    }
    if(model->setRecord(index,record)) // sets the db record with index (index) with the new record populated from table widget data at (row)
    {
        logger->logUpdate(index,row);
        return true;
    }
    return false;
}

void Importer::initHash(int fieldIndex)
{
    hash.clear();
    for(int i=0;i<model->rowCount();i++)
        hash.insert(model->data(model->index(i,fieldIndex)).toString(),i);
}
