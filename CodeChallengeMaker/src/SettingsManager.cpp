/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "SettingsManager.h"

#include "AppConfig.h"

SettingsManager::SettingsManager()
{
   const QString appName = QString::fromStdString(AppConfig::getApplicationName());
   m_settings            = new QSettings(QSettings::IniFormat, QSettings::UserScope, appName, appName, this);
}

SettingsManager::~SettingsManager() {}

QVariant SettingsManager::GetSetting(const QString& pGroup, const QString& pSetting, const QVariant& pDefault) const
{
   QVariant result;
   if(pGroup.isEmpty() || pSetting.isEmpty())
   {
      assert(!pGroup.isEmpty());
      assert(!pSetting.isEmpty());
      // returning an empty variant instead of the default because the settings "path" is not valid
      return result;
   }

   m_settings->beginGroup(pGroup);
   result = m_settings->value(pSetting, pDefault);
   m_settings->endGroup();

   return result;
}

void SettingsManager::SetSetting(const QString& pGroup, const QString& pSetting, const QVariant& pValue)
{
   if(pGroup.isEmpty() || pSetting.isEmpty())
   {
      assert(!pGroup.isEmpty());
      assert(!pSetting.isEmpty());
      return;
   }

   m_settings->beginGroup(pGroup);
   m_settings->setValue(pSetting, pValue);
   m_settings->endGroup();
}

void SettingsManager::sync()
{
   m_settings->sync();
}