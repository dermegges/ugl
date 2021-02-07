/** @file SourceSplitter.cpp



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

#include "ugl/SourceSplitter.hpp"

#include <fstream>

namespace ugl
{

// ------------------------------------------------------------------------

SourceSplitter::SourceStrings SourceSplitter::processFile(
        const std::string& filename)
{
    m_activeStreams.clear();
    m_common.str(std::string());
    m_common.clear();
    m_sourceStreams.clear();
    m_emitLine = false;
    size_t lineNumber = 1;

    {
        std::ifstream in(filename);

        while (in.good())
        {
            std::string line;
            getline(in, line);

            if (isDirective(line, "#common"))
            {
                m_activeStreams.clear();
                m_activeStreams.push_back(&m_common);
                for (std::map<ShaderType, std::stringstream>::iterator sourceStream = m_sourceStreams.begin();
                        sourceStream != m_sourceStreams.end(); ++sourceStream)
                    m_activeStreams.push_back(&(sourceStream->second));

                m_emitLine = true;
            }
            else if (isDirective(line, "#vertex"))
            {
                switchShaderType(VERTEX);
            }
            else if (isDirective(line, "#fragment"))
            {
                switchShaderType(FRAGMENT);
            }
            else if (isDirective(line, "#geometry"))
            {
                switchShaderType(GEOMETRY);
            }
            else if (isDirective(line, "#tess_control"))
            {
                switchShaderType(TESS_CONTROL);
            }
            else if (isDirective(line, "#tess_evaluation"))
            {
                switchShaderType(TESS_EVALUATION);
            }
            else
            {
                for (std::vector<std::ostream*>::iterator stream = m_activeStreams.begin();
                        stream != m_activeStreams.end(); ++stream)
                    **stream << line << '\n';
            }

            ++lineNumber;
            if (m_emitLine)
            {
                for (std::vector<std::ostream*>::iterator stream = m_activeStreams.begin();
                        stream != m_activeStreams.end(); ++stream)
                    **stream << "#line " << lineNumber << '\n';
                m_emitLine = false;
            }
        }
    }

    SourceStrings out;
    for (std::map<ShaderType, std::stringstream>::iterator sourceStream = m_sourceStreams.begin();
            sourceStream != m_sourceStreams.end(); ++sourceStream)
    {
        std::istream& in = sourceStream->second;
        std::ostringstream stream;
        while (in.good())
        {
            std::string line;
            getline(in, line);
            replaceDirective(line, "#varying",
                             (sourceStream->first == VERTEX) ? "out" : "in");
            stream << line << '\n';
        }

        out[sourceStream->first] = stream.str();
    }
    return out;
}

// ------------------------------------------------------------------------

const std::string SourceSplitter::WHITESPACE = " \t";

// ------------------------------------------------------------------------

bool SourceSplitter::isDirective(const std::string& line,
                                 const std::string& directive)
{
    const size_t start = line.find_first_not_of(WHITESPACE);
    const size_t length = directive.length();

    return    start != std::string::npos
           && !line.compare(start, length, directive)
           && line.find_first_not_of(WHITESPACE, start + length)
                  == std::string::npos;
}

// ------------------------------------------------------------------------

void SourceSplitter::replaceDirective(std::string& line,
                                      const std::string& directive,
                                      const std::string& replacement)
{
    const size_t start = line.find_first_not_of(WHITESPACE);
    const size_t length = directive.length();

    if (   start != std::string::npos
        && !line.compare(start, length, directive)
        && (   line.length() == start + length
            || WHITESPACE.find(line[start + length]) != std::string::npos))
    {
        line.replace(start, length, replacement);
    }
}

// ------------------------------------------------------------------------

void SourceSplitter::switchShaderType(ShaderType shaderType)
{
    std::map<ShaderType, std::stringstream>::iterator it
            = m_sourceStreams.find(shaderType);
    std::ostream* stream;
    if (it != m_sourceStreams.end())
        stream = &(it->second);
    else
        stream = &(m_sourceStreams[shaderType] << m_common.str());

    m_activeStreams.clear();
    m_activeStreams.push_back(stream);

    m_emitLine = true;
}

} // namespace ugl
