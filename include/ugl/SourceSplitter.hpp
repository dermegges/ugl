/** @file SourceSplitter.hpp



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

#ifndef _SOURCESPLITTER_HPP_
#define _SOURCESPLITTER_HPP_

#include "ShaderType.hpp"

#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace ugl
{

/**
 * Loads a GLSL source file, splits it into several shader sources depending on
 * preprocessor statements and returns them as strings.
 *
 * The preprocessor statements are
 *
 *   * `#vertex` - the following code will be included in the vertex shader only
 *   * `#fragment` - the following code will be included in the fragment shader only
 *   * `#geometry` - the following code will be included in the geometry shader only
 *   * `#common` - the following code will be included in all shader stages
 *
 * Additionally, there is the `#varying` statement which will be replaced by
 * `out` for vertex shaders and `in` for other shaders - it can be used to
 * declare the values which will be passed from vertex to fragment shader in the
 * `#common` section.
 */
class SourceSplitter
{
public:
    typedef std::map<ShaderType, std::string> SourceStrings;

    SourceStrings processFile(const std::string& filename);

private:
    static const std::string WHITESPACE;
    std::map<ShaderType, std::stringstream> m_sourceStreams;
    std::stringstream m_common;
    std::vector<std::ostream*> m_activeStreams;
    bool m_emitLine;

    static bool isDirective(const std::string& line,
            const std::string& directive);
    static void replaceDirective(std::string& line,
            const std::string& directive, const std::string& replacement);
    void switchShaderType(ShaderType shaderType);
};

// ------------------------------------------------------------------------

} // namespace ugl
#endif // _SOURCESPLITTER_HPP_
