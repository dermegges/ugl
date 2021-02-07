#include "StreamingMesh.hpp"

#include <glm/glm.hpp>

#include <ugl/CoreApplication.hpp>
#include <ugl/BoundingBox.hpp>
#include <ugl/TransparentRenderStage.hpp>
#include <ugl/ViewController3D.hpp>


/**
 * Interactive Afront streaming mesh viewer.
 */
class AfrontViewer : public ugl::CoreApplication
{
public:
    AfrontViewer(const std::string& path);
    virtual ~AfrontViewer();

protected:
    void initGL();
    void drawGL();
    void resizeGL(int width, int height);

    bool onMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt);
    bool onMouseMove(int x, int y);
    bool onMouseRelease(bool left, bool middle, bool right);

    bool onKeyPress(int code, bool shift, bool control, bool alt);

private:
    StreamingMesh* mesh;
    ugl::TransparentRenderStage* transparentRenderStage;

    ugl::StateSet stateSet;
    glm::vec3 lightDirection;
    float lightGlossiness;
    glm::vec3 defaultColor;

    bool showTweakBar;
    bool tweakBarHasMouse;
};


