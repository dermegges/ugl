/** @file dfbDisplay.glsl



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
// Sorts and displays the fragments of the dynamic fragment buffer. 
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

layout(binding = 0, r32ui) uniform uimage2D counterBuffer;
layout(binding = 1, r32ui) uniform uimage2D offsetBuffer;

uniform bool checkerboard = true;

struct Fragment
{
    vec4 color;
    float depth;
};

layout(binding = 0, std430) buffer fragmentBuffer
{
    Fragment fragments[];
};


out vec4 fragmentColor;

#import <ugl/bg_pattern.glsl>

void blendUnder(vec4 color)
{
    // Front-to-back blending
    // Cdst = Adst (Asrc Csrc) + Cdst
    // Adst = (1-Asrc) Adst 

    fragmentColor.rgb = fragmentColor.a * (color.a * color.rgb) + fragmentColor.rgb;
    fragmentColor.a = (1.0 - color.a) * fragmentColor.a;
}

void main()
{
	uint offset = imageLoad(offsetBuffer, ivec2(gl_FragCoord.xy)).x;
    uint count = imageLoad(counterBuffer, ivec2(gl_FragCoord.xy)).x;

    // Insertion sort fragments, increasing depth
    for (uint i = 0; i < count; ++i)
    {
        uint j = i;
        
        while (j > 0 && fragments[offset + j].depth < fragments[offset + j - 1].depth)
        {
              Fragment temp = fragments[offset + j];
              fragments[offset + j] = fragments[offset + j - 1];
              fragments[offset + j - 1] = temp;
              --j;
        }
    }

	// Blend values
	fragmentColor = vec4(0.0, 0.0, 0.0, 1.0);

	for (uint i = 0; i < count; ++i)
		blendUnder(fragments[offset + i].color);


	// Background 
	vec4 backgroundColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 bg = backgroundColor.rgb;                              
        
    if (checkerboard) 
        bg = pattern(gl_FragCoord.xy).xyz;
        
    blendUnder(vec4(bg, 1.0));
                             
    // In last pass, set correct alpha value for frame buffer
    fragmentColor.a = 1.0 - fragmentColor.a;

    // Reset counter buffer
    imageStore(counterBuffer, ivec2(gl_FragCoord.xy), uvec4(0));
}