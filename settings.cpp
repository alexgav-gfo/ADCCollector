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

#include "settings.h"

/**
 * Сохраняет настройки одного канала.
 * @param channelView - настройки канала.
 * @param numberOfChannel - номер канала.
 */
void Settings::saveChannelSettings(ChannelView *channelView, int numberOfChannel) {
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    QString group = QString("channel_%1").arg(numberOfChannel);
    settings.beginGroup(group);
    settings.setValue("enabled", channelView->enabled);
    settings.setValue("name", channelView->name);
    settings.setValue("color_of_text", channelView->colorOfText);
    settings.setValue("color_of_graph", channelView->colorOfGraph);
    settings.setValue("color_of_grid", channelView->colorOfGrid);
    settings.setValue("save_binary_data", channelView->saveBinaryData);
    settings.setValue("save_text_data", channelView->saveTextData);
}

/**
 * Загружает последние настройки об одном канале.
 * @param numberOfChannel - номер канала.
 * @return - настройки канала под полученным номером.
 */
ChannelView Settings::loadChannelSettings(int numberOfChannel) {
    ChannelView channelView;
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    QString group = QString("channel_%1").arg(numberOfChannel);
    channelView.enabled = settings.value(group + "/enabled", true).toBool();
    channelView.name = settings.value(group + "/name", "Unnamed").toString();
    channelView.colorOfText = settings.value(group + "/color_of_text", colorOfText).value<QColor>();
    channelView.colorOfGraph = settings.value(group + "/color_of_graph", colorOfGraph).value<QColor>();
    channelView.colorOfGrid = settings.value(group + "/color_of_grid", colorOfGrid).value<QColor>();
    channelView.saveBinaryData = settings.value(group + "/save_binary_data", true).toBool();
    channelView.saveTextData = settings.value(group + "/save_text_data", true).toBool();

    return channelView;
}

/**
 * Сохраняет глобальные настройки программы.
 * @param globalView - глобальные настройки программы.
 */
void Settings::saveGlobalSettings(GlobalView *globalView) {
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    QString group = "global";
    settings.beginGroup(group);
    settings.setValue("data_root", globalView->dataRoot);
    settings.setValue("logging_root", globalView->loggingRoot);
    settings.setValue("frequency", globalView->frequency);
    settings.setValue("meaning_data_buffer", globalView->meaningDataBuffer);
    settings.setValue("data_in_one_file", globalView->dataInOneFile);
}

/**
 * Загружает последние глобальные настройки программы.
 * @return - глобальные настройки программы.
 */
GlobalView Settings::loadGlobalSettings() {
    GlobalView globalView;
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    QString group = "global";
    globalView.dataRoot = settings.value(group + "/data_root", "").toString();
    globalView.loggingRoot = settings.value(group + "/logging_root", "").toString();
    globalView.frequency = settings.value(group + "/frequency", 100).toInt();
    globalView.meaningDataBuffer = settings.value(group + "/meaning_data_buffer", 0).toInt();
    globalView.dataInOneFile = settings.value(group + "/data_in_one_file", false).toBool();
    return globalView;
}

/**
 * @return - имя приложения.
 */
QString Settings::getApplicationName() {
    return APPLICATION_NAME;
}

/**
 * @return - имя организации.
 */
QString Settings::getOrganizationName() {
    return  ORGANIZATION_NAME;
}

/**
 * @return - версию приложения.
 */
QString Settings::getApplicationVersion() {
    return APPLICATION_VERSION;
}

/**
 * @return - количество каналов.
 */
const int Settings::getNumOfChannels() {
    return NUM_OF_CHANNELS;
}

/**
 * @return - цвет текста у каналов.
 */
const QColor Settings::getColorOfText() {
    return colorOfText;
}

/**
 * @return - цвет сетки у каналов.
 */
const QColor Settings::getColorOfGrid() {
    return colorOfGrid;
}

/**
 * @return - цвет графика у каналов.
 */
const QColor Settings::getColorOfGraph() {
    return colorOfGraph;
}

/**
 * @return - массив настроек всех каналов.
 */
std::vector<ChannelView> Settings::loadAllChannelSettings() {
    std::vector<ChannelView> allChSets;
    for(int i = 0; i < NUM_OF_CHANNELS; i++) {
        allChSets.push_back(loadChannelSettings(i));
    }
    return allChSets;
}
