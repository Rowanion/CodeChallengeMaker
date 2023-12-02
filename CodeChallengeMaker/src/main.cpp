/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include <QtWidgets/QApplication>

#include "ui/CodeChallengeMaker.h"
#include "ui/StyleFusionDark.h"

int main(int argc, char* argv[])
{
   QApplication app(argc, argv);

   CodeChallengeMaker mainWindow;
   ui_style::setStyle();
   mainWindow.show();

   app.exec();
   return 0;
}
