/** @file blend.glsl



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
// Shaders for blending phase in depth peeling version of transparent render stage
//


// -------------------------------------------------------------------------
#common
// -------------------------------------------------------------------------

#version 400 core

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
    gl_Position = vec4( vertex[gl_VertexID], 0.0, 1.0 );            
}                                                                   


// -------------------------------------------------------------------------
#fragment
// -------------------------------------------------------------------------

#ifdef MSAA
    uniform sampler2DMS   previousTarget;                                     
    uniform sampler2DMS   colorTex;                                     
#else
    uniform sampler2DRect previousTarget;
    uniform sampler2DRect colorTex;
#endif

uniform bool          firstPass;   
uniform bool          lastPass;                                     
uniform vec4          viewport;                                     
uniform vec4          backgroundColor;                              
uniform bool          checkerboard;                                 


#ifdef VOLUMERENDERING
uniform struct
{
#ifdef MSAA
    sampler2DMS   currentDepthTex;                                     
    sampler2DMS   previousDepthTex;                                     
#else
    sampler2DRect currentDepthTex;
    sampler2DRect previousDepthTex;
#endif

    vec2 depthRange;

    vec3 boxMin;
    vec3 boxMax;
    float sampleStepSize;
    bool hasMask;
    sampler3D intensity;
    ivec3 intensityResolution;
    sampler3D mask;
    ivec3 maskResolution;
    int numTransfer;
    sampler1DArray transfer;  
    int transferResolution;  
    vec4 alpha;
} volumeRendering;
#endif


out vec4 fragColor;                                                 


#ifdef VOLUMERENDERING
vec4 calcEyeFromWindow(in vec3 windowCoord)
{
    vec3 ndcp ;
    ndcp .xy = ((2.0 * windowCoord.xy) - (2.0 * viewport.xy)) / (viewport.zw) - 1;
    ndcp .z = (2.0 * windowCoord.z - volumeRendering.depthRange.x - volumeRendering.depthRange.y) / (volumeRendering.depthRange.y - volumeRendering.depthRange.x);

    vec4 clipp ;
    clipp .w = globals.projectionMatrix[3][2] / (ndcp .z - (globals.projectionMatrix[2][2] / globals.projectionMatrix[2][3]));
    clipp .xyz = ndcp  * clipp .w;
 
    vec4 eyep  = globals.projectionMatrixInverse * clipp ;
    return eyep ;
}
#endif


void blendUnder(vec4 color)
{
    // Front-to-back blending
    // Cdst = Adst (Asrc Csrc) + Cdst
    // Adst = (1-Asrc) Adst 

    fragColor.rgb = fragColor.a * (color.a * color.rgb) + fragColor.rgb;
    fragColor.a = (1.0 - color.a) * fragColor.a;
}


vec3 safeNormalize(vec3 v)
{
    float l = length(v);

    if(l > 0.0)
        v /= l;

    return v;
}


void main()                                                         
{          
    /*
     * Initialize destination color from previous render target
     */
 #ifdef MSAA
    fragColor = texelFetch(previousTarget, ivec2(gl_FragCoord.xy - viewport.xy), gl_SampleID);     
#else
    fragColor = texture(previousTarget, gl_FragCoord.xy - viewport.xy);                                        
#endif



    /*
     * Volume rendering
     */
#ifdef VOLUMERENDERING
    //  
    // Current: the new fragment to be blended under the previous (closer) one
    //

#ifdef MSAA
    float currentDepth = texelFetch(volumeRendering.currentDepthTex, ivec2(gl_FragCoord.xy - viewport.xy), gl_SampleID).r;     
    float previousDepth = texelFetch(volumeRendering.previousDepthTex, ivec2(gl_FragCoord.xy - viewport.xy), gl_SampleID).r;     
#else
    float currentDepth = texture(volumeRendering.currentDepthTex, gl_FragCoord.xy - viewport.xy).r;
    float previousDepth = texture(volumeRendering.previousDepthTex, gl_FragCoord.xy - viewport.xy).r;   
#endif
    
    if (currentDepth < 1 && previousDepth < 1 && !firstPass)
    {            
        // World coordinates
        vec4 previousPosEye = calcEyeFromWindow(vec3(gl_FragCoord.xy - viewport.xy, previousDepth));
        vec4 currentPosEye = calcEyeFromWindow(vec3(gl_FragCoord.xy - viewport.xy, currentDepth));    
        vec4 previousPos = globals.modelviewMatrixInverse * previousPosEye;
        vec4 currentPos = globals.modelviewMatrixInverse * currentPosEye;
        float maxDistance = distance(previousPos, currentPos);

        // Local "relative" coordinates within volume cube (0-1 range)
        vec3 start = (previousPos.xyz - volumeRendering.boxMin) / (volumeRendering.boxMax - volumeRendering.boxMin);
        vec3 end = (currentPos.xyz - volumeRendering.boxMin) / (volumeRendering.boxMax - volumeRendering.boxMin);
        
        // Sampling
        int n = 100000;
        float accumulatedDistance = 0.0;

        for (int i = 0; i < n; ++i)
        {
            float s = accumulatedDistance / maxDistance;
            vec3 p = mix(start, end, s);
        
            for (int t = 0; t < volumeRendering.numTransfer; ++t)
            {
                bool skipSample = false;

                if (volumeRendering.hasMask)
                {
                    float mask = texture(volumeRendering.mask, p)[t];
                    skipSample = (mask < 0.5);
                }

                if (!skipSample)
                {            
                    float intensity = texture(volumeRendering.intensity, p)[t];
                    vec4 colorSample = texture(volumeRendering.transfer, vec2(intensity, float(t)));
                    colorSample.a *= volumeRendering.alpha[t];

                    // Lighting
                    vec3 intensitySpacing = vec3(1) / volumeRendering.intensityResolution;
                    float xp = texture(volumeRendering.intensity, p + vec3(intensitySpacing.x, 0, 0)).r;
                    float xm = texture(volumeRendering.intensity, p - vec3(intensitySpacing.x, 0, 0)).r;
                    float yp = texture(volumeRendering.intensity, p + vec3(0, intensitySpacing.y, 0)).r;
                    float ym = texture(volumeRendering.intensity, p - vec3(0, intensitySpacing.y, 0)).r;
                    float zp = texture(volumeRendering.intensity, p + vec3(0, 0, intensitySpacing.z)).r;
                    float zm = texture(volumeRendering.intensity, p - vec3(0, 0, intensitySpacing.z)).r;
                
                    vec3 normal = safeNormalize(globals.normalMatrix * vec3(xm - xp, ym - yp, xm - zp));

                    vec3 view = -mix(previousPosEye.xyz, currentPosEye.xyz, s);
                    vec3 light = normalize(-globals.light[0].direction);
                    vec3 refl = normalize(reflect(view, normal));

                    float kd = max(dot(normal, light), 0.0);
                    float ks = pow(max(dot(refl, light), 0.0), globals.light[0].glossiness);            
                    colorSample.rgb = (0.6 + 0.3 * kd) * colorSample.rgb + 0.1 * ks;

                    // Light transport integral
                    colorSample.a *= volumeRendering.sampleStepSize;
                    colorSample.a = min(1.0, colorSample.a);                    

                    blendUnder(colorSample);
                }
            }

            accumulatedDistance += volumeRendering.sampleStepSize;

            if (accumulatedDistance > maxDistance)
                break;
        }                
    }
#endif


    /*
     * Depth peeling
     */ 
#ifdef MSAA
    vec4 peelSurfaceColor = texelFetch(colorTex, ivec2(gl_FragCoord.xy - viewport.xy), gl_SampleID);     
#else
    vec4 peelSurfaceColor = texture(colorTex, gl_FragCoord.xy - viewport.xy);                                        
#endif
   
    blendUnder(peelSurfaceColor);


    /*
     * Background color
     */
    if(lastPass)                                                  
    {                                                               
        vec3 bg = backgroundColor.rgb;                              
        
        if (checkerboard) 
        { 
            int ix = int(gl_FragCoord.x/16.0);                          
            int iy = int(gl_FragCoord.y/16.0);                          
            int ti = (ix + iy) % 2;                                     
            
            bg *= (ti == 0) ? 0.5 : 0.52;                          
        } 
        
        blendUnder(vec4(bg, backgroundColor.a));

        // In last pass, set correct alpha value for frame buffer
        fragColor.a = 1.0 - fragColor.a;
    }                                                                  
}       






