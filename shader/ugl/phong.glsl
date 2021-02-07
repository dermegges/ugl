/** @file phong.glsl



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

vec4 phong( in vec3 N, in vec3 P, in vec4 color )
{
    vec3 L = normalize( -globals.light[0].direction );

    vec3 E = normalize( -P );
    vec3 R = normalize( reflect( -L, N ) );

    float NL = dot( N, L );

    float ka = 0.2;
    float kd = 0.8 * max( 0.0, NL );
    float ks = 0.0;

    if( NL>0 )
    {
        float RE = max( dot( R, E ), 0.0 );
        ks = 0.5 * pow( RE, globals.light[0].glossiness );
    }

    return clamp( vec4( (ka+kd)*color.rgb + ks, color.a ), 0.0, 1.0 );
}
