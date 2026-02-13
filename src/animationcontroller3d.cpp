#include "animationcontroller3d.h"
#include "lcuscene3d.h"
#include "datamodel.h"

AnimationController3D::AnimationController3D(LCUScene3D *scene, DataModel *dataModel, QObject *parent)
    : QObject(parent)
    , m_scene(scene)
    , m_dataModel(dataModel)
    , m_lastFrameTime(0)
    , m_frameRate(30)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AnimationController3D::updateFrame);
    
    // Set initial frame rate
    setFrameRate(m_frameRate);
}

void AnimationController3D::start()
{
    m_elapsedTimer.start();
    m_lastFrameTime = 0;
    m_timer->start();
}

void AnimationController3D::stop()
{
    m_timer->stop();
}

void AnimationController3D::setFrameRate(int fps)
{
    m_frameRate = fps;
    int interval = 1000 / fps; // milliseconds per frame
    m_timer->setInterval(interval);
}

void AnimationController3D::updateFrame()
{
    qint64 currentTime = m_elapsedTimer.elapsed();
    double deltaTime = (currentTime - m_lastFrameTime) / 1000.0; // Convert to seconds
    m_lastFrameTime = currentTime;
    
    // Update scene animations
    if (m_scene) {
        m_scene->updateAnimations(deltaTime);
    }
}
