/** @file UniformSet.hpp



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

#ifndef _UNIFORMSET_HPP_
#define _UNIFORMSET_HPP_

#include "AbstractValueSet.hpp"
#include "UniformTraits.hpp"

#include <GL/glew.h>

#include <string>
#include <map>

namespace ugl
{

// -------------------------------------------------------------------------

struct uniform_wrapper
{
    GLenum   type;
    GLsizei  size;
    GLubyte* data;
};

struct ImageTexture
{
	GLuint unit;
	GLuint texture;
	GLint level;
	GLboolean layered; 
	GLint layer;
	GLenum access;
	GLenum format;
};

// -------------------------------------------------------------------------

/**
 * A hierarchical set of OpenGL uniforms including texture references which can
 * be applied to a GLSL program.
 */
class UniformSet : public AbstractValueSet<uniform_wrapper>
{
public:
    template<typename T>
    void set( const std::string& name, const T& value );

    template<typename T>
    void get( const std::string& name, T* target );

    void setTexture( const std::string& name, GLenum target, GLuint texture );
    void setImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

    void apply( GLuint program ) const;

private:
	void applyImageTextures() const;

private:
    void set( const std::string& name, GLenum type, GLsizei size, const GLvoid *data );
    void get( const std::string& name, GLenum type, GLsizei size, GLvoid* data );

	std::map<GLuint, ImageTexture> imageTextures;
};

// -------------------------------------------------------------------------

template<typename T>
inline void UniformSet::set( const std::string& name, const T& value )
{
    set( name,
         uniform_traits<T>::type(),
         uniform_traits<T>::size(value),
         uniform_traits<T>::data(value) );
}

// -------------------------------------------------------------------------

template<typename T>
inline void UniformSet::get( const std::string& name, T* target )
{
    get( name,
         uniform_traits<T>::type(),
         uniform_traits<T>::size(*target),
         (GLvoid*) target );
}

// -------------------------------------------------------------------------

inline void UniformSet::setTexture( const std::string& name,
                                    GLenum target, GLuint texture )
{
    set( name, target, 1, &texture );
}

} // namespace ugl
#endif
