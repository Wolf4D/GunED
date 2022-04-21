#include "gunspecreader.h"

GunspecReader::GunspecReader(QObject *parent) : QObject(parent)
{

}

bool GunspecReader::readGunspec(QString cfile, QList<PropertyWidget *> &widgets, QString &unparsed)
{
    QStringList fileLines = cfile.split('\n');

    foreach (QString line, fileLines)
        if (line.isEmpty())
            fileLines.removeOne(line);
        else
            if (line[0] == ';')
                fileLines.removeOne(line);


    QMap<QString, QVariant> parameters;
    foreach (QString line, fileLines)
    {
        QStringList splitResult = line.split("=");
        if (splitResult.length()==2)
        {
            parameters.insert((splitResult[0]).trimmed().toLower(), (splitResult[1]).trimmed().toLower());
            //fileLines.removeOne(line);
        }
        else
           unparsed += line + "\n";
    }

    foreach (PropertyWidget * widget, widgets)
    {
        PropertyField propertyField = widget->getPropertyField();
        if (parameters.contains(propertyField.cmd))
        {
            propertyField.enabled = true;
            propertyField.currentValue = parameters.value(propertyField.cmd);
            parameters.remove(propertyField.cmd);
        }
        else
            propertyField.enabled = false;

        widget->setPropertyField(propertyField);
    }

    for(int i=0; i<parameters.count(); i++)
                  unparsed += parameters.keys().at(i) + " = " + parameters.value(parameters.keys().at(i)).toString() + "\n";

    return true;
}
