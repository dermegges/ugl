/** @file redTriangle.glsl



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

    Author(s): T.Biedert
*/

// -------------------------------------------------------------------------
#common
// -------------------------------------------------------------------------

#version 420 core

#import <ugl/globals.glsl>



// -------------------------------------------------------------------------
#vertex
// -------------------------------------------------------------------------

in vec3 vertexPosition;

void main( void )
{
    vec4 worldPos = globals.modelviewMatrix * vec4( vertexPosition, 1.0 );
    gl_Position   = globals.projectionMatrix * worldPos;
}



// -------------------------------------------------------------------------
#fragment
// -------------------------------------------------------------------------

layout(location=0) out vec4 texture1;
layout(location=1) out vec4 texture2;

void main( void )
{
    texture1 = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    texture2 = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
