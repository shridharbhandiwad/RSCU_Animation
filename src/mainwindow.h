#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QWidget>
#include "lcuscene.h"
#include "lcuscene3d.h"
#include "datamodel.h"
#include "animationcontroller.h"
#include "animationcontroller3d.h"

namespace Qt3DExtras {
    class Qt3DWindow;
    class QOrbitCameraController;
}

namespace Qt3DRender {
    class QCamera;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onStopClicked();
    void onResetClicked();
    void onDataChanged();
    void updateDisplay();
    void onToggleViewMode();

private:
    void setupUI();
    void createControlPanel();
    void createStatusPanel();
    void createSensorDisplay();
    void setup3DView();
    void switchTo2D();
    void switchTo3D();
    
    // 2D view components
    QGraphicsView *m_view;
    LCUScene *m_scene;
    AnimationController *m_animationController;
    
    // 3D view components
    Qt3DExtras::Qt3DWindow *m_3dWindow;
    QWidget *m_3dContainer;
    LCUScene3D *m_scene3d;
    AnimationController3D *m_animationController3d;
    Qt3DExtras::QOrbitCameraController *m_cameraController;
    
    // Shared data model
    DataModel *m_dataModel;
    
    // View state
    bool m_is3DMode;
    
    // Control widgets
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_resetButton;
    QPushButton *m_toggleViewButton;
    
    // Sensor displays
    QLabel *m_supplyTempLabel;
    QLabel *m_systemPressureLabel;
    QLabel *m_returnTempLabel;
    QLabel *m_returnPressureLabel;
    QLabel *m_flowRateLabel;
    
    // Channel displays
    QLabel *m_ch1Label;
    QLabel *m_ch2Label;
    QLabel *m_ch3Label;
    QLabel *m_ch4Label;
    
    // Refrigerant displays
    QLabel *m_pump1Label;
    QLabel *m_pump2Label;
    QLabel *m_wd1Label;
    QLabel *m_wd2Label;
    QLabel *m_wd3Label;
    
    QTimer *m_updateTimer;
};

#endif // MAINWINDOW_H
