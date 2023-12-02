/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "ChallengePlugin.h"

#include <QtCore/QFile>

ChallengePlugin::ChallengePlugin(QObject* _pParent /*= nullptr*/) {}

ChallengePlugin::~ChallengePlugin() = default;

QString ChallengePlugin::challengeTitle() const
{
   return QStringLiteral(PLUGIN_NAME);
}

QString ChallengePlugin::challengeDescription() const
{
   QFile f(QStringLiteral(":/" PLUGIN_NAME "/descriptionFile.txt"));
   if(!f.open(QFile::ReadOnly | QFile::Text))
   {
      qDebug() << "Failed to open description file";
      return {};
   }

   QTextStream in(&f);
   return in.readAll();
}

QString ChallengePlugin::challengeInput() const
{
   QFile f(QStringLiteral(":/" PLUGIN_NAME "/inputFile.txt"));
   if(!f.open(QFile::ReadOnly | QFile::Text))
   {
      qDebug() << "Failed to open input file";
      return {};
   }

   QTextStream in(&f);
   return in.readAll();
}

ChallengeResult ChallengePlugin::runChallenge(const QString& _pInput) const
{
   ChallengeResult result;
   result.output = QStringLiteral("ExamplePlugin: \n\n") + _pInput;

   return result;
}
