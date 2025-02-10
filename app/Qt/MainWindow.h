//
// Created by Xandr on 2/7/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringListModel>
#include <QStandardItemModel>
#include <qtreewidget.h>
#include <app/Chip8EmulatorQtWrapper.h>
#include <app/Drivers/GraphicsDrivers.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    void populateTabsWidgets();

    explicit MainWindow(QWidget *parent = nullptr);

    void checkFocusAndReset();

    ~MainWindow();

    void printToConsole(const QString &text);

private:
    QtRenderer *emulatorWidget;
    Ui::MainWindow *ui;
    Chip8EmulatorQtWrapper *chip8QtWrapper;

    QStringListModel *registerModel;
    QStringListModel *programCounterModel;
    QStringListModel *MemoryListViewModel;
    QStandardItemModel *RegisterTableModel;
    QMap<int, QPushButton *> keyToButtonMap;

    void updateUI();

    void setupModels();

    void setRegistersValues();

    void setRegisterValuesInTableView();

    void setProgramCounter();

    void viewMemoryValues();

    void keyPressEvent(QKeyEvent *event) override;

    void StyleYourButton(QPushButton *button, bool style);

    void keyReleaseEvent(QKeyEvent *event) override;

    void onButtonPressed();

    void onButtonReleased();

    void setButtonAction(int type);

    void createQtButtonMapping();

    void connectButtonsToTheirOnCalls();

    void onRomSelected(QTreeWidgetItem *item, int colum);

    void onGamesSelected(QTreeWidgetItem *item, int colum);


    void updateRegisters();
};

#endif // MAINWINDOW_H
