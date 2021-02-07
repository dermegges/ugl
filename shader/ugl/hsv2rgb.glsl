/** @file hsv2rgb.glsl



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

vec3 hsv2rgb( in vec3 hsv )
{
    vec3 rgb = vec3(hsv.z);

    if( hsv.y != 0.0 )
    {
        float var_h = hsv.x * 6.0;
        float var_i = floor( var_h );
        float var_1 = hsv.z * (1.0 - hsv.y);
        float var_2 = hsv.z * (1.0 - hsv.y * (var_h-var_i));
        float var_3 = hsv.z * (1.0 - hsv.y * (1.0 - (var_h-var_i)));

        switch( int(var_i) )
        {
            case  0: rgb = vec3(hsv.z, var_3, var_1); break;
            case  1: rgb = vec3(var_2, hsv.z, var_1); break;
            case  2: rgb = vec3(var_1, hsv.z, var_3); break;
            case  3: rgb = vec3(var_1, var_2, hsv.z); break;
            case  4: rgb = vec3(var_3, var_1, hsv.z); break;
            default: rgb = vec3(hsv.z, var_1, var_2); break;
        }
    }

    return rgb;
}
