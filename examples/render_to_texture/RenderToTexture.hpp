#include <glm/glm.hpp>

#include <ugl/CoreApplication.hpp>
#include <ugl/BoundingBox.hpp>
#include <ugl/TransparentRenderStage.hpp>
#include <ugl/ViewController3D.hpp>

#include "RedTriangle.hpp"


/**
 * A simple example demonstrating how to render to a texture within a drawable.
 */
class RenderToTexture : public ugl::CoreApplication
{
public:
    RenderToTexture();
    virtual ~RenderToTexture();

protected:
    void initGL();
    void drawGL();

private:
    ugl::TransparentRenderStage* transparentRenderStage;    
    ugl::StateSet stateSet;    

    RedTriangle* drawable;
};


