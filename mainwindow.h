#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gunspecxmlreader.h"
#include "propertywidget.h"
#include <QFile>
#include <QTextStream>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void makeTabs(bool hasAlt);
    void on_action_saveGunAs_triggered();
    void on_action_loadGun_triggered();
    void setDocumentModified(bool state = true);
    void on_actionAbout_triggered();
    void on_action_newGun_triggered();


private:
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    bool wasModified = false;
    QFile * currentFile = new QFile();
    QString currentFileName = "New gunspec";
    QTextStream dataStream;
    QTextEdit * miscTextEdit = nullptr;

    QString filename;
    Ui::MainWindow *ui;
    GunspecXMLReader * xmlReader = nullptr;
    QMultiMap<QString, PropertyWidget*> fields; // имя вкладки - поля
};
#endif // MAINWINDOW_H
