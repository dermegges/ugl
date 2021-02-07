/** @file VariantProgram.hpp



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

#ifndef _VARIANTPROGRAM_HPP_
#define _VARIANTPROGRAM_HPP_

#include "ShaderProgram.hpp"
#include "ShaderType.hpp"
#include "Utils.hpp"
#include "FileSystemWatcher.hpp"

#include <GL/glew.h>

#include <boost/variant.hpp>

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>

namespace ugl
{

/**
 * A GLSL program which can be modified by preprocessor defines.
 *
 * Each unique combination of preprocessor defines is compiled into a new
 * ShaderProgram. Once compiled they are cached and directly returned on the
 * next bind() call without recompilation.
 */
class VariantProgram : public FileSystemWatcher::Listener
{
public:
    typedef std::map<std::string, boost::variant<long, std::string> > DefineMap;

    VariantProgram();
    virtual ~VariantProgram();
    void addImportPath(const std::string& path);
    void addShaderFromSourceFile(ShaderType type, const std::string& fileName);
    void addAttributeLocation(const std::string& name, GLuint location);
    GLuint getUnusedAttributeLocation() const;
    ShaderProgram& bind(const DefineMap& defineMap);
    ShaderProgram& bind();
    void clearCache();

public:
    void fileEvent(const std::string& path);

private:
    std::string searchImports(const std::string& path);

private:
    typedef std::pair<ShaderType, std::string> ShaderFile;
    typedef std::pair<std::string, GLuint> AttributeLocation;
    typedef std::map<DefineMap, ShaderProgram*> CompiledProgramMap;

    std::vector<std::string>       m_importPaths;
    std::vector<ShaderFile>        m_shaderFiles;
    std::vector<AttributeLocation> m_attributeLocations;
    CompiledProgramMap             m_compiledProgramMap;
};





} // namespace ugl
#endif // _VARIANTPROGRAM_HPP_
