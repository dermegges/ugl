#include <glm/glm.hpp>

#include <ugl/Drawable.hpp>
#include <ugl/StateSet.hpp>
#include <ugl/BoundingBox.hpp>

#include <fstream>


/**
 * Afront streaming mesh.
 */
class StreamingMesh : public ugl::Drawable
{
public:
    StreamingMesh(const std::string& path);
    virtual ~StreamingMesh();

    bool initGL();

    void draw(const ugl::StateSet& state);
    ugl::BoundingBox getBoundingBox() const;

private:
    void updateData();

private:
    std::string path;
    long offset = 0;

    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec3> triangles;

    ugl::BoundingBox boundingBox;
    ugl::VariantProgram program;

    GLuint vertexArray;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint triangleBuffer;
    GLsizei triangleCount;
    GLuint edgeBuffer;
    GLsizei edgeCount;

    bool autoUpdate = true;
    bool drawSurface = true;
    bool drawEdges = true;
    float alpha = 0.5f;
};


