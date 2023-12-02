/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "IconCache.h"

#include <QtGui/QIcon>
#include <QtGui/QPixmap>

const std::unordered_map<EIcon, QString> IconCache::g_imagePathMap = {
    {EIcon::DELETE_FROM_DISK, QStringLiteral(":/icons/bin.png")}, {EIcon::CLOSE, QStringLiteral(":/icons/delete.png")},
    {EIcon::LOAD, QStringLiteral(":/icons/export.png")},
    {EIcon::CLEAR_LIST, QStringLiteral(":/icons/icons8-clear-60.png")},
    {EIcon::INFO, QStringLiteral(":/icons/icons8-info-30.png")},
    {EIcon::HOT_RELOAD, QStringLiteral(":/icons/icons8-restart-40.png")},
    {EIcon::EXECUTE, QStringLiteral(":/icons/icons8-zahnrad-64.png")}};

IconCache& IconCache::get()
{
   static IconCache s_instance;
   return s_instance;
}

IconCache::IconCache() {}


IconCache::~IconCache() {}

QPixmap* IconCache::pixmap(EIcon _pIcon) const
{
   auto pmIt = m_pixmapCache.find(_pIcon);
   if(pmIt == m_pixmapCache.end())
   {
      pmIt = m_pixmapCache.emplace(_pIcon, new QPixmap(g_imagePathMap.at(_pIcon))).first;
   }
   return pmIt->second;
}

QIcon* IconCache::icon(EIcon _pIcon) const
{
   auto pmIt = m_iconCache.find(_pIcon);
   if(pmIt == m_iconCache.end())
   {
      pmIt = m_iconCache.emplace(_pIcon, new QIcon(g_imagePathMap.at(_pIcon))).first;
   }
   return pmIt->second;
}
