#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>
#include "camera/CamtransCamera.h"
#include "scenegraph/SceneviewScene.h"
#include "lib/CS123XmlSceneParser.h"
#include "Settings.h"
#include "lib/ResourceLoader.h"
#include "gl/datatype/FBO.h"
#include "gl/shaders/CS123Shader.h"
#include "gl/textures/Texture2D.h"
#include "gl/textures/RenderBuffer.h"
#include "gl/util/FullScreenQuad.h"
#include "gl/datatype/VAO.h"

using namespace CS123::GL;

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), m_captureMouse(true),
    m_isDragging(false), m_camera(new CamtransCamera()),
    m_blurFBO1(nullptr), m_blurFBO2(nullptr), m_quad(nullptr),
    m_runningTime(0)
{
    settings.loadSettingsOrDefaults();
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor
//    if (m_captureMouse) {
//        QApplication::setOverrideCursor(Qt::BlankCursor);
//    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

View::~View()
{
}

void View::initializeGL() {
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }
    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    m_scene = std::make_unique<SceneviewScene>();
    CS123XmlSceneParser parser = CS123XmlSceneParser(
                "/Users/ericelliott/cs/cs1230/final/cs1230-final/scenes/museumscene/eggmusfried.xml");
//    CS123XmlSceneParser parser = CS123XmlSceneParser(
//                "/Users/ericelliott/cs/cs1230/finalthree/cs1230-final/scenes/eggfried.xml");
    parser.parse();
    Scene::parse(m_scene.get(), &parser);
    m_scene->loadMeshes();

    m_quad = std::make_unique<FullScreenQuad>();

    loadDofShaders();
}

void View::paintGL() {

    if (settings.enableDof) {
        m_blurFBO1->bind();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: Implement the demo rendering here
    float ratio = static_cast<QGuiApplication *>(
                QCoreApplication::instance())->devicePixelRatio();
    getCamera()->setAspectRatio(
                static_cast<float>(width()) / static_cast<float>(height()));

    if (settings.enableDof) {
        glViewport(0, 0, width(), height());
    } else {
        glViewport(0, 0, width() * ratio, height() * ratio);
    }

    m_scene->render(this);

    if (settings.enableDof) {
        m_blurFBO1->unbind();
        performDoF();
    }
}

void View::resizeGL(int w, int h) {
    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
    w = static_cast<int>(w / ratio);
    h = static_cast<int>(h / ratio);

    // TODO: [Task 5] Initialize FBOs here, with dimensions m_width and m_height.
    m_blurFBO1 = std::make_unique<FBO>(2, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, w, h,
                                       TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);
    //       [Task 12] Pass in TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE as the last parameter
    m_blurFBO2 = std::make_unique<FBO>(2, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, w, h,
                                       TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);

    m_blurFBO1->bind();
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);
    m_blurFBO1->unbind();

    m_blurFBO2->bind();
    glDrawBuffers(2, drawBuffers);
    m_blurFBO2->unbind();

    glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event) {
//    if (event->button() == Qt::RightButton) {
//        m_camera->mouseDown(event->x(), event->y());
//        m_isDragging = true;
//        update();
//    }
}

void View::mouseMoveEvent(QMouseEvent *event) {
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
//    if(m_captureMouse) {
////        int deltaX = event->x() - width() / 2;
////        int deltaY = event->y() - height() / 2;
////        if (!deltaX && !deltaY) return;
////        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

//        // TODO: Handle mouse movements here
//        if (m_isDragging) {
//            m_camera->mouseDragged(event->x(), event->y());
//            update();
//        }
//    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {
//    if (m_isDragging && event->button() == Qt::RightButton) {
//        m_camera->mouseUp(event->x(), event->y());
//        m_isDragging = false;
//        update();
//    }
}

void View::wheelEvent(QWheelEvent *event) {
//    m_camera->mouseScrolled(event->delta());
//    update();
}

void View::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
}

void View::keyReleaseEvent(QKeyEvent *event) {

}

CamtransCamera* View::getCamera() {
    return m_camera.get();
}

void View::loadDofShaders() {
    std::string fragmentSourceH = ResourceLoader::loadResourceFileToString(":/shaders/dof_h.frag");
    std::string vertexSourceH = ResourceLoader::loadResourceFileToString(":/shaders/quad.vert");
    std::string fragmentSourceV = ResourceLoader::loadResourceFileToString(":/shaders/dof_v.frag");
    std::string vertexSourceV = ResourceLoader::loadResourceFileToString(":/shaders/quad.vert");
    m_dofShaderH = std::make_unique<CS123Shader>(vertexSourceH, fragmentSourceH);
    m_dofShaderV = std::make_unique<CS123Shader>(vertexSourceV, fragmentSourceV);
}

void View::performDoF() {
    float ratio = static_cast<QGuiApplication *>(
                QCoreApplication::instance())->devicePixelRatio();
    m_blurFBO2->bind();
    glViewport(0, 0, width(), height());
    m_dofShaderH->bind();
    m_dofShaderH->setTexture("tex", m_blurFBO1->getColorAttachment(0));
    m_dofShaderH->setTexture("depthVals", m_blurFBO1->getColorAttachment(1));
    m_dofShaderH->setUniform("fStop", settings.fStop);
    m_dofShaderH->setUniform("planeInFocus", settings.planeInFocus);

    glActiveTexture(GL_TEXTURE0);
    m_blurFBO1->getColorAttachment(0).bind();
    glActiveTexture(GL_TEXTURE1);
    m_blurFBO1->getColorAttachment(1).bind();

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    m_quad->draw();

    m_dofShaderH->unbind();
    m_blurFBO2->unbind();

    m_dofShaderV->bind();
    m_dofShaderV->setTexture("tex", m_blurFBO2->getColorAttachment(0));
    m_dofShaderV->setTexture("depthVals", m_blurFBO2->getColorAttachment(1));
    m_dofShaderV->setUniform("fStop", settings.fStop);
    m_dofShaderV->setUniform("planeInFocus", settings.planeInFocus);

    glActiveTexture(GL_TEXTURE0);
    m_blurFBO2->getColorAttachment(0).bind();
    glActiveTexture(GL_TEXTURE1);
    m_blurFBO2->getColorAttachment(1).bind();
    glViewport(0, 0, width() * ratio, height() * ratio);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_quad->draw();
    m_dofShaderV->unbind();
}

void View::settingsChanged() {
}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = m_time.restart() * 0.001f;

    m_runningTime += seconds;
    m_scale = settings.scale;
    // clamping scale to a valid range
    if(m_scale < 1.f) {
        m_scale = 1.f;
    }
    if(m_scale > 4.f) {
        m_scale = 4.f;
    }
    updateCameraPosition();

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

void View::updateCameraPosition() {
//    glm::vec4 curEye = m_camera->getPosition();
    glm::vec4 curEye;
    glm::vec2 bez;
    // resetting eye if scene is paused
    if(settings.paused) {
        curEye = m_prev_eye;
        m_runningTime = m_prev_time;
    }
    else {
        // checking path selected in settings
        if(settings.pathNum == 0) {
            bez = bezierPathOne();
        }
        else if(settings.pathNum == 1) {
            bez = bezierPathTwo();
        }
        else {
            bez = bezierPathThree();
        }
        // clamping height to a valid range
        float height = settings.height;
        if(height < 2.f) {
            height = 2.f;
        }
        else if(height > 4.f) {
            height = 4.f;
        }
        curEye = glm::vec4(bez.x, height, bez.y, 1.f);
        m_prev_eye = curEye;
        m_prev_time = m_runningTime;
    }
    glm::vec4 look;
    // look away from egg
    if(settings.facingEgg) {
        look = -(glm::vec4(glm::vec3(0.f), 1.f) - curEye);
        look.y = -.5f;
    }
    // look at egg
    else {
        look = glm::vec4(glm::vec3(0.f), 1.f) - curEye;
    }
    glm::vec4 up = glm::vec4(0.f, 1.f, 0.f, 0.f);
    m_camera->orientLook(curEye, look, up);
}

// Gets the point on the first defined piecewise bezier curve
glm::vec2 View::bezierPathOne() {
    // defining total number of seconds for this curve
    float total_seconds = 20.f;
    float div = 5.f;
    if (m_runningTime > total_seconds) {
        m_runningTime = 0.f;
    }
    float t_value = m_runningTime/div;
    glm::vec2 bez;
    // defining points for each piece of the piecewise curve
    if(t_value < 1.f) {
        glm::vec2 p0(-.5f, -.5f);
        glm::vec2 p1(-1.5f, -.5f);
        glm::vec2 p2(-1.5f, .5f);
        glm::vec2 p3(-.5f, .5f);
        bez = bezierPoint(t_value, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    else if(t_value < 2.f) {
        // have to shift t-value to get valid range
        float t_value_shifted = t_value - 1.f;
        glm::vec2 p0(-.5f, .5f);
        glm::vec2 p1(-.5f, 1.5f);
        glm::vec2 p2(.5f, 1.5f);
        glm::vec2 p3(.5f, .5f);
        bez = bezierPoint(t_value_shifted, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    else if(t_value < 3.f) {
        float t_value_shifted = t_value - 2.f;
        glm::vec2 p0(.5f, .5f);
        glm::vec2 p1(1.5f, .5f);
        glm::vec2 p2(1.5f, -.5f);
         glm::vec2 p3(.5f, -.5f);
        bez = bezierPoint(t_value_shifted, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    else {
        float t_value_shifted = t_value - 3.f;
        glm::vec2 p0(.5f, -.5f);
        glm::vec2 p1(.5f, -1.5f);
        glm::vec2 p2(-.5f, -1.5f);
        glm::vec2 p3(-.5f, -.5f);
        bez = bezierPoint(t_value_shifted, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    return bez;
}

// Gets the point on the second defined piecewise bezier curve
glm::vec2 View::bezierPathTwo() {
    // defining total number of seconds for this curve
    float total_seconds = 20.f;
    float div = 5.f;
    if (m_runningTime > total_seconds) {
        m_runningTime = 0.f;
    }
    float t_value = m_runningTime/div;
    glm::vec2 bez;
    // defining points for each piece of the piecewise curve
    if(t_value < 1.f) {
        glm::vec2 p0(0.f, -1.f);
        glm::vec2 p1(2.f, -.8f);
        glm::vec2 p2(2.f, -.2f);
        glm::vec2 p3(1.f, 0.f);
        bez = bezierPoint(t_value, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    else if(t_value < 2.f) {
        // have to shift t-value to get valid range
        float t_value_shifted = t_value - 1.f;
        glm::vec2 p0(1.f, 0.f);
        glm::vec2 p1(2.f, .2f);
        glm::vec2 p2(2.f, .8f);
        glm::vec2 p3(0.f, 1.f);
        bez = bezierPoint(t_value_shifted, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);

    }
    else if(t_value < 3.f) {
        float t_value_shifted = t_value - 2.f;
        glm::vec2 p0(0.f, 1.f);
        glm::vec2 p1(-2.f, .8f);
        glm::vec2 p2(-2.f, .2f);
        glm::vec2 p3(-1.f, 0.f);
        bez = bezierPoint(t_value_shifted, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    else {
        float t_value_shifted = t_value - 3.f;
        glm::vec2 p0(-1.f, 0.f);
        glm::vec2 p1(-2.f, -.2f);
        glm::vec2 p2(-2.f, -.8f);
        glm::vec2 p3(0.f, -1.f);
        bez = bezierPoint(t_value_shifted, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    return bez;
}

// Returns a point on the third bezier path
glm::vec2 View::bezierPathThree() {
    // defining total number of seconds for this curve
    float total_seconds = 20.f;
    float div = 10.f;
    if (m_runningTime > total_seconds) {
        m_runningTime = 0.f;
    }
    float t_value = m_runningTime/div;
    glm::vec2 bez;
    // defining points for each piece of the piecewise curve
    if(t_value < 1.f) {
        glm::vec2 p0(0.f, -1.f);
        glm::vec2 p1(-1.5f, -1.f);
        glm::vec2 p2(-1.5f, 1.f);
        glm::vec2 p3(0.f, 1.f);
        bez = bezierPoint(t_value, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    else if(t_value < 2.f) {
        // have to shift t-value to get valid range
        float t_value_shifted = t_value - 1.f;
        glm::vec2 p0(0.f, 1.f);
        glm::vec2 p1(1.5f, 1.f);
        glm::vec2 p2(1.5f, -1.f);
        glm::vec2 p3(0.f, -1.f);
        bez = bezierPoint(t_value_shifted, m_scale * p0, m_scale * p1, m_scale * p2, m_scale * p3);
    }
    return bez;
}

// Gets the point on the cubic bézier curve with points p0, p1, p2, p3
glm::vec2 View::bezierPoint(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    return std::powf((1.f - t), 3.f) * p0
            + 3.f * std::powf((1.f - t), 2.f) * t * p1
            + 3.f * (1.f - t) * std::powf(t, 2.f) * p2
            + std::powf(t, 3.f) * p3;
}
