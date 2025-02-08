//
// Created by Xandr on 2/7/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QPushButton>
#include <QStringListModel>
#include <QStandardItemModel>
#include <app/Chip8EmulatorQtWrapper.h>
#include <app/Drivers/GraphicsDrivers.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void setRegistersValues();

    void setRegisterValuesInTableView();

    void setProgramCounter();

    void viewMemoryValues();

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void updateRegisters();

private:
    QtRenderer* emulatorWidget;
    Ui::MainWindow *ui;
    Chip8EmulatorQtWrapper* chip8QtWrapper;

    QStringListModel* registerModel;
    QStringListModel* programCounterModel;
    QStringListModel* MemoryListViewModel;
    QStandardItemModel* RegisterTableModel;

    // QPushButton
};

#endif // MAINWINDOW_H
