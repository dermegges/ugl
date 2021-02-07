/** @file dfbClear.glsl



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

//
// Clears the counter buffer of the dynamic fragment buffer. 
//


// -------------------------------------------------------------------------
#common
// -------------------------------------------------------------------------

#version 430 core


// -------------------------------------------------------------------------
#vertex
// -------------------------------------------------------------------------

const vec2 vertex[4] = vec2[]
(                                                                   
    vec2(-1.0,  1.0), vec2(-1.0, -1.0),                              
    vec2( 1.0,  1.0), vec2( 1.0, -1.0)                               
);                                                                  

void main()                                                         
{                                                                   
    gl_Position = vec4( vertex[gl_VertexID], 0.0, 1.0 );            
}                                                                   


// -------------------------------------------------------------------------
#fragment
// -------------------------------------------------------------------------

layout(binding = 0, r32ui) uniform writeonly uimage2D counterBuffer;

void main()
{
    imageStore(counterBuffer, ivec2(gl_FragCoord.xy), uvec4(0));
}