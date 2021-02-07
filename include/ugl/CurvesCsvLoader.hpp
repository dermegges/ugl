/** @file CurvesCsvLoader.hpp



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

#ifndef _CURVESCSVLOADER_HPP_
#define _CURVESCSVLOADER_HPP_

#include "CurvesData.hpp"
#include "ScalarData.hpp"

#include <string>

namespace ugl
{

/**
 * Loads curves from a file in a CSV-format used as an example for Ivan Kostov's
 * bachelor thesis.
 *
 * The CSV-format is very simple - each curve is one ASCII file. The first line
 * gives the number of points for this curve. After that, each point is given by
 * its time, x-, y- and z-coordinate in one line.
 *
 * Provides CurvesData and ScalarData.
 */
class CurvesCsvLoader
{
public:
    void addCurve(const std::string& filename);
    CurvesData& getCurvesData();
    ScalarData& getScalarData();

private:
    CurvesData m_curvesData;
    ScalarData m_scalarData;
};

// -------------------------------------------------------------------------

inline CurvesData& CurvesCsvLoader::getCurvesData()
{
    return m_curvesData;
}

// -------------------------------------------------------------------------

inline ScalarData& CurvesCsvLoader::getScalarData()
{
    return m_scalarData;
}

} // namespace ugl
#endif // _CURVESCSVLOADER_HPP_
