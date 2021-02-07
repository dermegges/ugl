#include "SurfaceRenderer.hpp"
#include "SurfaceVectorData.hpp"
#include "SkullVolume.hpp"

#include <glm/glm.hpp>

#include <ugl/CoreApplication.hpp>
#include <ugl/BoundingBox.hpp>
#include <ugl/TransparentRenderStage.hpp>
#include <ugl/ViewController3D.hpp>


/**
 * A viewer showing a surface loaded by SurfaceVectorData.
 */
class Viewer: public ugl::CoreApplication
{
public:
    Viewer();
    virtual ~Viewer();

protected:
    void initGL();
    void drawGL();
    void resizeGL(int width, int height);

    bool onMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt);
    bool onMouseMove(int x, int y);
    bool onMouseRelease(bool left, bool middle, bool right);

    bool onKeyPress(int code, bool shift, bool control, bool alt);

private:
    SurfaceRenderer* surfaceRenderer;
    SkullVolume* volume;
    ugl::TransparentRenderStage* transparentRenderStage;

    ugl::StateSet stateSet;
    glm::vec3 lightDirection;
    float lightGlossiness;
    glm::vec3 defaultColor;

    unsigned int numSamples;

    bool volumeRenderingEnabled;
    float volumeRenderingSampleStepLength;

    bool showTweakBar;
    bool tweakBarHasMouse;    
};

