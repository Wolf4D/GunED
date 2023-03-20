#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QFrame>
#include "propertyfield.h"

#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>


class PropertyWidget : public QFrame
{
    Q_OBJECT

public:
    explicit PropertyWidget(QWidget *parent, const PropertyField& field);
    ~PropertyWidget();

    PropertyField getPropertyField();
    void setPropertyField(const PropertyField &value);

signals:
    void wasModified();

private slots:
    void pressed(bool state);
private:
    QString thisName;
    PropertyField propertyField;
    QLabel * label = nullptr;
    QLineEdit * edit = nullptr;
    QSpinBox * spin = nullptr;
    QCheckBox * box = nullptr;
    QPushButton * button = nullptr;

};

#endif // PROPERTYWIDGET_H
