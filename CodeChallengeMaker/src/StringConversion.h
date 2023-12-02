/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include <QtCore/QString>

#include <string>

#define STR_Q(str) QString::fromStdString(str)
#define Q_STR(qstr) qstr.toStdString()