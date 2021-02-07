/** @file surface.glsl



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

#ifdef DYNAMIC_FRAGMENT_BUFFER
#version 430 core
#else
#version 400 core
#endif

#import <ugl/globals.glsl>

// -------------------------------------------------------------------------
#vertex
// -------------------------------------------------------------------------

#ifdef LINE_MODE
    #import <ugl/normal_none.vert>
    #import <ugl/color_white.vert>
#else
    // compute further attributes only if not drawing in line mode
    #ifdef USE_VERTEX_NORMAL
        // compute transformed vertex normal only when needed
        #import <ugl/normal_vertex.vert>
    #else
        #import <ugl/normal_face.vert>
    #endif

    #ifdef SHOW_VECTORS
        #import <ugl/color_vector.vert>
    #else
        #import <ugl/color_default.vert>
    #endif
#endif

in vec3 vertexPosition;

void main( void )
{
    vec4 worldPos = globals.modelviewMatrix * vec4( vertexPosition, 1.0 );
    gl_Position   = globals.projectionMatrix * worldPos;

    computeForNormal( worldPos );
    computeForColor();
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
    #ifdef USE_VERTEX_NORMAL
        #import <ugl/normal_vertex.frag>
    #else
        #import <ugl/normal_face.frag>
    #endif

    #ifdef SHOW_VECTORS
        #import <ugl/color_vector.frag>
    #else
        #import <ugl/color_default.frag>
    #endif
#endif

uniform float alpha;

void main( void )
{
    countFragment();

    vec4 color = vec4(getColor(), alpha);
    color = illuminate(color);
    outputFragment(color);
}
