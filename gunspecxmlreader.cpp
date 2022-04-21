#include "gunspecxmlreader.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
GunspecXMLReader::GunspecXMLReader(QObject *parent) : QObject(parent)
{

}

bool GunspecXMLReader::readXML(QString cfile)
{
    QFile * file = new QFile("./gunspec.xml");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "FILE!";
        return false;
    }

    QXmlStreamReader xml(file);


    if (xml.readNextStartElement())
        if (xml.name()=="root")
            while (xml.readNextStartElement())
            {
                if (xml.name() == "tab")
                {
                    TabStruct currentTab;
                    currentTab.tabName = xml.attributes().at(0).value().toString();

                    while(xml.readNextStartElement())
                    {
                        if (xml.name() == "record")
                        {
                            PropertyField record;
                            record.recName = xml.attributes().at(0).value().toString();

                            while(xml.readNextStartElement())
                            {
                                if (xml.name() == "type")
                                {
                                    QString typeName = xml.readElementText();
                                    if (typeName=="text") record.type = TextType;
                                    if (typeName=="bool") record.type = BoolType;
                                    if (typeName=="int") record.type = IntType;
                                }

                                if (xml.name() == "default")  record.currentValue = xml.readElementText();
                                if (xml.name() == "minvalue") record.minValue = xml.readElementText();
                                if (xml.name() == "maxvalue") record.maxValue = xml.readElementText();
                                if (xml.name() == "hasAlt") record.hasAlt = QVariant(xml.readElementText()).toBool();
                                if (xml.name() == "persistent") record.persistent = QVariant(xml.readElementText()).toBool();
                                if (xml.name() == "enabled") record.enabled = QVariant(xml.readElementText()).toBool();
                                if (xml.name() == "comment") record.comment = QVariant(xml.readElementText()).toString();
                                if (xml.name() == "cmd") record.cmd = QVariant(xml.readElementText()).toString();

                            }

                            currentTab.properties.append(record);
                        }

                    }

                    tabs.append(currentTab);
                }
            }

    return true;
}
