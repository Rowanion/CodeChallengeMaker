/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "CodeChallengeMaker.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeData>
#include <QtCore/QStandardPaths>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>

#include "IChallengePlugin.h"
#include "IconCache.h"
#include "SettingsManager.h"
#include "StringConversion.h"
#include "ui_CodeChallengeMaker.h"
#include <SystemTraits.h>
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

CodeChallengeMaker::CodeChallengeMaker(QWidget* parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui_CodeChallengeMaker>())
    , m_settings(std::make_unique<SettingsManager>())
{
   ui->setupUi(this);
   m_statusMessage = new QLabel(this);
   ui->statusbar->addWidget(m_statusMessage);
   setupToolbar();

   // disable this for now
   setAcceptDrops(false);
}

CodeChallengeMaker::~CodeChallengeMaker()
{
   m_settings->sync();
};

void CodeChallengeMaker::dragEnterEvent(QDragEnterEvent* _rEvent)
{
   if(_rEvent->mimeData()->hasUrls() && _rEvent->mimeData()->urls().size() == 1)
   {
      _rEvent->acceptProposedAction();
      return;
   }

   QWidget::dragEnterEvent(_rEvent);
}

void CodeChallengeMaker::dropEvent(QDropEvent* _rEvent)
{
   if(_rEvent->mimeData()->hasUrls() && _rEvent->mimeData()->urls().size() == 1)
   {
      // TODO: implement multiple URL drop off
      const QUrl url = _rEvent->mimeData()->urls()[0];
      if(url.isLocalFile())
      {
         QFileInfo info(url.toLocalFile());
         if(info.isFile() && info.exists()
             && (info.suffix() == QStringLiteral("mp4") || info.suffix() == QStringLiteral("mkv")))
         {
            _rEvent->acceptProposedAction();
         }

         QWidget::dropEvent(_rEvent);
         return;
      }
      else  // remote URL
      {
         // TODO

         _rEvent->accept();
      }
   }
}

void CodeChallengeMaker::onTestingClicked()
{
   ui->pte_input->appendPlainText("WIP");
   ui->pte_output->appendPlainText("WIP");
}

void CodeChallengeMaker::onExecuteClicked()
{
   //
   const ChallengeResult result = m_currentPlugin->runChallenge(ui->pte_input->toPlainText());

   if(!result.errorStack.isEmpty())
   {
      ui->pte_output->setPlainText("Errors occured: ");
      ui->pte_output->setPlainText(result.errorStack.join("\n"));
      return;
   }

   ui->pte_output->setPlainText(result.output);
}

void CodeChallengeMaker::setupToolbar()
{
   // disable context menu on toolbar to prevent disabling it
   ui->toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
   ui->menubar->setContextMenuPolicy(Qt::PreventContextMenu);
   ui->toolbar->toggleViewAction()->setEnabled(false);

   m_openAction  = ui->toolbar->addAction(*IconCache::get().icon(EIcon::LOAD), QStringLiteral("Load challenge plugin"),
        [this]() { on_actionLoadChallenge_triggered(); });
   m_closeAction = ui->toolbar->addAction(
       *IconCache::get().icon(EIcon::CLOSE), QStringLiteral("Unload plugin"), [this]() { unloadPlugin(); });
   ui->toolbar->addSeparator();
   m_actionList.emplace_back(ui->toolbar->addAction(
       *IconCache::get().icon(EIcon::EXECUTE), QStringLiteral("Run plugin"), [this]() { onExecuteClicked(); }));
   m_actionList.emplace_back(ui->toolbar->addAction(
       *IconCache::get().icon(EIcon::INFO), QStringLiteral("Challenge Info"), [this]() { onInfoClicked(); }));
   m_actionList.emplace_back(ui->toolbar->addAction(*IconCache::get().icon(EIcon::HOT_RELOAD),
       QStringLiteral("Hot-reload plugin [WIP]"), [this]() { onTestingClicked(); }));

   m_openAction->setEnabled(true);
   m_closeAction->setEnabled(false);
   setEnableButtons(false);
}

void CodeChallengeMaker::setEnableButtons(bool bEnabled)
{
   for(auto& action : m_actionList)
   {
      action->setEnabled(bEnabled);
   }
}

void CodeChallengeMaker::unloadPlugin()
{
   m_pluginLoader.unload();
   m_currentPlugin = nullptr;
   m_statusMessage->clear();
   setEnableButtons(false);
   m_openAction->setEnabled(true);
   m_closeAction->setEnabled(false);
}

void CodeChallengeMaker::loadPlugin(const QString& _pPluginFilePath)
{
   if(m_currentPlugin)
   {
      unloadPlugin();
   }

   m_pluginLoader.setFileName(_pPluginFilePath);
   if(!m_pluginLoader.load())
   {
      m_statusMessage->setText(QStringLiteral("Failed to load plugin: ") + m_pluginLoader.errorString());
      return;
   }
   m_currentPlugin = qobject_cast<IChallengePlugin*>(m_pluginLoader.instance());

   if(m_currentPlugin)
   {
      m_statusMessage->setText(QStringLiteral("Plugin: ") + m_currentPlugin->challengeTitle());
      setEnableButtons(true);
      m_openAction->setEnabled(false);
      m_closeAction->setEnabled(true);

      ui->pte_output->setPlainText(m_currentPlugin->challengeDescription());
      if(!m_currentPlugin->challengeInput().isEmpty())
      {
         ui->pte_input->setPlainText(m_currentPlugin->challengeInput());
      }
   }
}

void CodeChallengeMaker::onInfoClicked() const
{
   if(!m_currentPlugin->challengeDescription().isEmpty())
   {
      ui->pte_output->setPlainText(m_currentPlugin->challengeDescription());
   }
}

void CodeChallengeMaker::on_actionQuit_triggered()
{
   qApp->quit();
}

void CodeChallengeMaker::on_actionLoadChallenge_triggered()
{
   const QString lastPluginFolder =
       m_settings->GET_STR_SETTING("general", "lastPluginFolder", QApplication::applicationDirPath());

   QString filter;
   if(platform::isWindowsOS())
   {
      filter = QStringLiteral("Plugins (*.dll)");
   }
   else if(platform::isLinuxOS())
   {
      filter = QStringLiteral("Plugins (*.so)");
   }

   const QString pluginFilePath =
       QFileDialog::getOpenFileName(this, QStringLiteral("Load challenge plugin ..."), lastPluginFolder, filter);

   if(pluginFilePath.isEmpty())
   {
      return;
   }

   m_settings->SET_SETTING("general", "lastPluginFolder", QFileInfo(pluginFilePath).absolutePath());
   m_settings->sync();

   loadPlugin(pluginFilePath);
}
