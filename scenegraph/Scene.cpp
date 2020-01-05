#include "Scene.h"
#include "camera/Camera.h"
#include "lib/CS123ISceneParser.h"
#include "scenegraph/TextureUtils.h"
#include "gl/textures/Texture2D.h"

#include <iostream>

#include "glm/gtx/transform.hpp"

Scene::Scene()
{
}

/** Copy constructor **/
Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = {1, 1, 1, 1};
    setGlobal(global);

    // Copy over objects
    std::list<ObjectListNode>::iterator primIt;
    for (primIt = scene.m_objectList.begin(); primIt != scene.m_objectList.end(); ++primIt) {
        m_objectList.push_back(*primIt);
    }

    // Copy over lights
    std::list<CS123SceneLightData>::iterator lightIt;
    for (lightIt = scene.m_lightList.begin(); lightIt != scene.m_lightList.end(); ++lightIt) {
        m_lightList.push_back(*lightIt);
    }

    // Copy over global data
    setGlobal(scene.m_globalData);
}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // Set global data
    CS123SceneGlobalData global;
    parser->getGlobalData(global);
    sceneToFill->setGlobal(global);

    // Add all lights
    int numLights = parser->getNumLights();
    for (int i = 0; i < numLights; i++) {
        CS123SceneLightData light;
        parser->getLightData(i, light);
        sceneToFill->addLight(light);
    }

    // Add all objects
    CS123SceneNode *root = parser->getRootNode();
    glm::mat4x4 initMat = glm::mat4x4();
    sceneToFill->addAllObjects(root, initMat);
}

void Scene::addAllObjects(const CS123SceneNode *curNode, const glm::mat4x4 &cumMat) {
    glm::mat4x4 curMat = cumMat;
    int numTransforms = static_cast<int>(curNode->transformations.size());
    for (int i = 0; i < numTransforms; i++) {
        CS123SceneTransformation *curTrans = curNode->transformations.at(i);
        glm::mat4x4 transformMat;

        // Calculate transformation matrices based on type of transform.
        switch(curTrans->type) {
            case TRANSFORMATION_TRANSLATE: {
                transformMat = glm::translate(curTrans->translate);
                break;
            }
            case TRANSFORMATION_ROTATE: {
                transformMat = glm::rotate(curTrans->angle, curTrans->rotate);
                break;
            }
            case TRANSFORMATION_SCALE: {
                transformMat = glm::scale(curTrans->scale);
                break;
            }
            case TRANSFORMATION_MATRIX: {
                transformMat = curTrans->matrix;
                break;
            }
        }

        // Apply transform to current cumulative matrix.
        curMat = curMat * transformMat;
    }

    // Add primitive to list.
    int numPrims = static_cast<int>(curNode->primitives.size());
    for (int i = 0; i < numPrims; i++) {
        addObjects(*(curNode->primitives.at(i)), curMat);
    }
	
    // Recursive call to all children.
    int numChildren = static_cast<int>(curNode->children.size());
    for (int i = 0; i < numChildren; i++) {
        CS123SceneNode *curChild = curNode->children.at(i);
        addAllObjects(curChild, curMat);
    }
}

void Scene::addObjects(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    std::shared_ptr<Texture2D> textureMap;
    std::shared_ptr<QImage> textureMapImage;
    if (scenePrimitive.material.textureMap.isUsed) {
        QImage tempImage = TextureUtils::getTextureImage(scenePrimitive.material.textureMap.filename);
        textureMapImage = std::make_shared<QImage>(tempImage.convertToFormat(QImage::Format_RGBA8888));
        textureMap = std::make_shared<Texture2D>(
                    textureMapImage->bits(), textureMapImage->width(), textureMapImage->height());
	}

    std::shared_ptr<Texture2D> bumpMap;
    std::shared_ptr<QImage> bumpMapImage;
    if (scenePrimitive.material.bumpMap.isUsed) {
        QImage tempImage = TextureUtils::getTextureImage(scenePrimitive.material.bumpMap.filename);
        bumpMapImage = std::make_shared<QImage>(tempImage.convertToFormat(QImage::Format_RGBA8888));
        bumpMap = std::make_shared<Texture2D>(
                    bumpMapImage->bits(), bumpMapImage->width(), bumpMapImage->height());
    }
    ObjectListNode node = {scenePrimitive, NULL, matrix, textureMapImage, textureMap, bumpMapImage, bumpMap};
    m_objectList.push_back(node);
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lightList.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_globalData = global;
}
