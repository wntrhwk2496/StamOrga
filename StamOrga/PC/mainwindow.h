/*
*	This file is part of StamOrga
*   Copyright (C) 2017 Markus Schneider
*
*	This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 3 of the License, or
*   (at your option) any later version.
*
*	Foobar is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../connectionhandling.h"
#include "../globaldata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnSendData_clicked();
    void connectionFinished(qint32 result);
    void versionRequestFinished(qint32 result, QString msg);
    void propertyRequestFinished(qint32 result, quint32 value);
    void updatePasswordFinished(qint32 result);
    void getGamesListFinished(qint32 result);

    void on_btnUdpatePassword_clicked();

    void on_btnGetGamesList_clicked();

private:
    Ui::MainWindow *ui;

    ConnectionHandling *m_pConHandling;
    GlobalData          *m_pGlobalData;
};

#endif // MAINWINDOW_H
