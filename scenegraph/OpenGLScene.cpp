#include "OpenGLScene.h"

#include <GL/glew.h>

#include "Settings.h"

OpenGLScene::~OpenGLScene()
{
}

void OpenGLScene::setClearColor() {
    glClearColor(0, 0, 0, 0);
}

void OpenGLScene::settingsChanged() {

}
