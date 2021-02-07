/** @file curves.glsl



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

#import <ugl/normal_none.vert>

#ifdef SHOW_SCALARS
    #import <ugl/color_scalar.vert>
#else
    #ifdef COLOR_CURVES
        #import <ugl/color_curve.vert>
    #else
        #import <ugl/color_white.vert>
    #endif
#endif

in vec3 vertexPosition;

void main( void )
{
    vec4 worldPos = globals.modelviewMatrix * vec4( vertexPosition, 1.0 );
    gl_Position   = globals.projectionMatrix * worldPos;

    computeForColor();
}

// -------------------------------------------------------------------------
#fragment
// -------------------------------------------------------------------------

// fragment output
#import <ugl/out.frag>

#import <ugl/normal_none.frag>

#ifdef SHOW_SCALARS
    #import <ugl/color_scalar.frag>
#else
    #ifdef COLOR_CURVES
        #import <ugl/color_curve.frag>
    #else
        #import <ugl/color_white.frag>
    #endif
#endif

uniform float alpha;

void main( void )
{
    vec4 color = vec4( getColor(), alpha );
    outputFragment( color );
}
