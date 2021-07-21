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

#include "channelsettingswidget.h"

/**
 * Конструктор виджета с настройками канала.
 * @param parent - указатель на дочерний виджет.
 */
ChannelSettingsWidget::ChannelSettingsWidget(QWidget *parent) : QWidget(parent) {
    enabledCheckBox = new QCheckBox(tr("Enabled"), this);
    connect(enabledCheckBox, &QCheckBox::toggled, this, &ChannelSettingsWidget::slotEnabled);
    saveBinaryData = new QCheckBox(tr("Save binary data"), this);
    connect(saveBinaryData, &QCheckBox::toggled, this, &ChannelSettingsWidget::slotSaveBinaryData);
    saveTextData = new QCheckBox(tr("Save text data"), this);
    connect(saveTextData, &QCheckBox::toggled, this, &ChannelSettingsWidget::slotSaveTextData);
    enabledCheckBox->setChecked(true);
    saveBinaryData->setChecked(true);
    saveTextData->setChecked(true);

    enabled = true;
    saveBinaryDataFlag = true;
    saveTextDataFlag = true;
    nameOfChannel = new QString(tr("Unnamed"));

    colorOfGrid = new QColor(Settings::instance().getColorOfGrid());
    colorOfGraph = new QColor(Settings::instance().getColorOfGraph());
    colorOfText = new QColor(Settings::instance().getColorOfText());

    name = new QLineEdit(*nameOfChannel, this);
    nameLabel = new QLabel(tr("Name: "), this);
    colorOfGraphBtn = new QPushButton(this);
    connect(colorOfGraphBtn, &QPushButton::clicked, this, &ChannelSettingsWidget::slotColorOfGraphBtn);
    colorOfGridBtn = new QPushButton(this);
    connect(colorOfGridBtn, &QPushButton::clicked, this, &ChannelSettingsWidget::slotColorOfGridBtn);
    colorOfTextBtn = new QPushButton(this);
    connect(colorOfTextBtn, &QPushButton::clicked, this, &ChannelSettingsWidget::slotColorOfTextBtn);
    colorOfGraphLabel = new QLabel(tr("Color of graph: "), this);
    colorOfGridLabel = new QLabel(tr("Color of grid: "), this);
    colorOfTextLabel = new QLabel(tr("Color of text: "), this);

    colorsLayout = new QHBoxLayout;
    colorsLayout->addWidget(colorOfGridLabel);
    colorsLayout->addWidget(colorOfGridBtn);
    colorsLayout->addWidget(colorOfTextLabel);
    colorsLayout->addWidget(colorOfTextBtn);
    colorsLayout->addWidget(colorOfGraphLabel);
    colorsLayout->addWidget(colorOfGraphBtn);

    nameLayout = new QHBoxLayout;
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(name);

    checkboxesLayout = new QHBoxLayout;
    checkboxesLayout->addWidget(enabledCheckBox);
    checkboxesLayout->addWidget(saveBinaryData);
    checkboxesLayout->addWidget(saveTextData);

    labels = new QVBoxLayout(this);
    labels->addLayout(nameLayout);
    labels->addLayout(checkboxesLayout);
    labels->addLayout(colorsLayout);
}

/**
 * Слот для кнопки выбора цвета сетки.
 */
void ChannelSettingsWidget::slotColorOfGridBtn() {
    makeColorFromColorDialog(colorOfGridBtn, colorOfGrid);
}

/**
 * Слот для кнопки выбора цвета графика.
 */
void ChannelSettingsWidget::slotColorOfGraphBtn() {
    makeColorFromColorDialog(colorOfGraphBtn, colorOfGraph);
}

/**
 * Слот для кнопки выбора цвета текста.
 */
void ChannelSettingsWidget::slotColorOfTextBtn() {
    makeColorFromColorDialog(colorOfTextBtn, colorOfText);
}

/**
 * Устанавливает цвет кнопки.
 * @param btn - кнопка, цвет которой устанавливается.
 * @param color - устанавливаемый цвет.
 */
void ChannelSettingsWidget::setColor(QPushButton *btn, QColor *color) {
    int red = color->red();
    int green = color->green();
    int blue = color->blue();
    QLatin1Char fill('0');
    QString btnColor = QString("background-color: #%1%2%3").arg(red, 2, 16, fill).arg(green, 2, 16, fill).arg(blue, 2,16, fill);
    btn->setStyleSheet(btnColor);
}

/**
 * Получает цвет из диалога выбора цвета и устанавливает его на нужную кнопку.
 * @param btn - кнопка, на которую нужно установить цвет.
 * @param changedColor - устанавливаемый цвет.
 */
void ChannelSettingsWidget::makeColorFromColorDialog(QPushButton *btn, QColor *changedColor) {
    QColor currColor = QColorDialog::getColor();

    if(currColor.isValid()) {
        setColor(btn, &currColor);
        *changedColor = currColor;
    }
}

/**
 * Слот для чекбокса, который разрешает или запрещает работу канала.
 */
void ChannelSettingsWidget::slotEnabled() {
    enabled = enabledCheckBox->isChecked();
}

/**
 * Слот для чекбокса, который устанавливает нужно ли записывать данные в бинарном виде.
 */
void ChannelSettingsWidget::slotSaveBinaryData() {
    saveBinaryDataFlag = saveBinaryData->isChecked();
}

/**
 * Слот для чекбокса, который устанавливает нужно ли записывать данные в текстовом виде.
 */
void ChannelSettingsWidget::slotSaveTextData() {
    saveTextDataFlag = saveTextData->isChecked();
}

/**
 * @return - разрешена ли работа канала.
 */
bool ChannelSettingsWidget::getEnabled() {
    return enabled;
}

/**
 * @return - цвет сетки.
 */
QColor ChannelSettingsWidget::getColorOfGrid() {
    return *colorOfGrid;
}

/**
 * @return - цвет графика.
 */
QColor ChannelSettingsWidget::getColorOfGraph() {
    return *colorOfGraph;
}

/**
 * @return - цвет текста.
 */
QColor ChannelSettingsWidget::getColorOfText() {
    return *colorOfText;
}

/**
 * @return - имя канала.
 */
QString ChannelSettingsWidget::getName() {
    return name->text();
}

/**
 * @return - сохранять ли данные в бинарном виде.
 */
bool ChannelSettingsWidget::getSaveBinaryDataFlag() {
    return saveBinaryDataFlag;
}

/**
 * @return - сохранять ли данные в текстовом виде.
 */
bool ChannelSettingsWidget::getSaveTextDataFlag() {
    return saveTextDataFlag;
}

/**
 * Устанавливает состояние разрешенности работы.
 * @param en - флаг, обозначающий разрешение работы канала.
 */
void ChannelSettingsWidget::setEnabled(bool en) {
    enabled = en;
    enabledCheckBox->setChecked(enabled);
}

/**
 * Устанавливает цвет сетки.
 * @param color - устанавливаемый цвет.
 */
void ChannelSettingsWidget::setColorOfGrid(QColor color) {
    *colorOfGrid = color;
    setColor(colorOfGridBtn, colorOfGrid);
}

/**
 * Устанавливает цвет графика.
 * @param color - устанавливаемый цвет.
 */
void ChannelSettingsWidget::setColorOfGraph(QColor color) {
    *colorOfGraph = color;
    setColor(colorOfGraphBtn, colorOfGraph);
}

/**
 * Устанавливает цвет текста.
 * @param color - устанавливаемый цвет.
 */
void ChannelSettingsWidget::setColorOfText(QColor color) {
    *colorOfText = color;
    setColor(colorOfTextBtn, colorOfText);
}

/**
 * Устанавливает имя канала.
 * @param color - устанавливаемое имя канала.
 */
void ChannelSettingsWidget::setName(QString nameStr) {
    *nameOfChannel = nameStr;
    name->setText(*nameOfChannel);
}

/**
 * Устанавливает нужно ли записывать данные в бинарном виде.
 * @param color - устанавливаемое состояние.
 */
void ChannelSettingsWidget::setSaveBinaryDataFlag(bool f) {
    saveBinaryDataFlag = f;
    saveBinaryData->setChecked(saveBinaryDataFlag);
}

/**
 * Устанавливает нужно ли записывать данные в текстовом виде.
 * @param color - устанавливаемое состояние.
 */
void ChannelSettingsWidget::setSaveTextDataFlag(bool f) {
    saveTextDataFlag = f;
    saveTextData->setChecked(saveTextDataFlag);
}
