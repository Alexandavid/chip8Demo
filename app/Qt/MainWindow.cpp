//
// Created by Xandr on 2/7/2025.
#include "MainWindow.h"

#include <iostream>
#include "ui_mainwindow.h"

#define RELEASE 1
#define PRESS 0


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Qt Chip8 Emulator");
    emulatorWidget = new QtRenderer(this);
    emulatorWidget->setFixedSize(SUPER_CHIP_WIDTH, SUPER_CHIP_HEIGHT);

    // I need to figure out why I can't explicitly set a layout in QT Designer :/
    if (!ui->Emulator->layout()) {
        ui->Emulator->setLayout(new QVBoxLayout);
    }
    ui->Emulator->layout()->addWidget(emulatorWidget);

    chip8QtWrapper = new Chip8EmulatorQtWrapper(this, emulatorWidget);
    chip8QtWrapper->loadROM("../games/UFO.ch8");

    setupModels();

    createQtButtonMapping();
    connectButtonsToTheirOnCalls();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateUI);
    timer->start(100);

}

MainWindow::~MainWindow() {
    delete ui;
    delete registerModel;
    delete chip8QtWrapper;
}

void MainWindow::updateUI() {
    setRegistersValues();
    setProgramCounter();
    viewMemoryValues();
    setRegisterValuesInTableView();
}

void MainWindow::setupModels() {
    registerModel = new QStringListModel(this);
    ui->RegistersListView->setModel(registerModel);
    setRegistersValues();

    programCounterModel = new QStringListModel(this);
    ui->ProgramCounterListView->setModel(programCounterModel);
    setProgramCounter();

    MemoryListViewModel = new QStringListModel(this);
    ui->MemoryListView->setModel(MemoryListViewModel);
    viewMemoryValues();

    RegisterTableModel = new QStandardItemModel(NUM_OF_REGISTER, 2, this);
    RegisterTableModel->setHeaderData(0, Qt::Horizontal, "Register");
    RegisterTableModel->setHeaderData(1, Qt::Horizontal, "Value");
    ui->RegisterTableView->setModel(RegisterTableModel);
    ui->RegisterTableView->verticalHeader()->setVisible(false);
    ui->RegisterTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::setRegistersValues() {
    QStringList registerValues;
    CHIP8Specification* hardwareContext = chip8QtWrapper->getChip8unwrapped()->getChip8HardwareContext();

    for (int i = 0; i < NUM_OF_REGISTER; ++i) {
        QString part1 = QString("V[%1]").arg(i, 2, 16, QChar(' ')).toUpper();
        QString part2 = QString(" = 0x%1").arg(hardwareContext->V[i], 2, 16, QChar('0'));
        registerValues.append(part1 + part2);
    }

    registerModel->setStringList(registerValues);
}

void MainWindow::setRegisterValuesInTableView() {
    QStringList registerValues;

    RegisterTableModel->clear();
    RegisterTableModel->setColumnCount(2);
    RegisterTableModel->setHeaderData(0, Qt::Horizontal, "Register");
    RegisterTableModel->setHeaderData(1, Qt::Horizontal, "Value");

    CHIP8Specification* hardwareContext = chip8QtWrapper->getChip8unwrapped()->getChip8HardwareContext();
    for (int i = 0; i < NUM_OF_REGISTER; ++i) {
        QString regName = QString("V[%1]").arg(i, 2, 10, QChar(' '));  // Align register names
        QString regValue = QString("0x%1").arg(hardwareContext->V[i], 2, 16, QChar('0')).toUpper();  // Format hex values

        RegisterTableModel->setItem(i, 0, new QStandardItem(regName));
        RegisterTableModel->setItem(i, 1, new QStandardItem(regValue));
    }
}

void MainWindow::setProgramCounter() {
    QStringList registerValues;

    CHIP8Specification* hardwareContext = chip8QtWrapper->getChip8unwrapped()->getChip8HardwareContext();
    registerValues.append( QString("Program counter"));
    registerValues.append( QString("  0x%1").arg(hardwareContext->PC, 4, 16,  QChar('0')));

    programCounterModel->setStringList(registerValues);
}

void MainWindow::viewMemoryValues() {
    CHIP8Specification* hardwareContext = chip8QtWrapper->getChip8unwrapped()->getChip8HardwareContext();

    QStringList registerValues;
    int pc = hardwareContext->PC;
    int range = 40;

    for (int i = -(range>>2); i <= range>>2 ; ++i) {
        int memoryIndex = pc + i;

        // Ensure the memory index is within bounds
        if (memoryIndex >= 0 && memoryIndex < sizeof(hardwareContext->memory)){
            QString formattedString = QString("Memory[%1] = 0x%2")
                .arg(memoryIndex, 4, 16, QChar('0'))
                .arg(hardwareContext->memory[memoryIndex], 2, 16, QChar('0'));
            registerValues.append(formattedString);
        }
    }

    MemoryListViewModel->setStringList(registerValues);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    chip8QtWrapper->handleEvents(event);

    if (keyToButtonMap.contains(event->key())) {
        QPushButton* button = keyToButtonMap.value(event->key());
        if (button) {
            StyleYourButton(button, true);
        }
    }
}

void MainWindow::StyleYourButton(QPushButton* button, bool style) {
    style ? button->setStyleSheet("background-color: #FFCC00; border: 2px solid #FF9900;") :  button->setStyleSheet("");
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    chip8QtWrapper->handleEvents(event);

    if (keyToButtonMap.contains(event->key())) {
        QPushButton* button = keyToButtonMap.value(event->key());
        if (button) {
            StyleYourButton(button, false);
        }
    }
}

void MainWindow::onButtonPressed() {
    setButtonAction(PRESS);
}

void MainWindow::onButtonReleased() {
   setButtonAction(RELEASE);
}

void MainWindow::setButtonAction(int type) {
    QPushButton* keypadButtonClicked = qobject_cast<QPushButton*>(sender());

    if (keypadButtonClicked) {
        QMap<int, QPushButton*>::key_type key = keyToButtonMap.key(keypadButtonClicked);
        if (key != -1) {
            if (type == PRESS) {
                keypadButtonClicked->setStyleSheet("background-color: #FFCC00; border: 2px solid #FF9900;");
                QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::NoModifier);
                chip8QtWrapper->handleEvents(&keyEvent);
            } else {
                keypadButtonClicked->setStyleSheet("");
                QKeyEvent keyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
                chip8QtWrapper->handleEvents(&keyEvent);
            }
        }
    }
}



void MainWindow::createQtButtonMapping() {
    keyToButtonMap = {
        {Qt::Key_1, ui->H1}, {Qt::Key_2, ui->H2}, {Qt::Key_3, ui->H3}, {Qt::Key_4, ui->HC},
        {Qt::Key_Q, ui->H4}, {Qt::Key_W, ui->H5}, {Qt::Key_E, ui->H6}, {Qt::Key_R, ui->HD},
        {Qt::Key_A, ui->H7}, {Qt::Key_S, ui->H8}, {Qt::Key_D, ui->H9}, {Qt::Key_F, ui->HE},
        {Qt::Key_Z, ui->HA}, {Qt::Key_X, ui->H0}, {Qt::Key_C, ui->HB}, {Qt::Key_V, ui->HF}
    };
}

void MainWindow::connectButtonsToTheirOnCalls() {
    for (auto button : keyToButtonMap.values()) {
        connect(button, &QPushButton::pressed, this, &MainWindow::onButtonPressed);
        connect(button, &QPushButton::released, this, &MainWindow::onButtonReleased);
    }
}

