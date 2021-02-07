/** @file UniformSet.cpp



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

#include "ugl/UniformSet.hpp"
#include "ugl/ErrorCheck.hpp"

#include <iostream>

namespace ugl
{

// -------------------------------------------------------------------------

size_t uniformTypeSize( GLenum type )
{
    switch( type )
    {
    case GL_FLOAT:              return 1*sizeof(GLfloat);
    case GL_FLOAT_VEC2:         return 2*sizeof(GLfloat);
    case GL_FLOAT_VEC3:         return 3*sizeof(GLfloat);
    case GL_FLOAT_VEC4:         return 4*sizeof(GLfloat);
    case GL_DOUBLE:             return 1*sizeof(GLdouble);
    case GL_DOUBLE_VEC2:        return 2*sizeof(GLdouble);
    case GL_DOUBLE_VEC3:        return 3*sizeof(GLdouble);
    case GL_DOUBLE_VEC4:        return 4*sizeof(GLdouble);
    case GL_INT:                return 1*sizeof(GLint);
    case GL_INT_VEC2:           return 2*sizeof(GLint);
    case GL_INT_VEC3:           return 3*sizeof(GLint);
    case GL_INT_VEC4:           return 4*sizeof(GLint);
    case GL_UNSIGNED_INT:       return 1*sizeof(GLuint);
    case GL_UNSIGNED_INT_VEC2:  return 2*sizeof(GLuint);
    case GL_UNSIGNED_INT_VEC3:  return 3*sizeof(GLuint);
    case GL_UNSIGNED_INT_VEC4:  return 4*sizeof(GLuint);
    case GL_BOOL:               return 1*sizeof(GLuint);
    case GL_BOOL_VEC2:          return 2*sizeof(GLuint);
    case GL_BOOL_VEC3:          return 3*sizeof(GLuint);
    case GL_BOOL_VEC4:          return 4*sizeof(GLuint);
    case GL_FLOAT_MAT2:         return  4*sizeof(GLfloat);
    case GL_FLOAT_MAT3:         return  9*sizeof(GLfloat);
    case GL_FLOAT_MAT4:         return 16*sizeof(GLfloat);
    case GL_FLOAT_MAT2x3:       return  6*sizeof(GLfloat);
    case GL_FLOAT_MAT3x2:       return  6*sizeof(GLfloat);
    case GL_FLOAT_MAT2x4:       return  8*sizeof(GLfloat);
    case GL_FLOAT_MAT4x2:       return  8*sizeof(GLfloat);
    case GL_FLOAT_MAT3x4:       return 12*sizeof(GLfloat);
    case GL_FLOAT_MAT4x3:       return 12*sizeof(GLfloat);
    case GL_DOUBLE_MAT2:        return  4*sizeof(GLdouble);
    case GL_DOUBLE_MAT3:        return  9*sizeof(GLdouble);
    case GL_DOUBLE_MAT4:        return 16*sizeof(GLdouble);
    case GL_DOUBLE_MAT2x3:      return  6*sizeof(GLdouble);
    case GL_DOUBLE_MAT3x2:      return  6*sizeof(GLdouble);
    case GL_DOUBLE_MAT2x4:      return  8*sizeof(GLdouble);
    case GL_DOUBLE_MAT4x2:      return  8*sizeof(GLdouble);
    case GL_DOUBLE_MAT3x4:      return 12*sizeof(GLdouble);
    case GL_DOUBLE_MAT4x3:      return 12*sizeof(GLdouble);
    case GL_TEXTURE_1D:
    case GL_TEXTURE_2D:
    case GL_TEXTURE_3D:
    case GL_TEXTURE_1D_ARRAY:
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_RECTANGLE:
    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_BUFFER:
    case GL_TEXTURE_2D_MULTISAMPLE:
    case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
        return sizeof(GLuint);
    default:
        return 0;
    }
}

// -------------------------------------------------------------------------

void applyUniform( GLint location, GLenum type, GLsizei size, GLvoid* ptr,
                   GLint& textureUnit )
{
    switch( type )
    {
    case GL_FLOAT:              glUniform1fv( location, size, (GLfloat*)ptr ); break;
    case GL_FLOAT_VEC2:         glUniform2fv( location, size, (GLfloat*)ptr ); break;
    case GL_FLOAT_VEC3:         glUniform3fv( location, size, (GLfloat*)ptr ); break;
    case GL_FLOAT_VEC4:         glUniform4fv( location, size, (GLfloat*)ptr ); break;
    case GL_DOUBLE:             glUniform1dv( location, size, (GLdouble*)ptr ); break;
    case GL_DOUBLE_VEC2:        glUniform2dv( location, size, (GLdouble*)ptr ); break;
    case GL_DOUBLE_VEC3:        glUniform3dv( location, size, (GLdouble*)ptr ); break;
    case GL_DOUBLE_VEC4:        glUniform4dv( location, size, (GLdouble*)ptr ); break;
    case GL_INT:                glUniform1iv( location, size, (GLint*)ptr ); break;
    case GL_INT_VEC2:           glUniform2iv( location, size, (GLint*)ptr ); break;
    case GL_INT_VEC3:           glUniform3iv( location, size, (GLint*)ptr ); break;
    case GL_INT_VEC4:           glUniform4iv( location, size, (GLint*)ptr ); break;
    case GL_UNSIGNED_INT:       glUniform1uiv( location, size, (GLuint*)ptr ); break;
    case GL_UNSIGNED_INT_VEC2:  glUniform2uiv( location, size, (GLuint*)ptr ); break;
    case GL_UNSIGNED_INT_VEC3:  glUniform3uiv( location, size, (GLuint*)ptr ); break;
    case GL_UNSIGNED_INT_VEC4:  glUniform4uiv( location, size, (GLuint*)ptr ); break;
    case GL_BOOL:               glUniform1uiv( location, size, (GLuint*)ptr ); break;
    case GL_BOOL_VEC2:          glUniform2uiv( location, size, (GLuint*)ptr ); break;
    case GL_BOOL_VEC3:          glUniform3uiv( location, size, (GLuint*)ptr ); break;
    case GL_BOOL_VEC4:          glUniform4uiv( location, size, (GLuint*)ptr ); break;
    case GL_FLOAT_MAT2:         glUniformMatrix2fv( location, size, GL_FALSE, (GLfloat*)ptr ); break;
    case GL_FLOAT_MAT3:         glUniformMatrix3fv( location, size, GL_FALSE, (GLfloat*)ptr ); break;
    case GL_FLOAT_MAT4:         glUniformMatrix4fv( location, size, GL_FALSE, (GLfloat*)ptr ); break;
        //    case GL_FLOAT_MAT2x3:       return  6*sizeof(GLfloat);
        //    case GL_FLOAT_MAT3x2:       return  6*sizeof(GLfloat);
        //    case GL_FLOAT_MAT2x4:       return  8*sizeof(GLfloat);
        //    case GL_FLOAT_MAT4x2:       return  8*sizeof(GLfloat);
        //    case GL_FLOAT_MAT3x4:       return 12*sizeof(GLfloat);
        //    case GL_FLOAT_MAT4x3:       return 12*sizeof(GLfloat);
        //    case GL_DOUBLE_MAT2:        return  4*sizeof(GLdouble);
        //    case GL_DOUBLE_MAT3:        return  9*sizeof(GLdouble);
        //    case GL_DOUBLE_MAT4:        return 16*sizeof(GLdouble);
        //    case GL_DOUBLE_MAT2x3:      return  6*sizeof(GLdouble);
        //    case GL_DOUBLE_MAT3x2:      return  6*sizeof(GLdouble);
        //    case GL_DOUBLE_MAT2x4:      return  8*sizeof(GLdouble);
        //    case GL_DOUBLE_MAT4x2:      return  8*sizeof(GLdouble);
        //    case GL_DOUBLE_MAT3x4:      return 12*sizeof(GLdouble);
        //    case GL_DOUBLE_MAT4x3:      return 12*sizeof(GLdouble);
    case GL_TEXTURE_1D:
    case GL_TEXTURE_2D:
    case GL_TEXTURE_3D:
    case GL_TEXTURE_1D_ARRAY:
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_RECTANGLE:
    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_BUFFER:
    case GL_TEXTURE_2D_MULTISAMPLE:
    case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
        glUniform1i( location, textureUnit );
        glActiveTexture( GL_TEXTURE0 + textureUnit );
        glBindTexture( type, *( (GLuint*)ptr ) );
        ++textureUnit;
        break;
    default:
        std::cerr << "uniform type not implemented" << std::endl;
        break;
    }
}

// -------------------------------------------------------------------------

void UniformSet::apply( GLuint program ) const
{
    GLint nuniform;
    glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &nuniform );

    GLint maxlen;
    glGetProgramiv( program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxlen );

    GLchar* name = new GLchar[maxlen];

    GLint textureUnit = 0;

    for( int index=0; index<nuniform; ++index )
    {
        GLint  size;
        GLenum type;

        glGetActiveUniform( program, index, maxlen, NULL,
                            &size,
                            &type,
                            name );

        const uniform_wrapper* w = AbstractValueSet::get(name);

        if (!w)
            continue;

        if( type == GL_DOUBLE )
            std::cerr << *((GLdouble*)w->data) << std::endl;

        GLuint location = glGetUniformLocation( program, name );
        applyUniform( location, w->type, w->size, w->data, textureUnit );
        checkGLErrorLabel("Set uniform " + std::string(name));
    }

    delete[] name;

	this->applyImageTextures();
}


void UniformSet::applyImageTextures() const
{
	// Apply parent image textures first
	if (this->m_parent)
		((UniformSet*) this->m_parent)->applyImageTextures();

	for (auto& it : this->imageTextures)
		glBindImageTexture(it.second.unit, it.second.texture, it.second.level, it.second.layered, it.second.layer, it.second.access, it.second.format);

}

// -------------------------------------------------------------------------

void UniformSet::set( const std::string& name,
                      GLenum type, GLsizei size, const GLvoid *data )
{
    const size_t bytes = size * uniformTypeSize(type);

    named_value_map::iterator ui = m_values.find( name );
    uniform_wrapper* w;

    if( ui == m_values.end() )
    {
        ui = m_values.insert( make_pair(name, uniform_wrapper()) ).first;
        w = &(ui->second);
        w->type = type;
        w->size = size;
        w->data = new unsigned char[bytes];
    }
    else
    {
        w = &(ui->second);

        if( bytes != w->size * uniformTypeSize(w->type) )
        {
            delete[] w->data;
            w->data = new unsigned char[bytes];
        }

        w->type = type;
        w->size = size;
    }

    if( type != GL_BOOL )
        memcpy( (void*)w->data, data, bytes );
    else
        // cast boolean input to unsigned int
        *((GLuint*) w->data) = *((GLboolean*) data);

    // vector<bool> not supported
}

// -------------------------------------------------------------------------

void UniformSet::get( const std::string& name, GLenum type, GLsizei size, GLvoid* data )
{
    const uniform_wrapper* w = AbstractValueSet::get(name);

    if (w)
    {
        const size_t bytes = size * uniformTypeSize(type);
        memcpy(data, w->data, bytes);
    }
}



void UniformSet::setImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
	this->imageTextures[unit] = ImageTexture {unit, texture, level, layered, layer, access, format };
}


} // namespace ugl
