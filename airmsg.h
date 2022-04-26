////////////////////////////////////////////////////////////////////////////////
///
/// A simple "flying" message for "Please wait" indication
///
////////////////////////////////////////////////////////////////////////////////

#ifndef AIRMSG_H
#define AIRMSG_H

#include <QString>
#include <QColor>

/// A simple stand-alone message window
/// \brief airMsg A simple stand-alone message window
/// \param text Text to indicate
/// \param color Color of window
/// \param addDelay Should we keep this message on screen for some time?
///
void airMsg(QString text, QColor color, bool addDelay = false);

#endif // AIRMSG_H

////////////////////////////////////////////////////////////////////////////////
