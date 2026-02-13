#include "mainwindow.h"
#include "lcuscene3d.h"
#include "animationcontroller3d.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFormLayout>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QCamera>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_is3DMode(false)
    , m_3dWindow(nullptr)
    , m_3dContainer(nullptr)
{
    setWindowTitle("Liquid Cooling Unit (LCU) - RSCU A C01");
    resize(1400, 900);
    
    // Create shared data model
    m_dataModel = new DataModel(this);
    
    // Create 2D scene and controller
    m_scene = new LCUScene(m_dataModel, this);
    m_animationController = new AnimationController(m_scene, m_dataModel, this);
    
    // Setup UI (will start in 2D mode)
    setupUI();
    
    // Setup 3D view (but don't show it yet)
    setup3DView();
    
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateDisplay);
    m_updateTimer->start(100); // Update display every 100ms
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Central widget with graphics view
    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setBackgroundBrush(QBrush(QColor(200, 220, 240)));
    setCentralWidget(m_view);
    
    createControlPanel();
    createStatusPanel();
    createSensorDisplay();
    
    // Menu bar
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("E&xit", this, &QMainWindow::close);
    
    QMenu *viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Zoom &In", [this]() { m_view->scale(1.2, 1.2); });
    viewMenu->addAction("Zoom &Out", [this]() { m_view->scale(0.8, 0.8); });
    viewMenu->addAction("&Reset Zoom", [this]() { m_view->resetTransform(); });
    
    statusBar()->showMessage("Ready");
}

void MainWindow::createControlPanel()
{
    QDockWidget *controlDock = new QDockWidget("Control Panel", this);
    controlDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    
    QWidget *controlWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(controlWidget);
    
    // System control buttons
    QGroupBox *controlGroup = new QGroupBox("System Control");
    QVBoxLayout *controlLayout = new QVBoxLayout();
    
    m_startButton = new QPushButton("Start System");
    m_stopButton = new QPushButton("Stop System");
    m_resetButton = new QPushButton("Reset All Trips");
    m_toggleViewButton = new QPushButton("Switch to 3D View");
    
    m_startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 10px; font-weight: bold; }");
    m_stopButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 10px; font-weight: bold; }");
    m_resetButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 10px; }");
    m_toggleViewButton->setStyleSheet("QPushButton { background-color: #FF9800; color: white; padding: 10px; font-weight: bold; }");
    
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(m_toggleViewButton, &QPushButton::clicked, this, &MainWindow::onToggleViewMode);
    
    controlLayout->addWidget(m_startButton);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addWidget(m_resetButton);
    controlLayout->addWidget(m_toggleViewButton);
    controlGroup->setLayout(controlLayout);
    layout->addWidget(controlGroup);
    
    // Cooling capacity control
    QGroupBox *capacityGroup = new QGroupBox("Cooling Capacity");
    QFormLayout *capacityLayout = new QFormLayout();
    
    QSpinBox *coolingCapacity = new QSpinBox();
    coolingCapacity->setRange(0, 100);
    coolingCapacity->setValue(30);
    coolingCapacity->setSuffix(" kW");
    connect(coolingCapacity, QOverload<int>::of(&QSpinBox::valueChanged), 
            [this](int value) { m_dataModel->setCoolingCapacity(value); });
    
    capacityLayout->addRow("Total Capacity:", coolingCapacity);
    capacityGroup->setLayout(capacityLayout);
    layout->addWidget(capacityGroup);
    
    layout->addStretch();
    controlWidget->setLayout(layout);
    controlDock->setWidget(controlWidget);
    addDockWidget(Qt::RightDockWidgetArea, controlDock);
}

void MainWindow::createStatusPanel()
{
    // This is created as part of sensor display
}

void MainWindow::createSensorDisplay()
{
    QDockWidget *sensorDock = new QDockWidget("Sensor Data", this);
    sensorDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    
    QWidget *sensorWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(sensorWidget);
    
    // Coolant system sensors
    QGroupBox *coolantGroup = new QGroupBox("Coolant System");
    QFormLayout *coolantLayout = new QFormLayout();
    
    m_supplyTempLabel = new QLabel("-- °C");
    m_systemPressureLabel = new QLabel("-- Bar");
    m_returnTempLabel = new QLabel("-- °C");
    m_returnPressureLabel = new QLabel("-- Bar");
    m_flowRateLabel = new QLabel("-- lpm");
    
    coolantLayout->addRow("Supply Temp:", m_supplyTempLabel);
    coolantLayout->addRow("System Pressure:", m_systemPressureLabel);
    coolantLayout->addRow("Return Temp:", m_returnTempLabel);
    coolantLayout->addRow("Return Pressure:", m_returnPressureLabel);
    coolantLayout->addRow("Flow Rate:", m_flowRateLabel);
    
    coolantGroup->setLayout(coolantLayout);
    layout->addWidget(coolantGroup);
    
    // Channel status
    QGroupBox *channelGroup = new QGroupBox("Channel Status");
    QFormLayout *channelLayout = new QFormLayout();
    
    m_ch1Label = new QLabel("CLOSED");
    m_ch2Label = new QLabel("CLOSED");
    m_ch3Label = new QLabel("CLOSED");
    m_ch4Label = new QLabel("CLOSED");
    
    channelLayout->addRow("CH 1:", m_ch1Label);
    channelLayout->addRow("CH 2:", m_ch2Label);
    channelLayout->addRow("CH 3:", m_ch3Label);
    channelLayout->addRow("CH 4:", m_ch4Label);
    
    channelGroup->setLayout(channelLayout);
    layout->addWidget(channelGroup);
    
    // Refrigerant system
    QGroupBox *refrigerantGroup = new QGroupBox("Refrigerant System");
    QFormLayout *refrigerantLayout = new QFormLayout();
    
    m_pump1Label = new QLabel("OFF");
    m_pump2Label = new QLabel("OFF");
    m_wd1Label = new QLabel("Idle");
    m_wd2Label = new QLabel("Idle");
    m_wd3Label = new QLabel("Idle");
    
    refrigerantLayout->addRow("Pump 1:", m_pump1Label);
    refrigerantLayout->addRow("Pump 2:", m_pump2Label);
    refrigerantLayout->addRow("WD 1:", m_wd1Label);
    refrigerantLayout->addRow("WD 2:", m_wd2Label);
    refrigerantLayout->addRow("WD 3:", m_wd3Label);
    
    refrigerantGroup->setLayout(refrigerantLayout);
    layout->addWidget(refrigerantGroup);
    
    layout->addStretch();
    sensorWidget->setLayout(layout);
    sensorDock->setWidget(sensorWidget);
    addDockWidget(Qt::RightDockWidgetArea, sensorDock);
}

void MainWindow::onStartClicked()
{
    m_dataModel->setSystemRunning(true);
    
    // Start the appropriate animation controller
    if (m_is3DMode) {
        m_animationController3d->start();
    } else {
        m_animationController->start();
    }
    
    statusBar()->showMessage("System Running");
}

void MainWindow::onStopClicked()
{
    m_dataModel->setSystemRunning(false);
    
    // Stop both animation controllers
    m_animationController->stop();
    m_animationController3d->stop();
    
    statusBar()->showMessage("System Stopped");
}

void MainWindow::onResetClicked()
{
    m_dataModel->resetAllTrips();
    statusBar()->showMessage("All trips reset");
}

void MainWindow::onDataChanged()
{
    updateDisplay();
}

void MainWindow::updateDisplay()
{
    // Update coolant system displays
    m_supplyTempLabel->setText(QString::number(m_dataModel->getSupplyTemp(), 'f', 1) + " °C");
    m_systemPressureLabel->setText(QString::number(m_dataModel->getSystemPressure(), 'f', 2) + " Bar");
    m_returnTempLabel->setText(QString::number(m_dataModel->getReturnTemp(), 'f', 1) + " °C");
    m_returnPressureLabel->setText(QString::number(m_dataModel->getReturnPressure(), 'f', 2) + " Bar");
    m_flowRateLabel->setText(QString::number(m_dataModel->getFlowRate(), 'f', 1) + " lpm");
    
    // Update channel status
    m_ch1Label->setText(m_dataModel->getChannelState(0) ? "OPEN" : "CLOSED");
    m_ch2Label->setText(m_dataModel->getChannelState(1) ? "OPEN" : "CLOSED");
    m_ch3Label->setText(m_dataModel->getChannelState(2) ? "OPEN" : "CLOSED");
    m_ch4Label->setText(m_dataModel->getChannelState(3) ? "OPEN" : "CLOSED");
    
    // Update refrigerant system
    m_pump1Label->setText(m_dataModel->getPumpState(0) ? "ON" : "OFF");
    m_pump2Label->setText(m_dataModel->getPumpState(1) ? "ON" : "OFF");
    
    m_wd1Label->setText(m_dataModel->getCompressorState(0) ? "Running" : "Idle");
    m_wd2Label->setText(m_dataModel->getCompressorState(1) ? "Running" : "Idle");
    m_wd3Label->setText(m_dataModel->getCompressorState(2) ? "Running" : "Idle");
}

void MainWindow::setup3DView()
{
    // Create 3D window
    m_3dWindow = new Qt3DExtras::Qt3DWindow();
    m_3dWindow->defaultFrameGraph()->setClearColor(QColor(200, 220, 240));
    
    // Create 3D scene
    m_scene3d = new LCUScene3D(m_dataModel);
    
    // Setup camera
    Qt3DRender::QCamera *camera = m_3dWindow->camera();
    m_scene3d->setupCamera(camera);
    
    // Setup camera controller for user interaction
    m_cameraController = new Qt3DExtras::QOrbitCameraController(m_scene3d);
    m_cameraController->setLinearSpeed(50.0f);
    m_cameraController->setLookSpeed(180.0f);
    m_cameraController->setCamera(camera);
    
    // Set the root entity
    m_3dWindow->setRootEntity(m_scene3d);
    
    // Create animation controller for 3D
    m_animationController3d = new AnimationController3D(m_scene3d, m_dataModel, this);
    
    // Create container widget (not shown initially)
    m_3dContainer = QWidget::createWindowContainer(m_3dWindow, this);
    m_3dContainer->setMinimumSize(QSize(800, 600));
    m_3dContainer->hide();
}

void MainWindow::onToggleViewMode()
{
    if (m_is3DMode) {
        switchTo2D();
    } else {
        switchTo3D();
    }
}

void MainWindow::switchTo2D()
{
    m_is3DMode = false;
    
    // Stop 3D animations
    m_animationController3d->stop();
    
    // Hide 3D view
    if (m_3dContainer) {
        m_3dContainer->hide();
    }
    
    // Show 2D view
    setCentralWidget(m_view);
    m_view->show();
    
    // Restart 2D animations if system is running
    if (m_dataModel->isSystemRunning()) {
        m_animationController->start();
    }
    
    // Update button text
    m_toggleViewButton->setText("Switch to 3D View");
    
    statusBar()->showMessage("Switched to 2D View");
}

void MainWindow::switchTo3D()
{
    m_is3DMode = true;
    
    // Stop 2D animations
    m_animationController->stop();
    
    // Hide 2D view
    m_view->hide();
    
    // Show 3D view
    setCentralWidget(m_3dContainer);
    m_3dContainer->show();
    
    // Start 3D animations if system is running
    if (m_dataModel->isSystemRunning()) {
        m_animationController3d->start();
    }
    
    // Update button text
    m_toggleViewButton->setText("Switch to 2D View");
    
    statusBar()->showMessage("Switched to 3D View - Use mouse to rotate/zoom");
}
