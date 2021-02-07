#include <glm/glm.hpp>

#include <ugl/CoreApplication.hpp>
#include <ugl/BoundingBox.hpp>
#include <ugl/MeshData.hpp>
#include <ugl/MeshDrawable.hpp>
#include <ugl/TransparentRenderStage.hpp>
#include <ugl/ViewController3D.hpp>


/**
 * A simple viewer example showing a tetrahedron by MeshDrawable
 * and does not need external data files.
 */
class BasicViewer : public ugl::CoreApplication
{
public:
    BasicViewer();
    virtual ~BasicViewer();

protected:
    void initGL();
    void drawGL();
    void resizeGL(int width, int height);

    bool onMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt);
    bool onMouseMove(int x, int y);
    bool onMouseRelease(bool left, bool middle, bool right);

    bool onKeyPress(int code, bool shift, bool control, bool alt);

private:
    ugl::MeshDrawable* meshDrawable;
    ugl::TransparentRenderStage* transparentRenderStage;

    ugl::StateSet stateSet;
    glm::vec3 lightDirection;
    float lightGlossiness;
    glm::vec3 defaultColor;

    bool showTweakBar;
    bool tweakBarHasMouse;
    unsigned int numSamples;
};


