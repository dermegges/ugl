/** @file bg_pattern.glsl



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

    Author(s): A.Lohfink
*/

uniform bool drawBg;

//gives background pattern for DefaultRenderStage
vec4 pattern(vec2 coord)
{
  vec4 color;
  if(drawBg)
   {
      color = vec4(1.);
      int ix = int(coord.x/16.0);                          
      int iy = int(coord.y/16.0);                          
      int ti = (ix + iy) % 2;                                     
     
      color *= (ti == 0) ? 0.5 : 0.52;
   }
   else
   {
      if (coord.y < 200)
      {
        float gray = coord.y/200.+0.2;
        color = vec4(gray, gray, gray, 1.);
      }
      else
        color = vec4(1.);
   }

   return color;
}