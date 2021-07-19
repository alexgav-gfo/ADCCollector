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

#include "directoryselector.h"

/**
 * Конструктор селектора каталогов.
 * @param name - имя селектора каталогов.
 * @param path - текущий путь, который будет содержать селектор.
 * @param parent - указатель на дочерний виджет.
 */
DirectorySelector::DirectorySelector(QString name, QString path, QWidget *parent) : QWidget(parent) {
    this->name = new QLabel(name, this);
    selectDirectoryBtn = new QToolButton(this);
    selectDirectoryBtn->setText("...");
    connect(selectDirectoryBtn, &QToolButton::clicked, this, &DirectorySelector::slotSelectDirectory);
    directoryPath = new QLineEdit(this);
    directoryPath->setText(path);
    this->path = path;

    connect(directoryPath, &QLineEdit::textChanged, this, &DirectorySelector::slotCheckDirectoryPath);

    labels = new QHBoxLayout(this);
    labels->addWidget(this->name);
    labels->addWidget(directoryPath);
    labels->addWidget(selectDirectoryBtn);
}

/**
 * Слот, проверяющий правильность введенного пути до каталога.
 * @param str - текущий введенный путь до каталога.
 */
void DirectorySelector::slotCheckDirectoryPath(const QString &str) {
    QDir directory(str);
    if(directory.exists()) {
        directoryPath->setStyleSheet("");
        path = str;
    } else {
        directoryPath->setStyleSheet("color: red");
        path = "";
    }
}

/**
 * Слот для кнопки выбора каталога.
 */
void DirectorySelector::slotSelectDirectory() {
    QString directory = QFileDialog::getExistingDirectory(this, tr("Selecting directory"));
    if(!directory.isEmpty()) {
        directoryPath->setText(directory);
    }
}

/**
 * @return - путь до выбранного каталога.
 */
QString DirectorySelector::getDirectoryPath() {
    return path;
}
