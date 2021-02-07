/** @file testureDisplay.glsl



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

const vec2 vertex[4] = vec2[]
(
    vec2(-1.0,  1.0), vec2(-1.0, -1.0),
    vec2( 1.0,  1.0), vec2( 1.0, -1.0)
);

void main()
{
    gl_Position = vec4(vertex[gl_VertexID], 0.0, 1.0);
}


// -------------------------------------------------------------------------
#fragment
// -------------------------------------------------------------------------

uniform sampler2DRect colorTexture;     // use sampler2DMS when multisampling  (see blend.glsl for example)
//uniform sampler2DRect colorTextureB;
uniform sampler2DRect depthTexture;

#import <ugl/out.frag>

void main()
{
	vec4 fragColor = texture(colorTexture, gl_FragCoord.xy); 
	float fragDepth = texture(depthTexture, gl_FragCoord.xy).r;

	// Only consider texels where fragments have been written to
	if (fragDepth < 1.0)
	{
		// Do some fancy stuff in image-space
		fragColor.g = abs(sin(0.1 * gl_FragCoord.x));
		fragColor.r -= fragColor.g;

		outputFragment(fragColor);

		gl_FragDepth = fragDepth;
	}
}
