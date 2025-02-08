//
// Created by Xandr on 2/7/2025.
//
#include "MainWindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus); // Keypad may not be working without this focus policy possibly due to SDL still included (check once everything is migrated)

    // Set up the title for the window
    setWindowTitle("Qt Chip8 Emulator");
    emulatorWidget = new QtRenderer(this);
    emulatorWidget->setFixedSize(SUPER_CHIP_WIDTH, SUPER_CHIP_HEIGHT);

    // I need to figure out why I can't explicitly set a layout in QT Designer :/
    if (!ui->Emulator->layout()) {
        ui->Emulator->setLayout(new QVBoxLayout);  // Sets the layout for the Emulator widget
    }
    ui->Emulator->layout()->addWidget(emulatorWidget);


    chip8QtWrapper = new Chip8EmulatorQtWrapper(this, emulatorWidget);
    chip8QtWrapper->loadROM("../games/UFO.ch8");

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


    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::setRegistersValues);
    connect(timer, &QTimer::timeout, this, &MainWindow::setProgramCounter);
    connect(timer, &QTimer::timeout, this, &MainWindow::viewMemoryValues);
    connect(timer, &QTimer::timeout, this, &MainWindow::setRegisterValuesInTableView);
    timer->start(100);

}

MainWindow::~MainWindow() {
    delete ui;
    delete registerModel;
    delete chip8QtWrapper;
}

void MainWindow::setRegistersValues() {
    QStringList registerValues;

    CHIP8Specification* hardwareContext = chip8QtWrapper->getChip8unwrapped()->getChip8HardwareContext();
    // Loop through the registers and get their values
    for (int i = 0; i < NUM_OF_REGISTER; ++i) {
        QString part1 = QString("V[%1]").arg(i, 2, 16, QChar(' ')).toUpper();
        QString part2 = QString(" = 0x%1").arg(hardwareContext->V[i], 2, 16, QChar('0'));
        registerValues.append(part1 + part2);
    }

    // Update the model with the new register values
    registerModel->setStringList(registerValues);
}

void MainWindow::setRegisterValuesInTableView() {
    QStringList registerValues;

    RegisterTableModel->clear();  // Clear existing data
    RegisterTableModel->setColumnCount(2);
    RegisterTableModel->setHeaderData(0, Qt::Horizontal, "Register");
    RegisterTableModel->setHeaderData(1, Qt::Horizontal, "Value");

    CHIP8Specification* hardwareContext = chip8QtWrapper->getChip8unwrapped()->getChip8HardwareContext();
    // Loop through the registers and get their values
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

    // Update the model with the new register values
    programCounterModel->setStringList(registerValues);
}

void MainWindow::viewMemoryValues() {
    CHIP8Specification* hardwareContext = chip8QtWrapper->getChip8unwrapped()->getChip8HardwareContext();

    QStringList registerValues;
    int pc = hardwareContext->PC;
    int romSize = sizeof(hardwareContext->memory);
    int range = 40;

    for (int i = -(range>>2); i <= range>>2 ; ++i) {
        int memoryIndex = pc + i;

        // Ensure the memory index is within bounds
        if (memoryIndex >= 0 && memoryIndex < romSize) {
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
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    chip8QtWrapper->handleEvents(event);
}


