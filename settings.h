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

#ifndef ADCCOLLECTOR_SETTINGS_H
#define ADCCOLLECTOR_SETTINGS_H
#include <QColor>
#include <QSettings>
#include <QDebug>
#include <vector>

const QString ORGANIZATION_NAME = "GFO";
const QString APPLICATION_NAME = "ADCCollector";
const QString APPLICATION_VERSION = "1.0";
const int NUM_OF_CHANNELS = 4;
const QColor colorOfGrid = QColor(0, 75, 0);
const QColor colorOfGraph = QColor(250, 40, 40);
const QColor colorOfText = Qt::green;

/**
 * Глобальные настройки:
 */
struct GlobalView {
    QString dataRoot;
    QString loggingRoot;
    int frequency;
    int meaningDataBuffer;
    bool dataInOneFile;
    bool autoStart;
};

/**
 * Настройки канала:
 */
struct ChannelView {
    QColor colorOfGrid;
    QColor colorOfText;
    QColor colorOfGraph;
    QString name;
    bool enabled;
    bool saveBinaryData;
    bool saveTextData;
};

/**
 * Настройки программы:
 */
class Settings {

public:
    static Settings& instance() {
        static Settings singleInstance;
        return singleInstance;
    }
    void saveGlobalSettings(GlobalView *globalView);
    GlobalView loadGlobalSettings();
    void saveChannelSettings(ChannelView *channelView, int numberOfChannel);
    ChannelView loadChannelSettings(int numberOfChannel);
    std::vector<ChannelView> loadAllChannelSettings();
    QString getApplicationName();
    QString getOrganizationName();
    QString getApplicationVersion();
    const int getNumOfChannels();
    const QColor getColorOfGrid();
    const QColor getColorOfGraph();
    const QColor getColorOfText();

private:
    Settings() {}
    Settings(const Settings& root);
    Settings& operator=(const Settings&);
};

#endif //ADCCOLLECTOR_SETTINGS_H
