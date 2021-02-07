/** @file out.frag



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

/// fragment output (with possible depth peeling test)

#ifdef DEPTH_PEELING

    #ifdef MSAA

        uniform struct
        {
            sampler2DMS discardDepth;
        } depthPeeling;

        void discardMaybe()
        {
            if(texelFetch(depthPeeling.discardDepth, ivec2(gl_FragCoord.xy), gl_SampleID).r >= gl_FragCoord.z)
                discard;            
        }

    #else

        uniform struct
        {
            sampler2DRect discardDepth;
        } depthPeeling;

        void discardMaybe()
        {
            if(texelFetch(depthPeeling.discardDepth, ivec2(gl_FragCoord.xy)).r >= gl_FragCoord.z)
                discard;
        }

    #endif

#else
    void discardMaybe()
    {
    }
#endif



#ifdef DYNAMIC_FRAGMENT_BUFFER

    layout(binding = 0, r32ui) uniform uimage2D counterBuffer;
    layout(binding = 1, r32ui) uniform uimage2D offsetBuffer;

    struct Fragment
    {
        vec4 color;
        float depth;
    };

    layout(binding = 0, std430) buffer fragmentBuffer
    {
        Fragment fragments[];
    };


#ifdef DYNAMIC_FRAGMENT_BUFFER_COUNT_PASS

    void countFragmentDynamicFragmentBuffer()
    {
        imageAtomicAdd(counterBuffer, ivec2(gl_FragCoord.xy), 1);

        // Stop fragment shader execution right here
        discard;
    }

    void storeInDynamicFragmentBuffer(vec4 color)
    {
        // Fallback: User did not use countFragment() function at beginning of fragment shader.
        countFragmentDynamicFragmentBuffer();
    }

#else

    void countFragmentDynamicFragmentBuffer()
    {
    }

    void storeInDynamicFragmentBuffer(vec4 color)
    {
        uint offset = imageLoad(offsetBuffer, ivec2(gl_FragCoord.xy)).x;
        uint count = imageAtomicAdd(counterBuffer, ivec2(gl_FragCoord.xy), 1);

        uint index = offset + count;

        fragments[index].color = color;
        fragments[index].depth = gl_FragCoord.z;
    }

#endif

#else

    void countFragmentDynamicFragmentBuffer()
    {
    }

    void storeInDynamicFragmentBuffer(vec4 color)
    {
    }

#endif




// -------------------------------------------------------------------------

out vec4 fragmentColor;

void countFragment()
{
    countFragmentDynamicFragmentBuffer();
}


void outputFragment( vec4 color )
{
    // Depth peeling
    discardMaybe();

    // Dynamic fragment buffer
    storeInDynamicFragmentBuffer(color);

    // Write color
    fragmentColor = color;
}

void outputFragment( vec3 color )
{
    outputFragment( vec4( color, 1.0 ) );
}

void outputFragment( float luminance )
{
    outputFragment( vec4( luminance, luminance, luminance, 1.0 ) );
}
