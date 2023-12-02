/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <QtCore/QPluginLoader>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>

#include <memory>

// forward declarations
class QDragEnterEvent;
class QDropEvent;
class QLabel;
class SettingsManager;
class IChallengePlugin;
class Ui_CodeChallengeMaker;

class CodeChallengeMaker : public QMainWindow
{
   Q_OBJECT
public:
   CodeChallengeMaker(QWidget* parent = nullptr);
   virtual ~CodeChallengeMaker() override;

   CodeChallengeMaker(CodeChallengeMaker&)             = delete;
   CodeChallengeMaker(CodeChallengeMaker&&)            = delete;
   CodeChallengeMaker& operator=(CodeChallengeMaker&)  = delete;
   CodeChallengeMaker& operator=(CodeChallengeMaker&&) = delete;

protected:
   virtual void dragEnterEvent(QDragEnterEvent* _rEvent) override;
   virtual void dropEvent(QDropEvent* _rEvent) override;

private slots:
   void on_actionQuit_triggered();
   void on_actionLoadChallenge_triggered();
   void onTestingClicked();
   void onExecuteClicked();

private:
   void setupToolbar();
   void setEnableButtons(bool bEnabled);
   void unloadPlugin();
   void loadPlugin(const QString& _pPluginFilePath);
   void onInfoClicked() const;

private:
   std::unique_ptr<Ui_CodeChallengeMaker> ui;
   std::unique_ptr<SettingsManager> m_settings;
   QLabel* m_statusMessage = nullptr;
   QPluginLoader m_pluginLoader;
   IChallengePlugin* m_currentPlugin = nullptr;

   // actions
   std::vector<QAction*> m_actionList;
   QAction* m_openAction  = nullptr;
   QAction* m_closeAction = nullptr;
};
