#ifndef PROPERTYFIELD_H
#define PROPERTYFIELD_H

#include <QWidget>
#include <QVariant>
enum FieldType
{
    TextType = 0,
    IntType  = 1,
    BoolType = 2
};

struct PropertyField
{
    QString recName;
    FieldType type;
    QVariant currentValue;
    QVariant minValue;
    QVariant maxValue;
    bool hasAlt = false;
    bool persistent = false;
    bool enabled = true;
    QString comment;
    QString cmd;
    bool isCurrentlyAlt = false; // является ли это поле заполнено для alt-варианта
};


#endif // PROPERTYFIELD_H
