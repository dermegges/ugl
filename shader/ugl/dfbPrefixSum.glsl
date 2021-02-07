/** @file dfbPrefixSum.comp


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
// Computes the base indices for each pixel in the global fragment buffer using parallel prefix sum / scan.
//

#version 450 core

layout (local_size_x = 1024) in;

layout(binding = 0, r32ui) uniform uimage2D counterBuffer;
layout(binding = 1, r32ui) uniform uimage2D offsetBuffer;

layout(binding = 0) buffer fragmentBuffer
{
    uint numFragments;
};



shared uint temp[gl_WorkGroupSize.x * 2];   // work group size is 1024

void main()
{ 	
 	uint id = gl_LocalInvocationID.x;
    uint rd_id;
    uint wr_id;
    uint mask;

    const uint steps = uint(log2(gl_WorkGroupSize.x)) + 1;    

    ivec2 size = imageSize(counterBuffer);
    uint n = size.x * size.y;    


    uint numBlocks = uint(ceil(float(n) / float(gl_WorkGroupSize.x * 2)));


    uint offset = 0;
    for (int j = 0; j < numBlocks; ++j)
    {
	    // Read pixels from counter image and reset it
	    barrier();
	    uint i = j * 2 * gl_WorkGroupSize.x + 2 * id;
	    if (i < n)
	    {
	    	temp[id * 2] = imageLoad(counterBuffer, ivec2(mod(i, size.x), i / size.x)).x;
	    	imageStore(counterBuffer, ivec2(mod(i, size.x), i / size.x), uvec4(0));
	    }
	    else
	    {
	    	temp[id * 2] = 0;
	    }

	    i = j * 2 * gl_WorkGroupSize.x + 2 * id + 1;
	    if (i < n)
	    {
	    	temp[id * 2 + 1] = imageLoad(counterBuffer, ivec2(mod(i, size.x), i / size.x)).x;
	    	imageStore(counterBuffer, ivec2(mod(i, size.x), i / size.x), uvec4(0));
	    }
	    else
	    {
	    	temp[id * 2 + 1] = 0;
	    }


	    // Compute prefix sum
	    barrier();
	    for (uint step = 0; step < steps; step++)
	    {
	        mask = (1 << step) - 1;
	        rd_id = ((id >> step) << (step + 1)) + mask;
	        wr_id = rd_id + 1 + (id & mask);

	        temp[wr_id] += temp[rd_id];

	        barrier();
	    }


	    // Write pixels on offset image and clear counter image
	    i = j * 2 * gl_WorkGroupSize.x + 2 * id + 1;
	    if (i < n)
			imageStore(offsetBuffer, ivec2(mod(i, size.x), i / size.x), uvec4(offset + temp[id * 2]));

		i = j * 2 * gl_WorkGroupSize.x + 2 * id + 2;
		if (i < n)
			imageStore(offsetBuffer, ivec2(mod(i, size.x), i / size.x), uvec4(offset + temp[id * 2 + 1]));

		offset += temp[gl_WorkGroupSize.x * 2 - 1];
	}

	if (id == 0)
	{
		imageStore(offsetBuffer, ivec2(0), uvec4(0));
		numFragments = offset;
	}
}