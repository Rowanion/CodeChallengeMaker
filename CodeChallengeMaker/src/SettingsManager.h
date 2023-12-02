/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <QtCore/QMetaProperty>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <vector>

class QSettings;

#define GET_T_SETTING(grp, key, def, type) GetSetting(grp, key, QVariant::fromValue<type>(def)).value<type>()
#define GET_UINT_SETTING(grp, key, def) GetSetting(grp, key, def).toUInt()
#define GET_INT_SETTING(grp, key, def) GetSetting(grp, key, def).toUInt()
#define GET_STR_SETTING(grp, key, def) GetSetting(grp, key, def).toString()
#define SET_SETTING(grp, key, val) SetSetting(grp, key, val)

class SettingsManager : public QObject
{
   Q_OBJECT
public:
   SettingsManager();
   virtual ~SettingsManager() override;

   QVariant GetSetting(const QString& pGroup, const QString& pSetting, const QVariant& pDefault) const;
   void SetSetting(const QString& pGroup, const QString& pSetting, const QVariant& pValue);

public slots:
   void sync();

signals:
   void resetLayout();

private:
   QSettings* m_settings = nullptr;
};
