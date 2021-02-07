#ifndef SKULLVOLUME_HPP
#define SKULLVOLUME_HPP

#include <ugl/Volume.hpp>

#include <string>


class SkullVolume : public ugl::Volume
{
public:
    SkullVolume(const ugl::BoundingBox& boundingBox, const std::string& volumePath, const std::string& transferPath);
    virtual ~SkullVolume();

    void setSampleStepSize(float size);

    virtual ugl::BoundingBox getBoundingBox() const;

    virtual GLuint getIntensityTexture() const;
    virtual glm::ivec3 getIntensityResolution() const;

    virtual int getNumTransferTextures() const;
    virtual GLuint getTransferArrayTexture() const;
    virtual glm::vec4 getAlpha() const;
    virtual int getTransferResolution() const;

    virtual float getSampleStepSize() const;

private:
    ugl::BoundingBox boundingBox;
    float sampleStepSize;
    GLuint intensityTexture3D;
    GLuint transferTexture1D;
};


#endif
