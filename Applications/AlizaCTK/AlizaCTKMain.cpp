/*=========================================================================

  Copyright (c) Mikhail Isakov

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#include <QApplication>
#include <QSettings>
#include <QDir>
#include <QPalette>
#include <ctkDICOMBrowser.h>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  app.setOrganizationName("AlizaViewer");
  app.setOrganizationDomain("alizaviewer.de");
  app.setApplicationName("AlizaCTK");
  {
    QString theme;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       app.organizationName(), app.applicationName());
    const QString db = settings.value(QString("DatabaseDirectory"), QString("")).toString();
    theme = settings.value(QString("Theme"), QString("Dark Fusion")).toString();
    if (db.isEmpty())
    {
      const QString h = QDir::toNativeSeparators(
        QDir::homePath() + QDir::separator() + QString("AlizaCtkDB"));
      settings.setValue(QString("DatabaseDirectory"), h);
      settings.sync();
    }
    if (theme==QString("Dark Fusion"))
    {
      QPalette darkPalette;
      const QColor bg_color = QColor(0x53,0x59,0x60);
      darkPalette.setColor(QPalette::Window,          bg_color);
      darkPalette.setColor(QPalette::WindowText,      Qt::white);
      darkPalette.setColor(QPalette::Text,            Qt::white);
      darkPalette.setColor(QPalette::Disabled,        QPalette::WindowText, Qt::gray);
      darkPalette.setColor(QPalette::Disabled,        QPalette::Text, Qt::gray);
      darkPalette.setColor(QPalette::Base,            bg_color);
      darkPalette.setColor(QPalette::AlternateBase,   bg_color);
      darkPalette.setColor(QPalette::ToolTipBase,     QColor(0x30, 0x39, 0x47));
      darkPalette.setColor(QPalette::ToolTipText,     Qt::white);
      darkPalette.setColor(QPalette::Button,          bg_color);
      darkPalette.setColor(QPalette::ButtonText,      Qt::white);
      darkPalette.setColor(QPalette::BrightText,      Qt::white);
      darkPalette.setColor(QPalette::Link,            Qt::darkBlue);
      darkPalette.setColor(QPalette::Highlight,       Qt::lightGray);
      darkPalette.setColor(QPalette::HighlightedText, Qt::black);
#if (QT_VERSION >= 0x050000)
      app.setStyle(QString("Fusion"));
#else
      app.setStyle(QString("Plastique"));
#endif
      app.setPalette(darkPalette);
      /*
      settings.setFallbacksEnabled(true);
      settings.beginGroup(QString("GlobalSettings"));
      double fpt = settings.value(QString("ptFontSize"), -1.0).toDouble();
      settings.endGroup();
      if (fpt <= 0.0)
      {
        QFont f = app.font();
        fpt = f.pointSizeF();
        if (fpt > 0)
        {
            f.setPointSizeF(fpt);
            app.setFont(f);
            settings.beginGroup(QString("GlobalSettings"));
            settings.setValue(QString("ptFontSize"), QVariant(fpt));
            settings.endGroup();
            settings.sync();
        }
      }
      */
    }
  }
  ctkDICOMBrowser DICOMBrowser;
  DICOMBrowser.setDatabaseDirectorySettingsKey("DatabaseDirectory");
  DICOMBrowser.show();
  return app.exec();
}
