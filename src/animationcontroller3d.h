#ifndef ANIMATIONCONTROLLER3D_H
#define ANIMATIONCONTROLLER3D_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class LCUScene3D;
class DataModel;

class AnimationController3D : public QObject
{
    Q_OBJECT

public:
    explicit AnimationController3D(LCUScene3D *scene, DataModel *dataModel, QObject *parent = nullptr);
    
    void start();
    void stop();
    void setFrameRate(int fps);

private slots:
    void updateFrame();

private:
    LCUScene3D *m_scene;
    DataModel *m_dataModel;
    QTimer *m_timer;
    QElapsedTimer m_elapsedTimer;
    qint64 m_lastFrameTime;
    int m_frameRate;
};

#endif // ANIMATIONCONTROLLER3D_H
