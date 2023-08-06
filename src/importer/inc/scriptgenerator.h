#ifndef SCRIPTGENERATOR_H
#define SCRIPTGENERATOR_H

#include "settings.h"

//! Script Generator class will use the settings parameters stored in settings class as it feed and will generate correspondent script to the gui settings
class ScriptGenerator
{
public:
    /*! Script Generator constructor to define settings class stored parameters pointer */
    ScriptGenerator(ImportSettings *settings)
        :settings(settings) {}
    /*! this function is for accessing the generated scripts based on the parameters saved in the settings
     * it will call Generate() procedure with-in and returns the result
     * \return generated script based on the settings parameters  */
    QString getFullScript();

private:
    /*! This procedure will do the main duty of generating script */
    void Generate();

    //! pointer to the import settings
    ImportSettings *settings;
    QString fullScript = "";
    QString abstractScript = "";
};
#endif // SCRIPTGENERATOR_H
