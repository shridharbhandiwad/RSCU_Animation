#include "animationcontroller.h"
#include "lcuscene.h"
#include "datamodel.h"

AnimationController::AnimationController(LCUScene *scene, DataModel *dataModel, QObject *parent)
    : QObject(parent)
    , m_scene(scene)
    , m_dataModel(dataModel)
    , m_running(false)
    , m_paused(false)
    , m_lastUpdateTime(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AnimationController::update);
}

void AnimationController::start()
{
    if (!m_running) {
        m_running = true;
        m_paused = false;
        m_elapsedTimer.start();
        m_lastUpdateTime = 0;
        m_timer->start(33); // ~30 FPS
    }
}

void AnimationController::stop()
{
    if (m_running) {
        m_running = false;
        m_paused = false;
        m_timer->stop();
    }
}

void AnimationController::pause()
{
    if (m_running && !m_paused) {
        m_paused = true;
    }
}

void AnimationController::resume()
{
    if (m_running && m_paused) {
        m_paused = false;
        m_lastUpdateTime = m_elapsedTimer.elapsed();
    }
}

void AnimationController::update()
{
    if (!m_running || m_paused) {
        return;
    }
    
    qint64 currentTime = m_elapsedTimer.elapsed();
    double deltaTime = (currentTime - m_lastUpdateTime) / 1000.0; // Convert to seconds
    
    if (m_lastUpdateTime == 0) {
        deltaTime = 0.033; // First frame
    }
    
    m_lastUpdateTime = currentTime;
    
    // Update data model simulation
    m_dataModel->updateSimulation(deltaTime);
    
    // Update scene animations
    m_scene->updateAnimations(deltaTime);
}
