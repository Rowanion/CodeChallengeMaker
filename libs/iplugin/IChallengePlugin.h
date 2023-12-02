/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <QtCore/QString>

#include <QtPlugin>

struct ChallengeResult
{
   QString output;
   // TODO: Add more fields
   QStringList errorStack;
};

class IChallengePlugin
{
public:
   virtual ~IChallengePlugin() = default;
   virtual QString challengeTitle() const = 0;
   virtual QString challengeDescription() const { return {}; };
   virtual QString challengeInput() const { return {}; };
   virtual ChallengeResult runChallenge(const QString& _pInput) const = 0;
};

#define IChallengePlugin_iid "com.CodeChallengeMaker.IChallengePlugin"
Q_DECLARE_INTERFACE(IChallengePlugin, IChallengePlugin_iid);
