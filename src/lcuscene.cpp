#include "lcuscene.h"
#include "datamodel.h"
#include "components/pump.h"
#include "components/valve.h"
#include "components/tank.h"
#include "components/heater.h"
#include "components/heatexchanger.h"
#include "components/condenser.h"
#include "components/blower.h"
#include "components/pipe.h"
#include "components/solenoidvalve.h"
#include <QGraphicsTextItem>
#include <QFont>

LCUScene::LCUScene(DataModel *dataModel, QObject *parent)
    : QGraphicsScene(parent)
    , m_dataModel(dataModel)
{
    setSceneRect(0, 0, 1200, 700);
    setupScene();
}

void LCUScene::setupScene()
{
    // Create all components according to the LCU diagram
    createCoolantSystem();
    createChannelSystem();
    createRefrigerantSystem();
    createPipingConnections();
    createLabels();
}

void LCUScene::createCoolantSystem()
{
    // Tank (left side, lower area)
    m_tank = new Tank();
    m_tank->setPos(100, 500);
    m_tank->setLevel(75.0);
    addItem(m_tank);
    m_allComponents.append(m_tank);
    
    // Heater (above tank)
    m_heater = new Heater();
    m_heater->setPos(100, 380);
    addItem(m_heater);
    m_allComponents.append(m_heater);
    
    // Coolant pumps (2 pumps)
    for (int i = 0; i < 2; ++i) {
        Pump *pump = new Pump(i);
        pump->setPos(180 + i * 60, 500);
        m_coolantPumps.append(pump);
        addItem(pump);
        m_allComponents.append(pump);
    }
    
    // Create coolant system label
    QGraphicsTextItem *coolantLabel = addText("Coolant System", QFont("Arial", 10, QFont::Bold));
    coolantLabel->setPos(50, 300);
    coolantLabel->setDefaultTextColor(Qt::darkBlue);
}

void LCUScene::createChannelSystem()
{
    // Create 4 channels with valves (top section)
    double startX = 350;
    double startY = 100;
    double spacing = 50;
    
    for (int i = 0; i < 4; ++i) {
        // Main flow valves
        Valve *inflowValve = new Valve(i * 3, Valve::BallValve);
        inflowValve->setPos(startX, startY + i * spacing);
        m_channelValves.append(inflowValve);
        addItem(inflowValve);
        m_allComponents.append(inflowValve);
        
        // Mid channel valves
        Valve *midValve1 = new Valve(i * 3 + 1, Valve::BallValve);
        midValve1->setPos(startX + 150, startY + i * spacing);
        m_channelValves.append(midValve1);
        addItem(midValve1);
        m_allComponents.append(midValve1);
        
        Valve *midValve2 = new Valve(i * 3 + 2, Valve::BallValve);
        midValve2->setPos(startX + 250, startY + i * spacing);
        m_channelValves.append(midValve2);
        addItem(midValve2);
        m_allComponents.append(midValve2);
        
        // Channel label
        QGraphicsTextItem *chLabel = addText(QString("CH %1").arg(i + 1), QFont("Arial", 8, QFont::Bold));
        chLabel->setPos(startX - 50, startY + i * spacing - 10);
    }
    
    // Add sensor display area at top
    QGraphicsTextItem *sensorLabel = addText("Sensor Data", QFont("Arial", 10, QFont::Bold));
    sensorLabel->setPos(350, 20);
    sensorLabel->setDefaultTextColor(Qt::darkBlue);
}

void LCUScene::createRefrigerantSystem()
{
    // Create 3 refrigerant loops (bottom right section)
    double startX = 650;
    double startY = 400;
    double spacing = 100;
    
    for (int i = 0; i < 3; ++i) {
        double yPos = startY + i * spacing;
        
        // PHE (Plate Heat Exchanger)
        HeatExchanger *phe = new HeatExchanger(i + 1);
        phe->setPos(startX - 80, yPos);
        m_heatExchangers.append(phe);
        addItem(phe);
        m_allComponents.append(phe);
        
        // Solenoid Valve
        SolenoidValve *sv = new SolenoidValve(i + 1);
        sv->setPos(startX, yPos);
        m_solenoidValves.append(sv);
        addItem(sv);
        m_allComponents.append(sv);
        
        // Condenser
        Condenser *condenser = new Condenser(i + 1);
        condenser->setPos(startX + 80, yPos);
        m_condensers.append(condenser);
        addItem(condenser);
        m_allComponents.append(condenser);
        
        // Blower
        Blower *blower = new Blower(i + 1);
        blower->setPos(startX + 150, yPos);
        m_blowers.append(blower);
        addItem(blower);
        m_allComponents.append(blower);
        
        // Loop label
        QGraphicsTextItem *loopLabel = addText(QString("Loop %1").arg(i + 1), QFont("Arial", 7));
        loopLabel->setPos(startX - 120, yPos - 15);
    }
    
    // Create refrigerant system label
    QGraphicsTextItem *refLabel = addText("Refrigerant System", QFont("Arial", 10, QFont::Bold));
    refLabel->setPos(650, 300);
    refLabel->setDefaultTextColor(Qt::darkBlue);
}

void LCUScene::createPipingConnections()
{
    // Connect coolant system components
    
    // Tank to heater
    Pipe *tankToHeater = new Pipe();
    tankToHeater->setPath({{100, 450}, {100, 410}});
    tankToHeater->setFluidColor(QColor(100, 150, 200));
    m_coolantPipes.append(tankToHeater);
    addItem(tankToHeater);
    m_allComponents.append(tankToHeater);
    
    // Heater to pumps
    for (int i = 0; i < 2; ++i) {
        Pipe *heaterToPump = new Pipe();
        heaterToPump->setPath({{120, 380}, {180 + i * 60, 380}, {180 + i * 60, 475}});
        heaterToPump->setFluidColor(QColor(100, 150, 200));
        m_coolantPipes.append(heaterToPump);
        addItem(heaterToPump);
        m_allComponents.append(heaterToPump);
    }
    
    // Pumps to channel distribution
    Pipe *pumpToChannels = new Pipe();
    pumpToChannels->setPath({{220, 500}, {300, 500}, {300, 150}});
    pumpToChannels->setFluidColor(QColor(100, 150, 200));
    m_coolantPipes.append(pumpToChannels);
    addItem(pumpToChannels);
    m_allComponents.append(pumpToChannels);
    
    // Channel connections
    for (int i = 0; i < 4; ++i) {
        double y = 100 + i * 50;
        
        // Supply to channel
        Pipe *supplyPipe = new Pipe();
        supplyPipe->setPath({{300, y}, {330, y}});
        supplyPipe->setFluidColor(QColor(100, 150, 200));
        m_channelPipes.append(supplyPipe);
        addItem(supplyPipe);
        m_allComponents.append(supplyPipe);
        
        // Channel flow
        Pipe *channelPipe = new Pipe();
        channelPipe->setPath({{370, y}, {480, y}});
        channelPipe->setFluidColor(QColor(120, 170, 220));
        m_channelPipes.append(channelPipe);
        addItem(channelPipe);
        m_allComponents.append(channelPipe);
        
        // Return from channel
        Pipe *returnPipe = new Pipe();
        returnPipe->setPath({{520, y}, {600, y}});
        returnPipe->setFluidColor(QColor(200, 150, 100));
        m_channelPipes.append(returnPipe);
        addItem(returnPipe);
        m_allComponents.append(returnPipe);
    }
    
    // Return manifold
    Pipe *returnManifold = new Pipe();
    returnManifold->setPath({{600, 100}, {600, 250}, {100, 250}, {100, 450}});
    returnManifold->setFluidColor(QColor(200, 150, 100));
    m_coolantPipes.append(returnManifold);
    addItem(returnManifold);
    m_allComponents.append(returnManifold);
    
    // Refrigerant connections
    for (int i = 0; i < 3; ++i) {
        double y = 400 + i * 100;
        
        // Connect PHE to Solenoid Valve
        Pipe *pheToSV = new Pipe();
        pheToSV->setPath({{595, y}, {635, y}});
        pheToSV->setFluidColor(QColor(150, 200, 255));
        m_refrigerantPipes.append(pheToSV);
        addItem(pheToSV);
        m_allComponents.append(pheToSV);
        
        // Connect Solenoid Valve to Condenser
        Pipe *svToCondenser = new Pipe();
        svToCondenser->setPath({{665, y}, {650, y}});
        svToCondenser->setFluidColor(QColor(150, 200, 255));
        m_refrigerantPipes.append(svToCondenser);
        addItem(svToCondenser);
        m_allComponents.append(svToCondenser);
        
        // Connect Condenser to return
        Pipe *condenserReturn = new Pipe();
        condenserReturn->setPath({{710, y}, {540, y}});
        condenserReturn->setFluidColor(QColor(255, 150, 150));
        m_refrigerantPipes.append(condenserReturn);
        addItem(condenserReturn);
        m_allComponents.append(condenserReturn);
    }
}

void LCUScene::createLabels()
{
    // Main title
    QGraphicsTextItem *title = addText("Liquid Cooling Unit (LCU) - RSCU A C01", 
                                       QFont("Arial", 14, QFont::Bold));
    title->setPos(300, -20);
    title->setDefaultTextColor(Qt::darkBlue);
    
    // System status indicator
    QGraphicsRectItem *statusBox = addRect(1050, 30, 120, 60, 
                                          QPen(Qt::black, 2), 
                                          QBrush(QColor(240, 240, 240)));
    
    QGraphicsTextItem *statusText = addText("System Status", QFont("Arial", 9, QFont::Bold));
    statusText->setPos(1060, 35);
    
    QGraphicsTextItem *runningText = addText("READY", QFont("Arial", 8));
    runningText->setPos(1070, 55);
    runningText->setDefaultTextColor(Qt::darkGreen);
}

void LCUScene::updateAnimations(double deltaTime)
{
    // Update all components
    for (BaseComponent *component : m_allComponents) {
        component->updateAnimation(deltaTime);
    }
    
    // Update component states based on data model
    
    // Update coolant pumps
    for (int i = 0; i < m_coolantPumps.size(); ++i) {
        bool running = m_dataModel->getPumpState(i);
        m_coolantPumps[i]->setRunning(running);
        m_coolantPumps[i]->setFlowRate(running ? m_dataModel->getFlowRate() / 2.0 : 0.0);
    }
    
    // Update heater
    if (m_dataModel->isSystemRunning()) {
        m_heater->setActive(true);
        m_heater->setPower(m_dataModel->getHeaterPower());
    } else {
        m_heater->setActive(false);
        m_heater->setPower(0.0);
    }
    
    // Update tank
    m_tank->setLevel(m_dataModel->getTankLevel());
    m_tank->setTemperature(m_dataModel->getSupplyTemp());
    
    // Update channel valves (simplified - every 3 valves per channel)
    for (int ch = 0; ch < 4; ++ch) {
        bool channelOpen = m_dataModel->getChannelState(ch);
        for (int v = 0; v < 3 && (ch * 3 + v) < m_channelValves.size(); ++v) {
            m_channelValves[ch * 3 + v]->setOpen(channelOpen);
        }
    }
    
    // Update refrigerant system
    for (int i = 0; i < 3; ++i) {
        // Update heat exchangers
        if (i < m_heatExchangers.size()) {
            bool active = m_dataModel->getCompressorState(i);
            m_heatExchangers[i]->setActive(active);
            m_heatExchangers[i]->setHotSideTemp(m_dataModel->getReturnTemp());
            m_heatExchangers[i]->setColdSideTemp(m_dataModel->getPHETemp(i));
        }
        
        // Update solenoid valves
        if (i < m_solenoidValves.size()) {
            bool open = m_dataModel->getSolenoidValveState(i);
            m_solenoidValves[i]->setOpen(open);
            m_solenoidValves[i]->setEnergized(open);
        }
        
        // Update condensers
        if (i < m_condensers.size()) {
            bool active = m_dataModel->getCompressorState(i);
            m_condensers[i]->setActive(active);
            m_condensers[i]->setTemperature(m_dataModel->getCondenserTemp(i));
        }
        
        // Update blowers
        if (i < m_blowers.size()) {
            bool running = m_dataModel->getBlowerState(i);
            m_blowers[i]->setRunning(running);
            m_blowers[i]->setSpeed(running ? 75.0 : 0.0);
        }
    }
    
    // Update pipe flows
    bool systemRunning = m_dataModel->isSystemRunning();
    
    // Coolant pipes
    for (Pipe *pipe : m_coolantPipes) {
        pipe->setFlowing(systemRunning);
    }
    
    // Channel pipes - only if channel is open
    for (int ch = 0; ch < 4; ++ch) {
        bool channelOpen = m_dataModel->getChannelState(ch);
        for (int p = 0; p < 3 && (ch * 3 + p) < m_channelPipes.size(); ++p) {
            m_channelPipes[ch * 3 + p]->setFlowing(channelOpen && systemRunning);
        }
    }
    
    // Refrigerant pipes
    for (int i = 0; i < m_refrigerantPipes.size(); ++i) {
        int loopIndex = i / 3;
        if (loopIndex < 3) {
            bool loopActive = m_dataModel->getCompressorState(loopIndex);
            m_refrigerantPipes[i]->setFlowing(loopActive);
        }
    }
}
