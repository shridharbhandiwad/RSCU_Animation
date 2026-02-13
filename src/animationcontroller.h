#ifndef ANIMATIONCONTROLLER_H
#define ANIMATIONCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class LCUScene;
class DataModel;

class AnimationController : public QObject
{
    Q_OBJECT

public:
    explicit AnimationController(LCUScene *scene, DataModel *dataModel, QObject *parent = nullptr);
    
    void start();
    void stop();
    void pause();
    void resume();
    
    bool isRunning() const { return m_running; }
    bool isPaused() const { return m_paused; }

private slots:
    void update();

private:
    LCUScene *m_scene;
    DataModel *m_dataModel;
    QTimer *m_timer;
    QElapsedTimer m_elapsedTimer;
    
    bool m_running;
    bool m_paused;
    qint64 m_lastUpdateTime;
};

#endif // ANIMATIONCONTROLLER_H
