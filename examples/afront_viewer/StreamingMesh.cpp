#include "StreamingMesh.hpp"

#include <AntTweakBar.h>

#include <ugl/ErrorCheck.hpp>

#include <sstream>


/**
 * @brief Constructor.
 * @param path
 */
StreamingMesh::StreamingMesh(const std::string& path) : path(path)
{
}


/**
 * @brief Destructor.
 */
StreamingMesh::~StreamingMesh()
{
}


/**
 * @brief Initialization.
 * @return
 */
bool StreamingMesh::initGL()
{
    // Init tweakbar
    TwBar* meshBar = TwNewBar("Streaming Mesh");
    TwAddVarRW(meshBar, "surface", TW_TYPE_BOOLCPP, &this->drawSurface, "label='Draw Surface' key=s");
    TwAddVarRW(meshBar, "edges",   TW_TYPE_BOOLCPP, &this->drawEdges, "label='Draw Edges' key=e");
    TwAddVarRW(meshBar, "alpha",   TW_TYPE_FLOAT,   &this->alpha, "label='Alpha' min=0.0 max=1.0 step=0.01");
    TwAddSeparator(meshBar, "", "");
    TwAddVarRW(meshBar, "autoUpdate", TW_TYPE_BOOLCPP, &this->autoUpdate, "label='Auto Update' key=u");

    // Init shader
    this->program.addImportPath("shader");
    this->program.addShaderFromSourceFile(ugl::COMBINED, "ugl/mesh.glsl");
    this->program.addAttributeLocation("vertexPosition", 0);
    this->program.addAttributeLocation("vertexNormal",   1);

    // Init buffers
    glGenVertexArrays(1, &this->vertexArray);
    glBindVertexArray(this->vertexArray);

    glGenBuffers(1, &this->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &this->normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenBuffers(1, &this->triangleBuffer);
    this->triangleCount = 0;

    glGenBuffers(1, &this->edgeBuffer);
    this->edgeCount = 0;

    // Load initial data
    this->updateData();

    return true;
}


/**
 * @brief Drawing.
 * @param state
 */
void StreamingMesh::draw(const ugl::StateSet& s)
{
    /*
     * Read new contents from file
     */
    if (this->autoUpdate)
        this->updateData();


    /*
     * Render
     */
    ugl::StateSet state(s);

    ugl::UniformSet& uniforms = state.getOrCreateUniforms();
    ugl::ModeSet& modes = state.getOrCreateModes();

    uniforms.set("alpha", this->alpha);

    glBindVertexArray(this->vertexArray);

    if (this->drawSurface)
    {
        modes.clear("LINE_MODE");

        state.apply(this->program);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->triangleBuffer);
        glDrawElements(GL_TRIANGLES, this->triangleCount, GL_UNSIGNED_INT, nullptr);
    }

    if (this->drawEdges)
    {
        modes.set("LINE_MODE", 1);

        state.apply(this->program);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->edgeBuffer);
        glDrawElements(GL_LINES, this->edgeCount, GL_UNSIGNED_INT, nullptr);
    }

    glBindVertexArray(0);

    checkGLError();
}


/**
 * @brief Bounding box.
 * @return
 */
ugl::BoundingBox StreamingMesh::getBoundingBox() const
{
    std::cout << this->boundingBox.getMin().x << ", " << this->boundingBox.getMin().y << ", " << this->boundingBox.getMin().z << " / " << this->boundingBox.getMax().x << ", " << this->boundingBox.getMax().y << ", " << this->boundingBox.getMax().z << std::endl;

    return this->boundingBox;
}


/**
 * @brief Load new data from the file.
 */
void StreamingMesh::updateData()
{
    std::ifstream stream(this->path);
    stream.seekg(this->offset);

    uint numVertices = 0;
    uint numFaces = 0;

    std::string line;
    while (std::getline(stream, line))
    {
        this->offset += (line.size() + 1);

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // Vertex
        if (type.find("Vertex") == 0)
        {
            int index;
            glm::vec3 vertex;

            iss >> index >> vertex.x >> vertex.y >> vertex.z;

            // Add vertex
            this->vertices.push_back(vertex);
            ++numVertices;

            // Update bounding box
            this->boundingBox.add(vertex);
        }

        // Face
        else if (type.find("Face") == 0)
        {
            int index;
            glm::uvec3 face;

            iss >> index >> face.x >> face.y >> face.z;

            --face.x;
            --face.y;
            --face.z;

            this->triangles.push_back(face);
            ++numFaces;
        }
    }

    stream.close();

    if (numFaces > 0)
    {
        if (numVertices > 0)
        {
            // Update vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_DYNAMIC_DRAW);
        }

        // Update normal buffer
        std::vector<glm::vec3> normals;
        normals.resize(this->vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

        for (const glm::uvec3& t : this->triangles)
        {
            glm::vec3 n = glm::cross(this->vertices[t.y] - this->vertices[t.x], this->vertices[t.z] - this->vertices[t.x]);

            normals[t.x] += n;
            normals[t.y] += n;
            normals[t.z] += n;
        }

        for (glm::vec3& n : normals)
            n = glm::normalize(n);

        glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_DYNAMIC_DRAW);

        // Update triangle buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->triangleBuffer);
        glBufferData(GL_ARRAY_BUFFER, this->triangles.size() * sizeof(glm::uvec3), &this->triangles[0], GL_DYNAMIC_DRAW);
        this->triangleCount = this->triangles.size() * 3;

        // Update edge buffer
        std::vector<glm::uvec2> edges;
        edges.reserve(this->triangles.size() * 3);
        for (const glm::uvec3& t : this->triangles)
        {
            edges.push_back(glm::uvec2(t.x, t.y));
            edges.push_back(glm::uvec2(t.y, t.z));
            edges.push_back(glm::uvec2(t.z, t.x));
        }

        glBindBuffer(GL_ARRAY_BUFFER, this->edgeBuffer);
        glBufferData(GL_ARRAY_BUFFER, edges.size() * sizeof(glm::uvec2), &edges[0], GL_DYNAMIC_DRAW);
        this->edgeCount = edges.size() * 2;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
