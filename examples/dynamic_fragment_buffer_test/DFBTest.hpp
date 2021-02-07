#include <glm/glm.hpp>

#include "SurfaceRenderer.hpp"

#include <ugl/CoreApplication.hpp>
#include <ugl/BoundingBox.hpp>
#include <ugl/TransparentRenderStageDFB.hpp>
#include <ugl/ViewController3D.hpp>


/**
 * Simple example demonstrating usage of the dynamic fragment buffer renderer.
 */
class DFBTest : public ugl::CoreApplication
{
public:
    DFBTest();
    virtual ~DFBTest();

protected:
    void initGL();
    void drawGL();
    void resizeGL(int width, int height);

    bool onMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt);
    bool onMouseMove(int x, int y);
    bool onMouseRelease(bool left, bool middle, bool right);

    bool onKeyPress(int code, bool shift, bool control, bool alt);

private:
    ugl::TransparentRenderStageDFB* transparentRenderStage;
    SurfaceRenderer* surfaceRenderer;

    ugl::StateSet stateSet;
    glm::vec3 lightDirection;
    float lightGlossiness;
    glm::vec3 defaultColor;

    bool showTweakBar;
    bool tweakBarHasMouse;
};


