#include "propertywidget.h"
#include <QStyle>

PropertyWidget::PropertyWidget(QWidget *parent, const PropertyField& field) :
    QFrame(parent), propertyField(field)
{

    if (propertyField.isCurrentlyAlt) propertyField.cmd.prepend("alt");

    this->setFrameShadow(Sunken);
    this->setFrameShape(StyledPanel);

    QHBoxLayout * layout = new QHBoxLayout(this);
    this->setLayout(layout);

    QString toolTipText = QString("" + propertyField.cmd +": ").toUpper() + field.comment;

    switch (field.type)
    {
    case TextType:
    {
        label = new QLabel(this);
        label->setText(field.recName + ":");
        label->setToolTip(toolTipText);

        edit = new QLineEdit(this);
        edit->setPlaceholderText(field.currentValue.toString());
        edit->setToolTip(toolTipText);

        layout->addWidget(label);
        layout->addWidget(edit);

        connect(edit, SIGNAL(textChanged(QString)), this, SIGNAL(wasModified()));
    }
        break;
    case IntType:
    {
        label = new QLabel(this);
        label->setText(field.recName + ":");
        label->setToolTip(toolTipText);

        spin = new QSpinBox(this);
        spin->setToolTip(toolTipText);

        if ((field.maxValue.toInt()!=0) && (field.minValue!=field.maxValue))
        {
            spin->setMinimum(field.minValue.toInt());
            spin->setMaximum(field.maxValue.toInt());
            spin->setToolTip(QString("From ") + QString::number(field.minValue.toInt()) + " to " + QString::number(field.maxValue.toInt()));
        }

        spin->setValue(field.currentValue.toInt());

        layout->addWidget(label);
        layout->addWidget(spin);

        connect(spin, SIGNAL(valueChanged(QString)), this, SIGNAL(wasModified()));
    }
        break;

    case BoolType:
    {
        box = new QCheckBox(this);
        box->setText(field.recName);
        box->setChecked(field.currentValue.toBool());
        box->setToolTip(toolTipText);

        layout->addWidget(box);

        connect(box, SIGNAL(stateChanged(int)), this, SIGNAL(wasModified()));
    }
        break;
    default:
        break;
    }


    QSpacerItem * hspacer = new QSpacerItem(10, 10, QSizePolicy::Expanding);
    layout->insertItem(2, hspacer);


    button = new QPushButton(this);
    //button->setText("X");
    button->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    button->setMaximumWidth(30);
    button->setCheckable(true);

    layout->addWidget(button);

    if (!field.enabled)
    {
        button->setChecked(true);
        button->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        this->pressed(true);
    }

    connect(button, SIGNAL(toggled(bool)), this, SLOT(pressed(bool)));
    connect(button, SIGNAL(toggled(bool)), this, SIGNAL(wasModified()));

    if (field.persistent)
        button->hide();

    thisName = propertyField.cmd;

}

PropertyWidget::~PropertyWidget()
{
}

void PropertyWidget::pressed(bool state)
{
    propertyField.enabled = !state;
    if (box) box->setEnabled(!state);
    if (spin) spin->setEnabled(!state);
    if (label) label->setEnabled(!state);
    if (edit) edit->setEnabled(!state);

    if (state)
        button->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    else
        button->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));

}

PropertyField PropertyWidget::getPropertyField()
{
    switch (propertyField.type)
    {
    case TextType:
        propertyField.currentValue = edit->text();
        break;
    case IntType:
        propertyField.currentValue = spin->value();
        break;
    case BoolType:
        propertyField.currentValue = box->isChecked();
        break;
    }

    return propertyField;
}

void PropertyWidget::setPropertyField(const PropertyField &value)
{
    propertyField = value;
    thisName = propertyField.cmd;

    switch (propertyField.type)
    {
    case TextType:
        edit->setText(propertyField.currentValue.toString());
        break;
    case IntType:
        spin->setValue(propertyField.currentValue.toInt());
        break;
    case BoolType:
        box->setChecked(propertyField.currentValue.toBool());
        break;
    }


    button->setChecked(!propertyField.enabled);
    if (box) box->setEnabled(propertyField.enabled);
    if (spin) spin->setEnabled(propertyField.enabled);
    if (label) label->setEnabled(propertyField.enabled);
    if (edit) edit->setEnabled(propertyField.enabled);

    if ((propertyField.persistent) && (!propertyField.enabled))
        button->show();

}
