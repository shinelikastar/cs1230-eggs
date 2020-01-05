#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "camera/Camera.h"
#include "camera/CamtransCamera.h"
#include "Settings.h"
#include "ui/view.h"
#include "ResourceLoader.h"
#include "shapes/MeshShape.h"
#include "gl/shaders/CS123Shader.h"
#include "scenegraph/TextureUtils.h"
#include "gl/textures/Texture2D.h"
#include <QImage>
#include <string.h>
#include <iostream>

using namespace CS123::GL;

SceneviewScene::SceneviewScene()
{
    loadPhongShader();
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::render(View *context) {
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms(context);
    setLights();
//    glBindTexture(GL_TEXTURE_2D, 0);
    renderGeometry();
    m_phongShader->unbind();
}

void SceneviewScene::setSceneUniforms(View *context) {
    CamtransCamera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, View *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights() {
    std::list<CS123SceneLightData>::iterator it;
    for (it = m_lightList.begin(); it != m_lightList.end(); ++it) {
        m_phongShader->setLight(*it);
    }
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Iterate through each primitive.
    std::list<ObjectListNode>::iterator it;

    for (it = m_objectList.begin(); it != m_objectList.end(); ++it) {

        // Set transformation matrix
        m_phongShader->setUniform("m", it->matrix);
        m_phongShader->setUniform("useTexture", 0);
        m_phongShader->setUniform("useBumpMap", 0);

        if (it->primitive.material.textureMap.isUsed && settings.enableTextures) {
            m_phongShader->setUniform("useTexture", 1);
            glActiveTexture(GL_TEXTURE0);
            it->textureMap->bind();
            m_phongShader->setTexture("tex", *(it->textureMap));
            m_phongShader->setUniform("repeatU", it->primitive.material.textureMap.repeatU);
            m_phongShader->setUniform("repeatV", it->primitive.material.textureMap.repeatV);
        }

        if (it->primitive.material.bumpMap.isUsed && settings.enableDisplacement) {
            glActiveTexture(GL_TEXTURE1);
            m_phongShader->setUniform("useBumpMap", true);
            it->bumpMap->bind();
            m_phongShader->setTexture("bump", *(it->bumpMap));
            m_phongShader->setUniform("repeatBumpU", it->primitive.material.bumpMap.repeatU);
            m_phongShader->setUniform("repeatBumpV", it->primitive.material.bumpMap.repeatV);
        }

        // Copy material from const primitive
        CS123SceneMaterial material = it->primitive.material;

        // Apply global parameters to diffuse component
        material.cDiffuse.r = m_globalData.kd * material.cDiffuse.r;
        material.cDiffuse.g = m_globalData.kd * material.cDiffuse.g;
        material.cDiffuse.b = m_globalData.kd * material.cDiffuse.b;

        // Apply global parameters to ambient component
        material.cAmbient.r = m_globalData.ka * material.cAmbient.r;
        material.cAmbient.g = m_globalData.ka * material.cAmbient.g;
        material.cAmbient.b = m_globalData.ka * material.cAmbient.b;

        // Apply new material
        m_phongShader->applyMaterial(material);

        // Draw
        CS123ScenePrimitive prim = it->primitive;
        if (prim.type == PrimitiveType::PRIMITIVE_MESH) {
            it->mesh->draw();
        }


        if (it->primitive.material.textureMap.isUsed) {
            it->textureMap->unbind();
        }

        if (it->primitive.material.bumpMap.isUsed) {
            it->bumpMap->unbind();
        }

    }
}

void SceneviewScene::settingsChanged() {
}

void SceneviewScene::loadMeshes() {
    std::list<ObjectListNode>::iterator objIt;

    for (objIt = m_objectList.begin(); objIt != m_objectList.end(); ++objIt) {
        const CS123ScenePrimitive* prim = &(objIt->primitive);
        if (prim->type == PrimitiveType::PRIMITIVE_MESH) {

            // Search for mesh in list of loaded meshes so that we aren't
            // loading two copies of the same mesh.
            std::list<std::unique_ptr<MeshShape>>::iterator meshIt;
            for (meshIt = m_loadedMeshes.begin(); meshIt != m_loadedMeshes.end(); ++meshIt) {
                if ((*meshIt)->getPath().compare(prim->meshfile) == 0) {
                    objIt->mesh = meshIt->get();
                    break;
                }
            }

            // Mesh does not exist in the loaded meshes list. So we
            // push it into the list and get a pointer to it.
            if (objIt->mesh == NULL) {
                m_loadedMeshes.push_back(
                            std::make_unique<MeshShape>(prim->meshfile));
                objIt->mesh = m_loadedMeshes.back().get();
            }

        } else {
            fprintf(stderr, "Non-mesh object encountered in scenegraph!");
            exit(1);
        }
    }

    std::cout << m_loadedMeshes.back()->getPath() << std::endl;
}
