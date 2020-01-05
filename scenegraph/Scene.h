#ifndef SCENE_H
#define SCENE_H

#include "lib/CS123SceneData.h"
#include "shapes/MeshShape.h"
#include <memory>
#include <map>
#include <list>
#include <QImage>

class Camera;
class CS123ISceneParser;
class Shape;

namespace CS123 { namespace GL {

class Texture2D;

}}

struct ObjectListNode {
    const CS123ScenePrimitive primitive;
    MeshShape* mesh;
    glm::mat4x4 matrix;
    std::shared_ptr<QImage> textureMapImage;
    std::shared_ptr<Texture2D> textureMap;
    std::shared_ptr<QImage> bumpMapImage;
    std::shared_ptr<Texture2D> bumpMap;
};

/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

protected:

    // Adds a primitive to the scene.
    virtual void addObjects(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    // Recursive function for adding all primitives to list and keeping track of the
    // cumulative matrix.
    virtual void addAllObjects(const CS123SceneNode *curNode, const glm::mat4x4 &cumMat);

    std::list<ObjectListNode> m_objectList;
    std::list<CS123SceneLightData> m_lightList;
    CS123SceneGlobalData m_globalData;
};

#endif // SCENE_H
