#include "SkullVolume.hpp"


// init the 1 dimentional texture for transfer function
GLuint initTFF1DTex(const char* filename)
{
    // read in the user defined data of transfer function
    std::ifstream inFile(filename, std::ifstream::in);
    if (!inFile)
    {
        std::cerr << "Error openning file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    const int MAX_CNT = 10000;
    GLubyte *tff = (GLubyte *) calloc(MAX_CNT, sizeof(GLubyte));
    inFile.read(reinterpret_cast<char *>(tff), MAX_CNT);
    if (inFile.eof())
    {
        size_t bytecnt = inFile.gcount();
        *(tff + bytecnt) = '\0';
        std::cout << "bytecnt " << bytecnt << std::endl;
    }
    else if(inFile.fail())
    {
        std::cout << filename << "read failed " << std::endl;
    }
    else
    {
        std::cout << filename << "is too large" << std::endl;
    }

    GLuint tff1DTex;

    glGenTextures(1, &tff1DTex);
    glBindTexture(GL_TEXTURE_1D_ARRAY, tff1DTex);

    int n = 256;
    int numTransferFunctions = 1;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, GL_RGBA8, n, numTransferFunctions, 0, GL_RGBA, GL_UNSIGNED_BYTE, tff);

    glTexParameteri(GL_TEXTURE_1D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_1D_ARRAY, 0);

    free(tff);
    return tff1DTex;
}

// init 3D texture to store the volume data used fo ray casting
GLuint initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d)
{

    FILE *fp;
    size_t size = w * h * d;
    GLubyte *data = new GLubyte[size];			  // 8bit
    if (!(fp = fopen(filename, "rb")))
    {
        std::cout << "Error: opening .raw file failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "OK: open .raw file successed" << std::endl;
    }
    if ( fread(data, sizeof(char), size, fp)!= size)
    {
        std::cout << "Error: read .raw file failed" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "OK: read .raw file successed" << std::endl;
    }
    fclose(fp);

    GLuint volTex;
    glGenTextures(1, &volTex);
    // bind 3D texture target
    glBindTexture(GL_TEXTURE_3D, volTex);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    // pixel transfer happens here from client to OpenGL server
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, w, h, d, 0, GL_RED, GL_UNSIGNED_BYTE,data);

    delete []data;
    std::cout << "volume texture created" << std::endl;
    return volTex;
}


/**
 * @brief Constructor.
 */
SkullVolume::SkullVolume(const ugl::BoundingBox& boundingBox, const std::string& volumePath, const std::string& transferPath)
{
    this->boundingBox = boundingBox;
    this->intensityTexture3D = initVol3DTex(volumePath.c_str(), 256, 256, 225);
    this->transferTexture1D = initTFF1DTex(transferPath.c_str());

    this->initBoundingBoxDrawable();
}


/**
 * @brief Destructor.
 */
SkullVolume::~SkullVolume()
{
    glDeleteTextures(1, &this->intensityTexture3D);
    glDeleteTextures(1, &this->transferTexture1D);
}


/**
 * @brief Sets the sample step size.
 * @param size
 */
void SkullVolume::setSampleStepSize(float size)
{
    this->sampleStepSize = size;
}


/**
 * @brief Returns the bounding box.
 * @return
 */
ugl::BoundingBox SkullVolume::getBoundingBox() const
{
    return this->boundingBox;
}


/**
 * @brief Returns the intensity texture.
 * @return
 */
GLuint SkullVolume::getIntensityTexture() const
{
    return this->intensityTexture3D;
}


/**
 * @brief Returns the resolution of the intensity volume.
 * @return
 */
glm::ivec3 SkullVolume::getIntensityResolution() const
{
    return glm::ivec3(256, 256, 225);
}


/**
 * @brief We only have a single transfer texture.
 * @return
 */
int SkullVolume::getNumTransferTextures() const
{
    return 1;
}


/**
 * @brief Returns the transfer texture.
 * @return
 */
GLuint SkullVolume::getTransferArrayTexture() const
{
    // We just have one linear transfer texture, i.e., a single layer in the array texture
    return this->transferTexture1D;
}


/**
 * @brief Volume opacity.
 * @return
 */
glm::vec4 SkullVolume::getAlpha() const
{
    // README:  This is a hack. Actually, alpha should be in normalized [0,1] range.
    //          In this case, it is used to boost the weak transfer function, which
    //          seems to be designed for calculation without the correct light
    //          transport integral (i.e., multiplying alpha by sample step size).
    return glm::vec4(1.0f / this->getSampleStepSize());
}


/**
 * @brief Returns the resolution of the transfer texture.
 * @return
 */
int SkullVolume::getTransferResolution() const
{
    return 256;
}


/**
 * @brief Returns the sample step size;
 * @return
 */
float SkullVolume::getSampleStepSize() const
{
    return this->sampleStepSize;
}

