/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

#include "IChallengePlugin.h"
#include <QtPlugin>

class ChallengePlugin : public QObject, public IChallengePlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID IChallengePlugin_iid IChallengePlugin_iid)
   Q_INTERFACES(IChallengePlugin)

public:
   ChallengePlugin(QObject* _pParent = nullptr);
   virtual ~ChallengePlugin() override;

   /** IChallengePlugin interface */
   virtual QString challengeTitle() const override;
   virtual QString challengeDescription() const override;
   virtual QString challengeInput() const override;

public:
   virtual ChallengeResult runChallenge(const QString& _pInput) override;
   /** end of IChallengePlugin interface */

private:
};
