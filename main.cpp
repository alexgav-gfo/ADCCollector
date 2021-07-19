/*Copyright (C) 2021 Daria Gurieva
 This file is part of ADCCollector <https://github.com/dGurieva/ADCCollector>.

 ADCCollector is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ADCCollector is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with ADCCollector. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include "settings.h"
#include "MainWindow.h"

/**
 * Точка входа в программу.
 * @param argc - число аргументов командной строки.
 * @param argv - массив аргументов.
 * @return - код выхода, 0 - если успешно.
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle("cleanlooks");
    QApplication::setApplicationName(Settings::instance().getApplicationName());
    QApplication::setOrganizationName(Settings::instance().getOrganizationName());
    QApplication::setApplicationVersion(Settings::instance().getApplicationVersion());
    MainWindow mainWindow;
    mainWindow.resize(1200, 800);
    mainWindow.show();
    return QApplication::exec();
}
