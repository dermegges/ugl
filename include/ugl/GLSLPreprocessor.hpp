/** @file GLSLPreprocessor.hpp



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

#ifndef __GLSLPreprocessor_hpp
#define __GLSLPreprocessor_hpp

#include <list>
#include <string>
#include <vector>
#include <map>

namespace ugl
{

/**
 * Preprocessor for the GLSL - supports most preprocessor defines known from C.
 */
class GLSLPreprocessor
{
    enum TokenType
    {
        T_EOI,          ///< End of input stream
        T_ERROR,        ///< An error has been encountered
        T_WHITESPACE,   ///< A whitespace span (but not newline)
        T_NEWLINE,      ///< A single newline (CR & LF)
        T_CONTLINE,     ///< Line continuation ('\' followed by LF)
        T_NUMBER,       ///< A number
        T_KEYWORD,      ///< A keyword
        T_OPERATOR,     ///< An operator
        T_DIRECTIVE,    ///< A preprocessor directive
        T_STRING,       ///< A string
        T_COMMENT,      ///< A block comment
        T_LINECOMMENT,  ///< A line comment
        T_TEXT          ///< Unparsed text (cannot be returned from get_next_token())
    };

    /// the core token class
    /// FIXME: replace std::string by an alternative with better performance
    class token : public std::string
    {

    public:

        token() {}
        token( TokenType t ) : type(t) {}
        token( TokenType t, const std::string& s ) : std::string(s), type(t) {}

        token( TokenType t,
               std::string::const_iterator begin,
               std::string::const_iterator end ) :
               std::string(begin, end), type(t)
        {
        }

        token( const std::string& s ) : std::string(s), type(T_TEXT)
        {
        }

        token( long value ) : type(T_NUMBER)
        {
            set_value(value);
        }

        bool get_value( long &value ) const;
        void set_value( long value );

        bool operator==( const TokenType& otherType ) const
        {
            return type == otherType;
        }

        bool operator!=( const TokenType& otherType ) const
        {
            return type != otherType;
        }

        TokenType type;
    };

    // ----------------------------------------

    struct tokenizer
    {
        std::string::const_iterator cur;
        std::string::const_iterator end;
        size_t line;
        size_t sstr;
        bool   sol;

        tokenizer() {}

        tokenizer( std::string::const_iterator c, std::string::const_iterator e, size_t l, size_t s ) :
            cur(c), end(e), line(l), sstr(s), sol(true)
        {
        }

        tokenizer( const token& t, tokenizer& base ) :
            cur(t.begin()), end(t.end()), line(base.line), sstr(base.sstr), sol(true)
        {
        }
    };

    // ----------------------------------------

    typedef std::vector<token> token_list;

    /// macro datatype
    class macro
    {
    public:

        typedef token (*expand_func)( GLSLPreprocessor&, tokenizer& );

        bool         expanding;
        bool         persistent;
        token        value;
        token_list   args;
        expand_func  func;

        macro() : expanding(false), func(0)
        {
        }

        macro( expand_func f ) : expanding(false), func(f)
        {
        }

        macro( const token& v ) : expanding(false), value(v), func(0)
        {
        }
    };

    token get_next_token( tokenizer& state, bool expand );
    token get_next_nonwhitespace_token( tokenizer& state, bool expand );

    token parse( tokenizer &state );

    token handle_directive( tokenizer& ts, const token &t );

    bool  handle_define( tokenizer& state );
    bool  handle_undef( tokenizer& state );

    bool  handle_ifdef( tokenizer& state );
    bool  handle_if( tokenizer& state );
    bool  handle_elif( tokenizer& state );
    bool  handle_else( tokenizer& state );
    bool  handle_endif( tokenizer& state );

    bool  handle_version( tokenizer& state );
    bool  handle_extension( tokenizer& state );
    token handle_import( tokenizer& state );
    bool  handle_line( tokenizer& state );

    token expand_macro( tokenizer& state, const token &itoken );
    bool  parse_macro_arguments( tokenizer& state, token_list& args, bool expand, bool skipws );

    token evaluate_expression( tokenizer& state, token &result, int priority = 0 );
    bool  evaluate_condition( tokenizer& state );

    bool  parse_value( tokenizer& state, const token &t, long &value );

    void  error( tokenizer& state, const char *error, const token *error_token = 0 );
    void  warning( tokenizer& state, const char *error, const token *error_token = 0 );

    /// expansion functions for built-in macros
    static token expand_defined( GLSLPreprocessor& pp, tokenizer& state );
    static token expand_version( GLSLPreprocessor& pp, tokenizer& state );
    static token expand_file( GLSLPreprocessor& pp, tokenizer& state );
    static token expand_line( GLSLPreprocessor& pp, tokenizer& state );

    static void print_token( const token& t );

    std::string process( tokenizer& state );

    token read_file( const std::string& filename ) const;

public:

    void add_import_path( const std::string& path );

    void define( const std::string& name, const std::string& value );
    void define( const std::string& name, long value );

    bool undefine( const std::string& name );

    std::string process( const std::string& source, const std::string* name = 0 );
    std::string process_file( const std::string& filename );

protected:

    struct macro_map : public std::multimap<std::string,macro>
    {
        macro* get( const std::string& name )
        {
            iterator mi = find( name );
            return mi == end() ? 0 : &mi->second;
        }
    };

    /// conditional stack: lowest bit is true if current region active
    unsigned int                m_enabled;
    unsigned int                m_prevcnd;
    unsigned int                m_prevstm;

    /// should emit #line directive on next output?
    bool                        m_emitline;

    /// list of macros
    macro_map                   m_macros;

    /// version information
    unsigned int                m_versionnumber;
    token                       m_versionprofile;

    /// extensions information
    std::map<token,token>       m_extensions;

    /// source strings
    std::vector<std::string>    m_srcstrings;

    /// import paths
    std::vector<std::string>    m_importpaths;
};

} // namespace ugl
#endif // __GLSLPreprocessor_hpp
