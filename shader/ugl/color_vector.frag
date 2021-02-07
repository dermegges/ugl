/** @file color_vector.frag



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

#import <ugl/hsv2rgb.glsl>

in vec3 fragVector;

vec3 getColor()
{
    float l = length( fragVector );
    l = log( 1.0 + 9.0*l );
    vec3 hsv = vec3( l, 0.5, 1.0 );
    return hsv2rgb( hsv );
}
