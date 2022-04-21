#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gunspecxmlreader.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <QSet>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include "gunspecreader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    makeTabs(false);

    connect(ui->checkBox, SIGNAL(toggled(bool)), this, SLOT(makeTabs(bool)));

    //setWindowTitle("[*] - " + this->windowTitle()+ " v." + QString(VER));

    setCurrentFile("");
    statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeTabs(bool hasAlt)
{
    fields.clear();

    ui->tabWidget->hide();
    QApplication::processEvents();
    ui->tabWidget->clear();

    xmlReader = new GunspecXMLReader(this);
    xmlReader->readXML("");

    int tabCount = 0;
    foreach (TabStruct tab, xmlReader->tabs)
    {
        tabCount++;
        QScrollArea * central = new QScrollArea(this);
        QWidget * inner = new QWidget(this);
        QGridLayout * grid = new QGridLayout(central);
        grid->setColumnStretch(0, 1);
        grid->setColumnStretch(1, 2);
        grid->setColumnStretch(2, 1);
        grid->setColumnMinimumWidth(0, 250);
        inner->setLayout(grid);

        central->setWidgetResizable(true);
        central->setWidget(inner);

        if (hasAlt)
        {
            grid->setColumnMinimumWidth(1, 250);
            QLabel * label1 = new QLabel(central);
            label1->setText("<b>Main mode</b>");
            label1->setAlignment(Qt::AlignCenter);

            QLabel * label2 = new QLabel(central);
            label2->setText("<b>Alt mode</b>");
            label2->setAlignment(Qt::AlignCenter);

            grid->addWidget(label1, 0, 0);
            grid->addWidget(label2, 0, 1);
        }

        ui->tabWidget->addTab(central, tab.tabName);

        foreach (PropertyField field, tab.properties) {
            PropertyWidget * widget = new PropertyWidget(inner, field);
            grid->addWidget(widget, grid->rowCount(), 0);
            fields.insert(QChar(tabCount + 64) + tab.tabName,widget);// хак для сортировки
            connect(widget, SIGNAL(wasModified()), this, SLOT(setDocumentModified()));

            if (hasAlt)
                if (field.hasAlt)
                {
                    field.isCurrentlyAlt = true;
                    PropertyWidget * widgetAlt = new PropertyWidget(inner, field);
                    grid->addWidget(widgetAlt, grid->rowCount()-1, 1);
                    fields.insert(QChar(tabCount+ 64) +  tab.tabName,widgetAlt);
                    connect(widgetAlt, SIGNAL(wasModified()), this, SLOT(setDocumentModified()));
                }
        }

        QSpacerItem * hspacer = new QSpacerItem(10, 10, QSizePolicy::Expanding);
        grid->addItem(hspacer,0, 2);

        QSpacerItem * vspacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
        grid->addItem(vspacer, grid->rowCount(), 2);
    }

    miscTextEdit = new QTextEdit(this);
    ui->tabWidget->addTab(miscTextEdit, "Unsorted lines");

    ui->tabWidget->show();

}

void MainWindow::on_action_saveGunAs_triggered()
{
    QString fileContent = ";FPSC weapon gunspec\n;Made using GunED v0.1.\n\n";

    QList<QString> keys = fields.keys().toSet().toList();
    qSort(keys);
    foreach (QString tabName, keys)
    {
        QString tabContent;

        int fieldsCount = 0;
        foreach (PropertyWidget* widget, fields.values(tabName))
        {
            PropertyField propertyField = widget->getPropertyField();
            if (propertyField.enabled)
            {
                tabContent.prepend(propertyField.cmd + " = " + propertyField.currentValue.toString() + "\n");
                fieldsCount++;
            }
        }

        if (fieldsCount>0)
        {
            tabContent.prepend("\n;GUN " + QString(tabName).remove(0,1).toLower() + " settings\n\n");
            std::cout << tabContent.toStdString() << std::endl;
            std::flush(std::cout);

            fileContent +=  tabContent;
        }
    }

    if (miscTextEdit!=nullptr)
        fileContent.append(miscTextEdit->toPlainText());



    QFileDialog fileDialog(0, "Save gunspec file",  ".",  "TXT (*.txt)");
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    if (!fileDialog.exec()) return;
    if (fileDialog.selectedFiles().empty()) return;

    currentFileName = fileDialog.selectedFiles().at(0);




    QString fileName = currentFileName; //"D:/Pro/GunED/gunspectest.txt";

    if (!fileName.isEmpty())
    {
        currentFile->setFileName(fileName);
        currentFile->open(QIODevice::WriteOnly | QIODevice::Text);
        if (!currentFile->isOpen())
            QMessageBox::critical(0,"Error", "File not avalible!");

        dataStream.setDevice(currentFile);

        dataStream << fileContent;

        if (dataStream.status()!= QTextStream::Ok)
        {
            QMessageBox::critical(0,"Error", "File not written!");
            ui->statusbar->showMessage("File was NOT succesfully saved");
        }
        else
            ui->statusbar->showMessage("File was succesfully saved");

        currentFile->close();

        // TODO: А если нет?!
        setCurrentFile(currentFileName);
    }

}

void MainWindow::on_action_loadGun_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    "gunspec.txt", ".",
                                                    "TXT (*.txt)");
    if (fileName.isEmpty()) return;

    currentFile->setFileName(fileName);
    currentFile->open(QIODevice::ReadOnly | QIODevice::Text);
    dataStream.setDevice(currentFile);

    QString line = dataStream.readAll();

    if (dataStream.status()!= QTextStream::Ok)
    {
        QMessageBox::critical(0,"Error", "File not ok!");
        return;
    }

    setCurrentFile(fileName);

    // Определим, есть ли альтернативный режим
    if (line.contains("alt"))
    {
        ui->checkBox->setChecked(true);
        makeTabs(true);
    }
    else
    {
        ui->checkBox->setChecked(false);
        makeTabs(false);
    }


    QList<PropertyWidget*> widgets = fields.values();
    GunspecReader reader;
    QString unparsed;
    reader.readGunspec(line, widgets, unparsed);

    if (miscTextEdit!=nullptr)
        miscTextEdit->setText(unparsed);

    setDocumentModified(false);

}

void MainWindow::setDocumentModified(bool state)
{
    setWindowModified(state);
    wasModified = state;
}

bool MainWindow::maybeSave()
{
    if (wasModified) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("GunED"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return true;//save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}


bool MainWindow::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}


void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFileName = fileName;

    if (currentFileName.isEmpty())
        currentFileName = "untitled.txt";

    setWindowFilePath(currentFileName);
    setWindowTitle(currentFileName + "[*] - " + "FPSC Gun Editor (by Navy LiK), "+ " v." + QString(VER));
    setDocumentModified(false);
}

void MainWindow::on_actionAbout_triggered()
{
   QMessageBox::about(this, tr("About GunED"),
            QString("FPS Creator Gun Editor by NavY LiK (aka Ivan Klenov).\nWolf4D@list.ru")
                      + "\nCurrent version: " + QString(VER) +
                      "\nBuild date: " + QString(__DATE__) +
                      " at " + QString(__TIME__));
}

void MainWindow::on_action_newGun_triggered()
{
    currentFileName = "untitled.txt";
    makeTabs(ui->checkBox->isChecked());
}
