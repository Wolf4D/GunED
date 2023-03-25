#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gunspecxmlreader.h"
#include "propertywidget.h"
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

public slots:
    bool loadFile(const QString& fileName);
    void makeTabs(bool hasAlt);

private slots:
    void on_action_saveGun_triggered();
    void on_action_saveGunAs_triggered();
    void on_action_loadGun_triggered();
    void setDocumentModified(bool state = true);
    void on_actionAbout_triggered();
    void on_action_newGun_triggered();

private:
    bool maybeSave();
    bool saveFile();
    bool saveAs();
    void setCurrentFile(QString fileName);

    QString collectFileText();

    bool writeFile(const QString& fileName, const QString& fileContent);

    QFile * currentFile = new QFile();
    QString currentFileName = "";
    QTextStream dataStream;
    QTextEdit * miscTextEdit = nullptr;

    Ui::MainWindow *ui;
    QMultiMap<QString, PropertyWidget*> fields; // имя вкладки - поля
};
#endif // MAINWINDOW_H
