////////////////////////////////////////////////////////////////////////////////

#include "airmsg.h"
#include <QPushButton>
#include <QElapsedTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QThread>

////////////////////////////////////////////////////////////////////////////////

void airMsg(QString text, QColor color, bool addDelay)
{
    // This would be our message widget
    QPushButton * pushButton;

    // Finding parent for our button
    QWidget * parent = QApplication::activeWindow();

    if (parent!=nullptr)
        pushButton = new QPushButton( parent );
    else
    {
        // No luck, our parent is desktop window
        parent = QApplication::desktop()->screen();
        pushButton = new QPushButton( parent );
        pushButton->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    }

    // Required for colored apperance
    pushButton->setStyle(QStyleFactory::create("fusion"));

    // Sizes
    int x = parent->geometry().width();
    x = (x - (x/2)) - 140;
    int y = parent->geometry().height();
    y = (y - (y/2));

    pushButton->setGeometry(x,y,282,50);
    pushButton->setText(text);
    pushButton->setAutoFillBackground(true);

    // Colored apperance
    QPalette pal = pushButton->palette( );
    pal.setColor( QPalette::Button, color );
    pal.setColor( QPalette::Active, QPalette::Button, color );
    pal.setColor( QPalette::Inactive, QPalette::Button, color );
    pushButton->setPalette( pal );
    pushButton->update();

    pushButton->show();
    pushButton->setFocus();

    // Let our button be demonstrated (show won't work other way)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 10);

    // If we need some delay, then we'll have it
    if (addDelay)
    {
        QElapsedTimer timer;
        timer.start();
        do {
            // Don't let our app hang
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
            QThread::msleep(10);
        } while (timer.elapsed() < 200);
    }

    // Button will be removed from memory shortly
    pushButton->deleteLater();
}

////////////////////////////////////////////////////////////////////////////////
