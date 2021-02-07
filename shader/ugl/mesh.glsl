/** @file mesh.glsl



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

// -------------------------------------------------------------------------
#common
// -------------------------------------------------------------------------

#version 400 core

#import <ugl/globals.glsl>

// -------------------------------------------------------------------------
#vertex
// -------------------------------------------------------------------------

#ifdef LINE_MODE
    #import <ugl/normal_none.vert>
    #import <ugl/color_white.vert>
#else
    // compute further attributes only if not drawing in line mode
    #import <ugl/normal_vertex.vert>
    #import <ugl/color_default.vert>
#endif

in vec3 vertexPosition;

void main( void )
{
    vec4 worldPos = globals.modelviewMatrix * vec4( vertexPosition, 1.0 );
    gl_Position   = globals.projectionMatrix * worldPos;

    computeForNormal( worldPos );
}

// -------------------------------------------------------------------------
#fragment
// -------------------------------------------------------------------------

// fragment output
#import <ugl/out.frag>

#ifdef LINE_MODE
    #import <ugl/normal_none.frag>
    #import <ugl/color_white.frag>
#else
    #import <ugl/normal_vertex.frag>
    #import <ugl/color_default.frag>
#endif

uniform float alpha;

void main( void )
{
    vec4 color = vec4( getColor(), alpha );
    color = illuminate( color );
    outputFragment( color );
}
