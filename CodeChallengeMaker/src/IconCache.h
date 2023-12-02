/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <QtCore/QString>

#include <unordered_map>

class QIcon;
class QPixmap;

enum class EIcon
{
   DISCONNECTED,
   CONNECTING,
   CONNECTED,
   CONNECTION_ERROR,
   CLOSE,
   RESTART_ALL,
   CLEAR_LIST,
   CONNECT_TO_SERVER,
   DISCONNECT_FROM_SERVER,
   ADD_URL,
   SHUTDOWN_SERVER,
   DELETE_FROM_DISK,
   LOAD,
   EXTERN,
   TESTING,
   EXECUTE,
   INFO,
   HOT_RELOAD
};

class IconCache
{
public:
   static IconCache& get();
   ~IconCache();
   QPixmap* pixmap(EIcon _pIcon) const;
   QIcon* icon(EIcon _pIcon) const;

private:
   IconCache();
   mutable std::unordered_map<EIcon, QPixmap*> m_pixmapCache;
   mutable std::unordered_map<EIcon, QIcon*> m_iconCache;

   static const std::unordered_map<EIcon, QString> g_imagePathMap;
};
