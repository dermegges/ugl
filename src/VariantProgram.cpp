/** @file VariantProgram.cpp



Copyright 2016 Computational Topology Group, University of Kaiserslautern

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

    Author(s): C.Garth, T.Biedert
*/

#include "ugl/VariantProgram.hpp"
#include "ugl/GLSLPreprocessor.hpp"
#include "ugl/SourceSplitter.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <functional>

#include "ugl/FileSystemWatcher.hpp"


namespace ugl
{

VariantProgram::VariantProgram()
{
    // Add default import path
    std::string path = getBaseDir() + "/shader";
    //    std::cout << "Default shader import path: " << path << std::endl;

    this->addImportPath(path);
}

VariantProgram::~VariantProgram()
{
    FileSystemWatcher::getInstance().remove(this);
    clearCache();
}

// -------------------------------------------------------------------------

void VariantProgram::addImportPath(const std::string& path)
{
    FileSystemWatcher::getInstance().watch(path, this);

    m_importPaths.push_back(path);
    clearCache();
}

// -------------------------------------------------------------------------

void VariantProgram::addShaderFromSourceFile(ShaderType type, const std::string& fileName)
{
    // Determine complete path by also considering import paths
    std::string path = this->searchImports(fileName);

    FileSystemWatcher::getInstance().watch(path, this);

    m_shaderFiles.push_back(ShaderFile(type, path));
    clearCache();
}

// -------------------------------------------------------------------------

void VariantProgram::fileEvent(const std::string& /*path*/)
{
    this->clearCache();
}

// -------------------------------------------------------------------------

void VariantProgram::addAttributeLocation(const std::string& name, GLuint location)
{
    int max_attribs;
    glGetIntegerv (GL_MAX_VERTEX_ATTRIBS, &max_attribs);

    if(location < (GLuint)max_attribs)
    {
        m_attributeLocations.push_back(AttributeLocation(name, location));
        clearCache();
    }
    else 
        std::cerr<<"maximum vertex attributes count ("<<max_attribs<<") exceeded"<<std::endl;
}

// -------------------------------------------------------------------------

ShaderProgram& VariantProgram::bind()
{
    return bind(DefineMap());
}

// ------------------------------------------------------------------------

void VariantProgram::clearCache()
{
    for (CompiledProgramMap::const_iterator compiledProgram = m_compiledProgramMap.begin(); compiledProgram != m_compiledProgramMap.end(); ++compiledProgram)
        delete compiledProgram->second;

    m_compiledProgramMap.clear();
}

// ------------------------------------------------------------------------

std::string VariantProgram::searchImports(const std::string& path)
{
    std::string result = path;

    std::vector<std::string> candidates;
    candidates.push_back(path);

    for (std::vector<std::string>::const_iterator importPath = m_importPaths.begin(); importPath != m_importPaths.end(); ++importPath)
        candidates.push_back(*importPath + "/" + path);

    bool fileFound = false;

    for (std::vector<std::string>::const_iterator c = candidates.begin(); c != candidates.end(); ++c)
    {
        bool fileExists = std::ifstream(*c).good();

        if (fileExists)
        {
            result = *c;
            fileFound = true;
            break;
        }
    }

    if (!fileFound)
        std::cerr << "Error: Shader file not found! (" << path << ")" << std::endl;

    return result;
}


// -------------------------------------------------------------------------

GLuint VariantProgram::getUnusedAttributeLocation() const
{
    GLuint max = 0;
    for (std::vector<AttributeLocation>::const_iterator attributeLocation
         = m_attributeLocations.begin();
         attributeLocation != m_attributeLocations.end();
         ++attributeLocation)
        max = std::max(max, attributeLocation->second + 1);

    return max;
}

// ------------------------------------------------------------------------

class AddDefineVisitor : public boost::static_visitor<>
{
public:
    AddDefineVisitor(GLSLPreprocessor& preprocessor, const std::string& name) :
        m_preprocessor(preprocessor), m_name(name)
    {
    }

    template <typename T>
    void operator()(const T& value) const
    {
        m_preprocessor.define(m_name, value);
    }

private:
    GLSLPreprocessor& m_preprocessor;
    const std::string& m_name;
};

// ------------------------------------------------------------------------

ShaderProgram& VariantProgram::bind(const DefineMap& defineMap)
{
    // Update file watcher
    FileSystemWatcher::getInstance().update();

    // Bind
    CompiledProgramMap::iterator it = m_compiledProgramMap.find(defineMap);
    ShaderProgram* program;

    if (it != m_compiledProgramMap.end())
    {
        program = it->second;
    }
    else
    {
        // --- set up shader program
        program = new ShaderProgram;
        m_compiledProgramMap[defineMap] = program;
        GLSLPreprocessor preprocessor;

        // add import paths
        for (std::vector<std::string>::const_iterator path = m_importPaths.begin(); path != m_importPaths.end(); ++path)
            preprocessor.add_import_path(*path);

        // add define statements from defineMap
        for (DefineMap::const_iterator defineToken = defineMap.begin();
             defineToken != defineMap.end(); ++defineToken)
        {
            boost::apply_visitor(
                        AddDefineVisitor(preprocessor, defineToken->first),
                        defineToken->second);
        }

        // preprocess, load and compile shaders
        for (std::vector<ShaderFile>::const_iterator shaderFile = m_shaderFiles.begin(); shaderFile != m_shaderFiles.end(); ++shaderFile)
        {
            std::string path = shaderFile->second;

            // Process
            if (shaderFile->first == COMBINED)
            {
                SourceSplitter splitter;
                SourceSplitter::SourceStrings sourceStrings = splitter.processFile(path);

                for (SourceSplitter::SourceStrings::const_iterator sourceString = sourceStrings.begin(); sourceString != sourceStrings.end(); ++sourceString)
                {
                    std::string preprocessedSource = preprocessor.process(sourceString->second, &(path));
                    program->addShaderFromSourceCode(sourceString->first, preprocessedSource);
                }
            }
            else
            {
                std::string preprocessedSource = preprocessor.process_file(path);
                program->addShaderFromSourceCode(shaderFile->first, preprocessedSource);
            }
        }

        // bind attribute locations
        for (std::vector<AttributeLocation>::const_iterator attributeLocation
             = m_attributeLocations.begin();
             attributeLocation != m_attributeLocations.end();
             ++attributeLocation)
        {
            program->bindAttributeLocation(
                        attributeLocation->first, attributeLocation->second);
        }

        // link the program
        program->link();
    }

    // bind the program
    program->bind();

    return *program;
}

} // namespace ugl
