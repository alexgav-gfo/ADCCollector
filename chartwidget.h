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

#ifndef ADCCOLLECTOR_CHARTWIDGET_H
#define ADCCOLLECTOR_CHARTWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QMenu>
#include <QQueue>
#include <QPainter>
#include <QContextMenuEvent>
#include <QDebug>
#include <string>
#include <math.h>
#include "settings.h"

/**
 * Канал.
 */
class ChartWidget : public QWidget {
Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    void setMax(double max);
    void setMin(double min);
    void setSettings(ChannelView channelView);
    void addChartValue(double value);
    void clear();

private:
    const int Y_STEP_IN_PIXELS = 30;
    const int X_STEP_IN_PIXELS = 40;
    const int stepOfGraphInPixels = 2;

    QQueue<double> *data;
    QString name;
    double min;
    double max;
    double yStepInNumber;
    int widthOfChartWidget;
    int heightOfChartWidget;
    QColor colorOfGraph;
    QColor colorOfText;
    QColor colorOfGrid;
    QColor disabledColorOfText;
    QColor disabledColorOfGrid;
    int numberOfDrawingData;
    bool enabled;
    bool channelEnabled;
    bool autoMinMax;
    double selectedMin;
    double selectedMax;
    QString interval;

    QMenu *popupMenu;
    QAction *enabledAction;
    QAction *autoIntervalAction;
    QAction *oneHundredthIntervalAction;
    QAction *fiveHundredthIntervalAction;
    QAction *oneTenthIntervalAction;
    QAction *fiveTenthIntervalAction;
    QAction *oneIntervalAction;
    QAction *oneAndFiveTenthIntervalAction;
    QAction *twoAndFiveTenthIntervalAction;

    double findMin();
    double findMax();
    void drawGrid(QColor gridColor, QColor textColor);
    void drawGraph();
    void drawLabels();
    void initPopupMenu();
    void initActions();
    void setMinMax(double minVal, double maxVal);

protected:
    void paintEvent(QPaintEvent *) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void contextMenuEvent(QContextMenuEvent *ev) override;

private slots:
    void slotEnabledAction(bool en);
    void slotAutoMinMax();
    void slotAct0_01();
    void slotAct0_05();
    void slotAct0_1();
    void slotAct0_5();
    void slotAct1_0();
    void slotAct1_5();
    void slotAct2_5();

};


#endif //ADCCOLLECTOR_CHARTWIDGET_H
