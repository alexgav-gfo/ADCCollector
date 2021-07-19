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

#ifndef ADCCOLLECTOR_CHANNELSETTINGSWIDGET_H
#define ADCCOLLECTOR_CHANNELSETTINGSWIDGET_H
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QBoxLayout>
#include <QDebug>
#include <vector>
#include "settings.h"

/**
 * Виджет с настройками об одном канале.
 */
class ChannelSettingsWidget : public QWidget {
Q_OBJECT
public:
    explicit ChannelSettingsWidget(QWidget *parent = nullptr);

    void setEnabled(bool en);
    void setColorOfGrid(QColor color);
    void setColorOfGraph(QColor color);
    void setColorOfText(QColor color);
    void setName(QString nameStr);
    void setSaveBinaryDataFlag(bool f);
    void setSaveTextDataFlag(bool f);
    bool getEnabled();
    QColor getColorOfGrid();
    QColor getColorOfGraph();
    QColor getColorOfText();
    QString getName();
    bool getSaveBinaryDataFlag();
    bool getSaveTextDataFlag();

private:
    QCheckBox *enabledCheckBox;
    QCheckBox *saveBinaryData;
    QCheckBox *saveTextData;

    QString *nameOfChannel;
    QColor *colorOfGrid;
    QColor *colorOfGraph;
    QColor *colorOfText;
    bool enabled;
    bool saveBinaryDataFlag;
    bool saveTextDataFlag;

    QPushButton *colorOfGridBtn;
    QPushButton *colorOfGraphBtn;
    QPushButton *colorOfTextBtn;

    QLineEdit *name;
    QLabel *nameLabel;
    QLabel *colorOfGridLabel;
    QLabel *colorOfGraphLabel;
    QLabel *colorOfTextLabel;

    QHBoxLayout *colorsLayout;
    QHBoxLayout *nameLayout;
    QHBoxLayout *checkboxesLayout;
    QVBoxLayout *labels;

    void makeColorFromColorDialog(QPushButton *btn, QColor *changedColor);
    void setColor(QPushButton *btn, QColor *color);

private slots:
    void slotColorOfGridBtn();
    void slotColorOfGraphBtn();
    void slotColorOfTextBtn();
    void slotEnabled();
    void slotSaveBinaryData();
    void slotSaveTextData();
};


#endif //ADCCOLLECTOR_CHANNELSETTINGSWIDGET_H
