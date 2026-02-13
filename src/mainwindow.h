#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "lcuscene.h"
#include "datamodel.h"
#include "animationcontroller.h"

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

private:
    void setupUI();
    void createControlPanel();
    void createStatusPanel();
    void createSensorDisplay();
    
    QGraphicsView *m_view;
    LCUScene *m_scene;
    DataModel *m_dataModel;
    AnimationController *m_animationController;
    
    // Control widgets
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_resetButton;
    
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
