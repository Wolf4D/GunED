#ifndef GUNSPECXMLREADER_H
#define GUNSPECXMLREADER_H

#include <QObject>
#include "propertyfield.h"

struct TabStruct
{
    QString tabName;
    QList<PropertyField> properties;
};

class GunspecXMLReader : public QObject
{
    Q_OBJECT
public:
    explicit GunspecXMLReader(QObject *parent = nullptr);

    bool readXML(QString cfile);

    QList<TabStruct> tabs;

signals:

private:
};

#endif // GUNSPECXMLREADER_H
