/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */
 
#pragma once

#if QT_VERSION <= QT_VERSION_CHECK(5, 12, 8)


// #include <QtCore/QString>

// #define Qt::SkipEmptyParts QString::SkipEmptyParts
// #define Qt::KeepEmptyParts QString::KeepEmptyParts

// namespace Qt {
// enum SplitBehavior
// {
//    KeepEmptyParts = QString::KeepEmptyParts,
//    SkipEmptyParts = QString::SkipEmptyParts
// };
// }

// #define QT_SKIP_ENUM_TYPE Qt::SplitBehavior
#define QT_SPLIT_ENUM_NAMESPACE QString
#else
#define QT_SPLIT_ENUM_NAMESPACE Qt
#endif