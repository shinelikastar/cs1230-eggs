#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate

class CamtransCamera;
class SceneviewScene;

namespace CS123 { namespace GL {
class CS123Shader;
class FBO;
class FullScreenQuad;
}}

class View : public QGLWidget {
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

    CamtransCamera* getCamera();
    void settingsChanged();

private:
    QTime m_time;
    QTimer m_timer;
    bool m_captureMouse;
    bool m_isDragging;
    std::unique_ptr<CamtransCamera> m_camera;
    std::unique_ptr<SceneviewScene> m_scene;
    std::unique_ptr<CS123::GL::FBO> m_blurFBO1;
    std::unique_ptr<CS123::GL::FBO> m_blurFBO2;
    std::unique_ptr<CS123::GL::FullScreenQuad> m_quad;
    std::unique_ptr<CS123::GL::CS123Shader> m_dofShaderH;
    std::unique_ptr<CS123::GL::CS123Shader> m_dofShaderV;
    float m_runningTime;
    glm::vec4 m_prev_eye;
    float m_prev_time;
    float m_scale;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void loadDofShaders();
    void performDoF();
    void updateCameraPosition();
    glm::vec2 bezierPathOne();
    glm::vec2 bezierPathTwo();
    glm::vec2 bezierPathThree();
    glm::vec2 bezierPoint(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

private slots:
    void tick();
};

#endif // VIEW_H
