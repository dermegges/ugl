/** @file GLSLPreprocessor.cpp



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

#include "ugl/GLSLPreprocessor.hpp"

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <boost/algorithm/string/trim.hpp>

namespace ugl
{

using namespace std;
using namespace boost;

// -------------------------------------------------------------------------

void GLSLPreprocessor::print_token( const GLSLPreprocessor::token& t )
{
    switch( t.type )
    {
    case T_EOI:         printf( "EOI\n" ); break;
    case T_ERROR:       printf( "ERROR\n" ); break;
    case T_WHITESPACE:  printf( "WHITESPACE <<%s>>\n", t.c_str() ); break;
    case T_NEWLINE:     printf( "NEWLINE\n" ); break;
    case T_CONTLINE:    printf( "CONTLINE\n" ); break;
    case T_NUMBER:      printf( "NUMBER <<%s>>\n", t.c_str() ); break;
    case T_KEYWORD:     printf( "KEYWORD <<%s>>\n", t.c_str() ); break;
    case T_OPERATOR:    printf( "OPERATOR <<%s>>\n", t.c_str() ); break;
    case T_DIRECTIVE:   printf( "DIRECTIVE <<%s>>\n", t.c_str() ); break;
    case T_STRING:      printf( "STRING <<%s>>\n", t.c_str() ); break;
    case T_COMMENT:     printf( "COMMENT\n" ); break;
    case T_LINECOMMENT: printf( "LINECOMMENT\n" ); break;
    case T_TEXT:        printf( "TEXT <<%s>>\n", t.c_str() ); break;
    default:            printf( "UNKNOWN <<%s>>\n", t.c_str() ); break;
    };
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::token::get_value( long &value ) const
{
    long val = 0;
    size_t i = 0;

    bool negative = false;

    while( isspace( (*this)[i] ) )
        i++;

    if( (*this)[i] == '-' )
    {
        negative = true;
        ++i;
    }

    while( isspace( (*this)[i] ) )
        i++;

    long base = 10;

    if( (*this)[i] == '0' )
    {
        if( length() > i+1 && (*this)[i+1] == 'x' )
            base = 16, i += 2;
        else
            base = 8;
    }

    for( ; i<length(); ++i )
    {
        long c = long( (*this)[i] );

        if( isspace(c) )
            // Possible end of number
            break;

        if( c >= 'a' && c <= 'z' )
            c -= ('a' - 'A');

        c -= '0';
        if( c < 0 )
            return false;

        if( c > 9 )
            c -= ('A' - '9' - 1);

        if( c >= base )
            return false;

        val = (val * base) + c;
    }

    // ensure that all other characters are just spaces
    for( ; i<length(); i++ )
        if( !isspace( (*this)[i] ) )
            return false;

    value = negative ? -val : val;
    return true;
}

// -------------------------------------------------------------------------

void GLSLPreprocessor::token::set_value( long value )
{
    ostringstream tmp;
    tmp << value;

    assign( tmp.str() );
    type = T_NUMBER;
}

// -------------------------------------------------------------------------

void GLSLPreprocessor::error( tokenizer& state, const char *error, const token *error_token )
{
    if( state.sstr < m_srcstrings.size() )
        std::cerr << m_srcstrings[state.sstr];
    else
        std::cerr << state.sstr;

    std::cerr << ':' << state.line << ": error: " << error;

    if( error_token )
        std::cerr << ": " << *error_token;

    std::cerr << std::endl;
}

// -------------------------------------------------------------------------

void GLSLPreprocessor::warning( tokenizer& state, const char *error, const token *error_token )
{
    if( state.sstr < m_srcstrings.size() )
        std::cerr << m_srcstrings[state.sstr];
    else
        std::cerr << state.sstr;

    std::cerr << ':' << state.line << ": warning: " << error;

    if( error_token )
        std::cerr << ": " << *error_token;

    std::cerr << std::endl;
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::get_next_token( tokenizer& state, bool expand )
{
    if( state.cur == state.end )
        return T_EOI;

    string::const_iterator begin = state.cur;

    char c = *(state.cur++);


    if( c == '\n' || (c == '\r' && *state.cur == '\n') )
    {
        state.sol = true;
        state.line++;

        if( c == '\r' )
            ++state.cur;

        return token( T_NEWLINE, begin, state.cur );
    }
    else if( isspace(c) )
    {
        while( state.cur != state.end && *state.cur != '\r' && *state.cur != '\n' && isspace( *state.cur ) )
            ++state.cur;

        return token( T_WHITESPACE, begin, state.cur );
    }
    else if( isdigit(c) )
    {
        state.sol = false;

        if( c=='0' && state.cur != state.end && state.cur[0] == 'x' )
        {
            state.cur++;

            while( state.cur != state.end && isxdigit( *state.cur ) )
                state.cur++;
        }
        else
        {
            while( state.cur != state.end && isdigit(* state.cur) )
                state.cur++;
        }

        token tmp( T_NUMBER, begin, state.cur );
        return tmp;
    }
    else if( c == '_' || isalnum(c) )
    {
        state.sol = false;

        while( state.cur != state.end && (*state.cur == '_' || isalnum (*state.cur) ) )
            state.cur++;

        token t( T_KEYWORD, begin, state.cur );

        if( expand )
            t = expand_macro( state, t );

        return t;
    }
    else if( c == '"' || c == '\'' )
    {
        state.sol = false;

        while( state.cur != state.end && *state.cur != c )
        {
            if( *state.cur == '\\' )
            {
                ++state.cur;

                if( state.cur >= state.end )
                    break;
            }

            if( *state.cur == '\n' )
                ++state.line;

            ++state.cur;
        }

        if( state.cur != state.end )
            ++state.cur;

        return token( T_STRING, begin, state.cur );
    }
    else if( c == '/' && *state.cur == '/' )
    {
        state.sol = false;

        ++state.cur;

        while( state.cur != state.end && *state.cur != '\r' && *state.cur != '\n')
            ++state.cur;

        return token( T_LINECOMMENT, begin, state.cur );
    }
    else if( c == '/' && *state.cur == '*' )
    {
        state.sol = false;

        ++state.cur;

        while( state.cur != state.end && (state.cur[0] != '*' || state.cur[1] != '/') )
        {
            if (*state.cur == '\n')
                ++state.line;

            ++state.cur;
        }

        if( state.cur != state.end && *state.cur == '*' )
            ++state.cur;

        if( state.cur != state.end && *state.cur == '/' )
            ++state.cur;

        return token( T_COMMENT, begin, state.cur );
    }
    else if( c == '#' && state.sol )
    {
        while( state.cur != state.end && isspace( *state.cur ) )
            ++state.cur;
        while( state.cur != state.end && !isspace (*state.cur) )
            ++state.cur;

        return token( T_DIRECTIVE, begin, state.cur );
    }
    else if( c == '\\' && state.cur != state.end && (*state.cur == '\r' || *state.cur == '\n') )
    {
        if( *state.cur == '\r' )
            ++state.cur;
        if( *state.cur == '\n' )
            ++state.cur;

        ++state.line;
        state.sol = true;

        return token( T_CONTLINE, begin, state.cur );
    }
    else
    {
        state.sol = false;

        if( c == '>' && state.cur != state.end && (*state.cur == '>' || *state.cur == '=') )
            ++state.cur;
        else if( c == '<' && state.cur != state.end && (*state.cur == '<' || *state.cur == '=' ) )
            ++state.cur;
        else if( c == '!' && state.cur != state.end && *state.cur == '=' )
            ++state.cur;
        else if( c == '=' && state.cur != state.end && *state.cur == '=' )
            ++state.cur;
        else if( (c == '|' || c == '&' || c == '^') && state.cur != state.end && *state.cur == c )
            ++state.cur;

        return token( T_OPERATOR, begin, state.cur );
    }
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::get_next_nonwhitespace_token( tokenizer& state, bool expand )
{
    token t;

    do
    {
        t = get_next_token( state, expand );
    }
    while( t == T_WHITESPACE || t == T_CONTLINE    ||
           t == T_COMMENT    || t == T_LINECOMMENT );

    return t;
}


// -------------------------------------------------------------------------
GLSLPreprocessor::token
GLSLPreprocessor::expand_macro( tokenizer& state, const token &name )
{
    macro* m = m_macros.get( name );

    // do nothing for non-existing or currently expanding macros
    if( !m || m->expanding )
        return name;

    // save the current tokenization state
    // for error case below; if macro expansion
    // fails in any way, just return the macro token
    // without
    tokenizer saved = state;
    token result;

    // if the macro has an associated function,
    // defer further handling of arguments etc. to it
    if( m->func )
    {
        result = m->func( *this, state );
    }
    else
    {
        // parse argument list if the macro has arguments
        token_list vars;

        if( !m->args.empty() )
        {
            vars.reserve( m->args.size() );

            // if the macro carries an evaluation function,
            // assume that it does argument expansion on its own
            if( !parse_macro_arguments( state, vars, true, true ) )
            {
                state = saved;
                return name;
            }
        }

        // actually expand the macro
        // complex macro parameter logic:

        // 1) if macro has parameters, and too few are given,
        //    evaluate to the macro name
        if( m->args.size() && vars.size() < m->args.size() )
        {
            // restore tokenizer state to right after the macro name
            state = saved;
            return name;
        }

        // 2) if macro has parameters and too many are given, return an error
        if( vars.size() > m->args.size() )
        {
            // restore tokenizer state to right after the macro name
            state = saved;

            ostringstream out;
            out << "macro '" << name.c_str() << " passed " << (int)vars.size() << " arguments, but takes just " << (int)m->args.size();

            error( saved, out.str().c_str() );

            return name;
        }

        // here, we should have the same number of args and params
        assert( m->args.size() == vars.size() );

        // define temporary parameter macros
        std::vector<macro_map::iterator> tmpi( m->args.size() );

        for( size_t i=0; i<m->args.size(); ++i )
            tmpi[i] = m_macros.insert( m_macros.lower_bound(m->args[i]),
                                       std::make_pair(m->args[i],macro(vars[i])) );

        // expand by parsing
        m->expanding = true;

        tokenizer ts( m->value, state );
        result = parse( ts );

        m->expanding = false;

        // undefine temporary macros
        for( size_t i=0; i<tmpi.size(); ++i )
            m_macros.erase( tmpi[i] );
    }

    // compensate for skipped lines
    while( saved.line < state.line )
        ++saved.line, result.append( "\n" );

    return result;
}

// -------------------------------------------------------------------------

/**
 * Operator priority:
 *  0: Whole expression
 *  1: '(' ')'
 *  2: ||
 *  3: &&
 *  4: |
 *  5: ^
 *  6: &
 *  7: '==' '!='
 *  8: '<' '<=' '>' '>='
 *  9: '<<' '>>'
 * 10: '+' '-'
 * 11: '*' '/' '%'
 * 12: unary '+' '-' '!' '~'
 */
GLSLPreprocessor::token
GLSLPreprocessor::evaluate_expression( tokenizer& state, token &result, int priority )
{
    // char tmp[40]; // unused

    do
    {
        result = get_next_token( state, true );
    }
    while( result == T_WHITESPACE ||
           result == T_NEWLINE ||
           result == T_COMMENT ||
           result == T_LINECOMMENT ||
           result == T_CONTLINE );

    token op( T_WHITESPACE );

    // unary operators here
    if( result == T_OPERATOR && result.length() == 1 )
    {
        if( strchr( "+-!~", result.c_str()[0] ) )
        {
            char uop = result[0];
            op = evaluate_expression( state, result, 12 );

            long val;

            if( !parse_value( state, result, val ) )
            {
                ostringstream out;
                out << "unary " << uop << " not applicable";
                error( state, out.str().c_str(), &result );

                return T_ERROR;
            }

            if( uop == '-' )
                result.set_value( -val );
            else if (uop == '!')
                result.set_value( !val );
            else if (uop == '~')
                result.set_value( ~val );
        }
        else if( result[0] == '(' )
        {
            op = evaluate_expression( state, result, 1 );

            if( op == T_ERROR )
                return op;

            if( op == T_EOI )
            {
                error( state, "unclosed parenthesis in #if expression" );
                return T_ERROR;
            }

            if( !(op == T_OPERATOR && op.length() == 1 && op[0] == ')') )
            {
                error( state, "binary operator in expression" );
                return T_ERROR;
            }

            op = get_next_token( state, true );
        }
        else if( result[0] == ')' )
        {
            if( priority == 1 )
            {
                error( state, "expected expression in parentheses" );
                return T_ERROR;
            }
        }
    }

    while( op == T_WHITESPACE ||
           op == T_NEWLINE ||
           op == T_COMMENT ||
           op == T_LINECOMMENT ||
           op == T_CONTLINE )
        op = get_next_token( state, true );

    while( true )
    {
        if( op != T_OPERATOR )
            return op;

        int prio = 0;

        if( op.length() == 1 )
        {
            switch( op[0] )
            {
            case ')': return op;
            case '|': prio = 4; break;
            case '^': prio = 5; break;
            case '&': prio = 6; break;
            case '<':
            case '>': prio = 8; break;
            case '+':
            case '-': prio = 10; break;
            case '*':
            case '/':
            case '%': prio = 11; break;
            }
        }
        else if( op.length() == 2 )
        {
            switch( op[0] )
            {
            case '|': if( op[1] == '|' ) prio = 2; break;
            case '&': if( op[1] == '&' ) prio = 3; break;
            case '=': if( op[1] == '=' ) prio = 7; break;
            case '!': if( op[1] == '=' ) prio = 7; break;
            case '<':
                if( op[1] == '=' )
                    prio = 8;
                else if( op[1] == '<' )
                    prio = 9;
                break;
            case '>':
                if( op[1] == '=' )
                    prio = 8;
                else if( op[1] == '>' )
                    prio = 9;
                break;
            }
        }

        if( !prio )
        {
            error( state, "Expecting operator, got", &op );
            return T_ERROR;
        }

        if( priority >= prio )
            return op;

        token rop;
        token nextop = evaluate_expression( state, rop, prio );
        long vlop, vrop;

        if( !parse_value( state, result, vlop ) )
        {
            ostringstream out;
            out << "left operand of '" << op.c_str() << "' is not a number";
            error( state, out.str().c_str(), &result );

            return T_ERROR;
        }

        if( !parse_value( state, rop, vrop ) )
        {
            ostringstream out;
            out << "right operand of '" << op.c_str() << "' is not a number";
            error( state, out.str().c_str(), &rop);

            return T_ERROR;
        }

        switch( op[0] )
        {
        case '|':
            if (prio == 2)
                result.set_value( vlop || vrop );
            else
                result.set_value( vlop | vrop );
            break;
        case '&':
            if (prio == 3)
                result.set_value( vlop && vrop );
            else
                result.set_value( vlop & vrop );
            break;
        case '<':
            if( op.length() == 1 )
                result.set_value( vlop < vrop );
            else if( prio == 8 )
                result.set_value( vlop <= vrop );
            else if( prio == 9 )
                result.set_value( vlop << vrop );
            break;
        case '>':
            if( op.length() == 1 )
                result.set_value( vlop > vrop );
            else if( prio == 8 )
                result.set_value( vlop >= vrop );
            else if( prio == 9 )
                result.set_value( vlop >> vrop );
            break;
        case '^': result.set_value( vlop ^ vrop ); break;
        case '!': result.set_value( vlop != vrop ); break;
        case '=': result.set_value( vlop == vrop ); break;
        case '+': result.set_value( vlop + vrop ); break;
        case '-': result.set_value( vlop - vrop ); break;
        case '*': result.set_value( vlop * vrop ); break;
        case '/':
        case '%':
            if( vrop == 0 )
            {
                error( state, "Division by zero" );
                return T_ERROR;
            }

            if( op[0] == '/' )
                result.set_value( vlop / vrop );
            else
                result.set_value( vlop % vrop );
            break;
        }

        op = nextop;
    }
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::parse_value( tokenizer& state, const token &tok, long &value )
{
    token r;
    const token *vt = &tok;

    if( ( vt->type == T_KEYWORD ||
          vt->type == T_TEXT ||
          vt->type == T_NUMBER ) &&
          vt->empty() )
    {
        error( state, "trying to evaluate an empty expression" );
        return false;
    }

    if( vt->type == T_TEXT )
    {
        tokenizer ts( tok, state );
        token t = evaluate_expression( ts, r );

        if( t == T_ERROR )
            return false;

        if( t != T_EOI )
        {
            error( state, "garbage after expression", &t );
            return false;
        }

        vt = &r;
    }

    macro *m;

    switch( vt->type )
    {
    case T_EOI:
    case T_ERROR:
        return false;

    case T_KEYWORD:
        // Try to expand the macro
        if( (m = m_macros.get( *vt )) && !m->expanding )
        {
            token x = expand_macro( state, *vt );

            m->expanding = true;
            bool rc = parse_value( state, x, value );
            m->expanding = false;
            return rc;
        }

        // handle_undefined macro, "expand" to 0
        value = 0;
        break;

    case T_TEXT:
    case T_NUMBER:
        if( !vt->get_value (value) )
        {
            error( state, "Not a numeric expression", &tok );
            return false;
        }
        break;

    default:
        error( state, "Unexpected token evaluating conditional expression", &tok );
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::parse_macro_arguments( tokenizer& state, token_list& args, bool expand, bool skipws )
{
    token t, arg;
    args.clear();

    // if skipws == true, consume any whitespace before the arguments
    do
    {
        t = get_next_token( state, false );
    }
    while( skipws && ( t == T_WHITESPACE ||
                       t == T_NEWLINE ||
                       t == T_COMMENT ||
                       t == T_LINECOMMENT ||
                       t == T_CONTLINE ) );

    if( t != T_OPERATOR || t[0] != '(' )
        return false;

    bool finished = false;

    while( !finished )
    {
        unsigned int paren = 0;
        token arg;

        while( true )
        {
            t = get_next_token( state, expand );

            if( t == T_EOI )
            {
                error( state, "Unfinished list of arguments" );
                return false;
            }
            else if( t == T_ERROR )
                return false;
            else if( t == T_OPERATOR )
            {
                if( paren == 0 && t[0] == ',' )
                    break;
                else if( paren == 0 && t[0] == ')' )
                {
                    finished = true;
                    break;
                }
                else if( t[0] == ')' )
                    --paren;
                else if( t[0] == '(' )
                    ++paren;
            }

            if( !( t == T_CONTLINE ||
                   t == T_COMMENT ||
                   t == T_LINECOMMENT ||
                   t == T_NEWLINE ) )
                  arg.append( t );
        }

        args.push_back( arg );
    }

    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_define( tokenizer& state )
{
    token t = get_next_token( state, false );

    if( t != T_KEYWORD )
    {
        error( state, "Macro name expected after #define", &t );
        return false;
    }

    token name = t;
    macro m;

    // parse the argument list without expanding or
    // whitespace skipping
    parse_macro_arguments( state, m.args, false, false );

    do
    {
        t = get_next_token( state, false );
    }
    while( t == T_WHITESPACE );

//    string::const_iterator lastws = state.cur;

    switch( t.type )
    {
        case T_NEWLINE:
        case T_EOI:
            // empty value
            t = token( T_TEXT );
            break;
        case T_ERROR:
            return false;
        default:
            // rest of the token is the value
            t.type = T_TEXT;
            t.append( state.cur, state.end );
            break;
    }

    m.value = t;

    // insert/overwrite the macro and warn if there
    // was a previous definition
    macro_map::iterator mi = m_macros.lower_bound( name );

    if( (mi != m_macros.end() && mi->first == name) )
    {
        mi->second = m;

        ostringstream out;
        out << "redefinition of macro \"" << name << "\"";

        warning( state, out.str().c_str() );
    }
    else
        m_macros.insert( mi, std::make_pair( name, m ) );

    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_undef( tokenizer& state )
{
    token t = get_next_token( state, false );

    if( t.type != T_KEYWORD )
    {
        error( state, "Expecting a macro name after #undef, got", &t );
        return false;
    }

    // Don't barf if macro does not exist - standard C behaviour
    size_t erased = m_macros.erase( t );
    assert( erased < 2 );
    (void) erased; // silence warning about unused variable in release mode

    do
    {
        t = get_next_token( state, false );
    }
    while( t == T_WHITESPACE ||
           t == T_COMMENT ||
           t == T_LINECOMMENT );

    if( t.type != T_EOI )
        error( state, "Warning: Ignoring garbage after directive", &t );

    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_ifdef( tokenizer& state )
{
    if( m_enabled & (1 << 31) )
    {
        error( state, "Too many nested #if directives" );
        return false;
    }

    token next = get_next_token( state, false );

    if( next.type != T_KEYWORD )
    {
        error( state, "Expecting a macro name after #ifdef, got", &next );
        return false;
    }

    m_enabled <<= 1;
    m_prevcnd <<= 1;
    m_prevstm <<= 1;

    if( m_macros.get( next ) )
    {
        m_enabled |= 1;
        m_prevcnd |= 1;
    }

    token t;

    do
    {
         t = get_next_token( state, false );
    }
    while( next == T_WHITESPACE ||
           next == T_COMMENT ||
           next == T_LINECOMMENT );

    if( t.type != T_EOI )
        error( state, "Warning: Ignoring garbage after directive", &t );

    return true;
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::expand_defined( GLSLPreprocessor& pp, tokenizer& state )
{
    token_list args;

    tokenizer saved = state;

    // try to parse a conventional argument list
    if( pp.parse_macro_arguments( state, args, false, true ) )
    {
        if( args.size() != 1 )
        {
            pp.error( state, "defined() takes exactly one argument" );
            return T_ERROR;
        }
    }
    else
    {
        state = saved;
        token t = pp.get_next_nonwhitespace_token( state, false );

        if( t == T_KEYWORD )
            args.push_back( t );
        else
        {
            pp.error( state, "operator \"defined\" requires an identifier" );
            return T_ERROR;
        }
    }

    return pp.m_macros.get( args[0] ) != 0;
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::expand_line( GLSLPreprocessor&, tokenizer& state )
{
    return token( state.line );
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::expand_file( GLSLPreprocessor&, tokenizer& state )
{
    return token( state.sstr );
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::expand_version( GLSLPreprocessor& pp, tokenizer& )
{
    return token( pp.m_versionnumber );
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::evaluate_condition( tokenizer& state )
{
    // in #if, defined(x) must be understood;
    // add it to the macro list temporarily
    macro_map::iterator tmpdi =
        m_macros.insert( m_macros.lower_bound("defined"),
                         std::pair<std::string, macro>( "defined", expand_defined ) );

    // macros must be understood, expand by full parsing
    token parsed = parse( state );

    long val = 0;

    // if the value couldn't be parsed,
    // pretend it evaluated to false
    if( parsed != T_ERROR )
    {
        // create a temporary parser to evaluate the result
        tokenizer tmp( parsed, state );
        parse_value( tmp, parsed, val );
    }

    // remove defined(x) macro
    m_macros.erase( tmpdi );

    return val != 0;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_if( tokenizer& state )
{
    if( m_enabled & (1 << 31) )
    {
        error( state, "Too many nested #if directives" );
        return false;
    }

    m_enabled <<= 1;
    m_prevcnd <<= 1;
    m_prevstm <<= 1;

    if( evaluate_condition(state) )
    {
        m_enabled |= 1;
        m_prevcnd |= 1;
    }

    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_elif( tokenizer& state )
{
    if( m_enabled == 1 )
    {
        error( state, "#else without #if" );
        return true;
    }

    // if there was a previous #else, generate error
    if( m_prevstm & 1 )
    {
        error( state, "#elif after #else" );
        return true;
    }

    // if a previous branch of this conditional was true
    // this one cannot be true
    if( m_prevcnd & 1 )
    {
        m_enabled &= ~1;
        return true;
    }

    // check the actual condition
    if( evaluate_condition(state) )
    {
        m_enabled |= 1;
        m_prevcnd |= 1;
    }
    else
        m_enabled &= ~1;


    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_else( tokenizer& state )
{
    if( m_enabled == 1 )
    {
        error( state, "#else without #if");
        return true;
    }

    // if there was a previous #else, generate error
    if( m_prevstm & 1  )
    {
        error( state, "#elif after #else" );
        return true;
    }

    m_prevstm |= 1;

    // if a previous branch in this if/elif/else/endif was true
    // this one must be false
    if( m_prevcnd & 1 )
        m_enabled &= ~1;
    // if no previous branch was true, this one must be true
    else
    {
        m_enabled |= 1;
        m_prevcnd |= 1;
    }

    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_endif( tokenizer& state )
{
    // take conditional state from "stack"
    m_enabled >>= 1;
    m_prevcnd >>= 1;
    m_prevstm >>= 1;

    if( m_enabled == 0 )
    {
        error( state, "#endif without #if" );
        return true;
    }

    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_version( tokenizer& state )
{
    token t = get_next_token( state, false );

    if( t.type != T_NUMBER )
    {
        error( state, "version number expected after #version", &t );
        return false;
    }

    long number;
    bool rc = t.get_value( number );

    if( !rc )
    {
        error( state, "could not parse version number", &t );
        return false;
    }

    t = get_next_nonwhitespace_token( state, false );

    token profile;

    switch( t.type )
    {
        case T_EOI:
            // no profile given
            break;
        case T_KEYWORD:
            profile = t;
            break;
        case T_ERROR:
            return false;
        default:
            error( state, "expected version profile identifier", &t );
            break;
    }

    t = get_next_nonwhitespace_token( state, false );

    if( t != T_EOI )
    {
        error( state, "extra tokens after version directive", &t );
        return false;
    }

    // if a version was previously set, check that profile matches
    if( m_versionnumber > 0 )
    {
        if( profile != m_versionprofile )
        {
            error( state, "profile specification differs from previous one", &profile );
            return false;
        }
    }

    // set profile
    m_versionprofile = profile;

    if( static_cast<unsigned>( number ) > m_versionnumber )
        m_versionnumber = number;

    return true;
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::handle_import( tokenizer& state )
{
    token filename, t = get_next_token( state, false );

    if( t == T_OPERATOR && t[0] == '<' )
    {
        bool finished = false;

        while( !finished )
        {
            t = get_next_token( state, false );

            switch( t.type )
            {
            case T_EOI:
                error( state, "malformed import directive", &t );
            case T_ERROR:
                return false;
            case T_OPERATOR:
                if( t[0] == '>' )
                {
                    finished = true;
                    continue;
                }
            default:
                filename.append( t );
            }
        }
    }
    else if( t == T_STRING )
    {
        error( state, "\"\" syntax not supported, use <>", &t );
        return false;
    }

    // try to read the file using the provided import directories
    token fullname, source = T_ERROR;
    vector<string>::const_iterator pi;

    if( m_importpaths.empty() )
    {
        error( state, "import path list is empty" );
        return false;
    }

    for( pi=m_importpaths.begin(); pi!=m_importpaths.end(); ++pi )
    {
        fullname.assign( *pi + filename );
        source = read_file( fullname );

        if( source.type != T_ERROR )
            break;
    }

    // error if no valid file was found
    if( source == T_ERROR )
    {
        error( state, "could not locate import file", &filename );
        return false;
    }

    // add import file to the list of source strings, if not
    // already present
    vector<string>::const_iterator si =
        find( m_srcstrings.begin(), m_srcstrings.end(), fullname );

    if( si == m_srcstrings.end() )
        si = m_srcstrings.insert( m_srcstrings.end(), fullname );

    // go ahead and parse
    m_emitline = true;

    {
        tokenizer ts( source.begin(), source.end(), 1, si-m_srcstrings.begin() );
        source = parse( ts );
    }

    m_emitline = true;


    return source;
}

// -------------------------------------------------------------------------

bool  GLSLPreprocessor::handle_line( tokenizer& state )
{
    // read line number
    token t = get_next_token( state, false );

    if( t.type != T_NUMBER )
    {
        error( state, "expected line number", &t );
        return false;
    }

    long linenumber;
    bool rc = t.get_value( linenumber );

    if( !rc )
    {
        error( state, "could not parse line number", &t );
        return false;
    }

    // read source string number
    t = get_next_nonwhitespace_token( state, false );
    long source_string_number = state.sstr;

    switch( t.type )
    {
        case T_EOI:
            // no source string given
            break;
        case T_NUMBER:
            rc = t.get_value( source_string_number );

            if( !rc )
            {
                error( state, "could not parse source string number", &t );
                return false;
            }

            break;
        case T_ERROR:
            return false;
            break;
        default:
            error( state, "expected source string number", &t );
            break;
    }

    t = get_next_nonwhitespace_token( state, false );

    if( t != T_EOI )
    {
        error( state, "extra tokens after line directive", &t );
        return false;
    }

    state.line = linenumber;
    state.sstr = source_string_number;

    m_emitline = true;

    return true;
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::handle_extension( tokenizer& state )
{
    token extname = get_next_token( state, false );

    if( extname.type != T_KEYWORD )
    {
        error( state, "expected extension name", &extname );
        return false;
    }

    token t = get_next_nonwhitespace_token( state, false );

    if( !(t == T_OPERATOR && t.length() == 1 && t[0] == ':') )
    {
        error( state, "expected ':' after extension name", &t );
        return false;
    }

    token extkey = get_next_nonwhitespace_token( state, false );

    if( extkey.type != T_KEYWORD )
    {
        error( state, "expected keyword after ':'", &t );
        return false;
    }

    // see if this extension was already mentioned previously
    map<token,token>::iterator ei = m_extensions.find( extname );

    if( ei != m_extensions.end() )
    {
        if( ei->second != extkey )
        {
            error( state, "extension specification differs from previously encountered specification", &extkey );
            return false;
        }
    }
    else
        m_extensions.insert( make_pair( extname, extkey ) );

    return true;
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::handle_directive( tokenizer& state, const token &body )
{
    // remember tokenizer state for rollback
    // if the directive is not understood
    tokenizer saved = state;

    // analyze preprocessor directive
    string directive = trim_copy( string(++body.begin(), body.end()) );

    // collect the remaining part of the directive until EOL
    token line, ws, t;

    line = get_next_nonwhitespace_token( state, false );

    if( line == T_NEWLINE )
        goto directive_done;

    while( true )
    {
        t = get_next_token( state, false );

        switch( t.type )
        {
            case T_LINECOMMENT:
                // ignore comment, appears as newt
                t = T_NEWLINE;
            case T_EOI:
            case T_NEWLINE:
                goto directive_done;
            case T_COMMENT:
            case T_CONTLINE:
                continue;
            case T_ERROR:
                return t;
            case T_WHITESPACE:
                ws.append( t );
                continue;
            default:
                break;
        }

        line.append( ws );
        ws.clear();
        line.append( t );
        line.type = T_TEXT;
    }

directive_done:

    t = token( T_NEWLINE, "\n" );

    // prepare a subparser for the handlers
    tokenizer subt( line, saved );

    bool output_enabled = ((m_enabled & (m_enabled + 1)) == 0);
    bool rc = true;

    if( directive == "define" && output_enabled )
        rc = handle_define( subt );
    else if( directive == "undef" && output_enabled )
        rc = handle_undef( subt );
    else if( directive == "ifdef" )
        rc = handle_ifdef( subt );
    else if( directive == "ifndef" )
    {
        if( (rc = handle_ifdef( subt )) )
        {
            m_enabled ^= 1;
            m_prevcnd ^= 1;
        }
    }
    else if( directive == "if" )
        rc = handle_if( subt );
    else if( directive == "elif" )
        rc = handle_elif( subt );
    else if( directive == "else" )
        rc = handle_else( subt );
    else if( directive == "endif" )
        rc = handle_endif( subt );
    else if( directive == "version" && output_enabled )
        rc = handle_version( subt );
    else if( directive == "import" && output_enabled )
        t = handle_import( subt );
    else if( directive == "line" && output_enabled )
    {
        rc = handle_line( subt );
        state.line = subt.line;
        state.sstr = subt.sstr;
    }
    else if( directive == "extension" && output_enabled )
        rc = handle_extension( subt );
    else if( directive == "extrawurst" && output_enabled )
    {
        warning( state, "Extrawust denied!" );
        rc = true;
    }
    else
    {
        state = saved;
        return body;
    }

    if( !rc )
        return T_ERROR;

    return t;
}

// -------------------------------------------------------------------------

void GLSLPreprocessor::define( const string& name, const string& value )
{
    macro_map::iterator mi = m_macros.lower_bound( name );

    // erase all previous definitions if any
    while( mi != m_macros.end() && mi->first == name )
        m_macros.erase( ++mi );

    m_macros.insert( mi, std::make_pair( name, macro(token(value)) ) );
}

// -------------------------------------------------------------------------

void GLSLPreprocessor::define( const string& name, long value )
{
    macro_map::iterator mi = m_macros.lower_bound( name );

    // erase all previous definitions if any
    while( mi != m_macros.end() && mi->first == name )
        m_macros.erase( ++mi );

    m_macros.insert( mi, std::make_pair( name, macro(token(value)) ) );
}

// -------------------------------------------------------------------------

bool GLSLPreprocessor::undefine( const string& name )
{
    size_t erased = m_macros.erase( name );
    assert( erased < 2 );

    return erased > 0;
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token
GLSLPreprocessor::parse( tokenizer &state )
{
    // output accumulator
    ostringstream output;

    // track state of output enabling
    bool old_output_enabled = true;
    bool output_enabled = true;
    int  output_disabled_line = 0;
    int  contlines = 0;
    bool had_error = false;

    while( true )
    {
        size_t old_line = state.line;
        token t = get_next_token( state, true );

        switch( t.type )
        {
        case T_ERROR:
            // continue parsing to be able to report further errors
            had_error = true;
            continue;

        case T_EOI:
            // done parsing
            return had_error ? T_ERROR : token( T_TEXT, output.str() );

        case T_COMMENT:
            if( output_enabled )
            {
                // compensate for commented out / skipped newlines
                size_t skipped = count( t.begin(), t.end(), '\n' );

                while( skipped )
                    --skipped, output << '\n';

                output << ' ';
            }
            break;

        case T_LINECOMMENT:
            // C++ comments are simply ignored
            continue;

        case T_DIRECTIVE:

            // might have to emit a line here?
            if( output_enabled )
            {
                if( m_emitline )
                {
                    output << "#line " << state.line << ' ' << state.sstr << '\n';
                    m_emitline = false;
                }
            }

            // handle preprocessor directives
            t = handle_directive( state, t );

            if( t == T_ERROR )
            {
                had_error = true;
                continue;
            }

            // on return from directive, output might have
            // been enabled/disabled; if this is the case,
            // check whether a line directive should be
            // emitted
            output_enabled = ((m_enabled & (m_enabled + 1)) == 0);

            if( output_enabled != old_output_enabled )
            {
                if( output_enabled )
                {
                    size_t missing = old_line - output_disabled_line;

                    while( missing )
                        --missing, output << '\n';
                }
                else
                    output_disabled_line = old_line;

                old_output_enabled = output_enabled;
            }

            if( output_enabled )
                output << t;

            break;

        case T_CONTLINE:

            contlines++;
            continue;

        case T_NEWLINE:

            // compensate for the line continuations
            // to keep line numeration intact
            for( ; output_enabled && contlines ; contlines-- )
                output << '\n';

            goto default_output;

        case T_WHITESPACE:

            goto default_output;

        default_output:
        default:

            if( output_enabled )
            {
                if( m_emitline )
                {
                    output << "#line " << old_line << ' ' << state.sstr << '\n';
                    m_emitline = false;
                }

                output << t;
            }
            break;
        }
    }

    if( m_enabled != 1 )
    {
        error( state, "Unclosed #if at end of source" );
        return T_ERROR;
    }

    return had_error ? T_ERROR : token( T_TEXT, output.str() );
}

// -------------------------------------------------------------------------

void GLSLPreprocessor::add_import_path( const string& path )
{
    if( path.empty() || path.back() == '/' || path.back() == '\\' )
        m_importpaths.push_back( path );
    else
        m_importpaths.push_back( path + '/' );
}

// -------------------------------------------------------------------------

GLSLPreprocessor::token GLSLPreprocessor::read_file( const string& filename ) const
{
    token result = T_ERROR;

    ifstream in( filename.c_str() );

    if( !in.is_open() )
        return result;

    result.assign( istreambuf_iterator<char>(in), istreambuf_iterator<char>() );
    result.append( "\n" );

    if( !in.good() )
        return result;

    result.type = T_TEXT;
    return result;
}

// -------------------------------------------------------------------------

string GLSLPreprocessor::process( const string& source, const string* name )
{
    // add the input name to the list of source strings
    m_srcstrings.clear();
    m_srcstrings.push_back( name ? *name : "<string>" );

    // reset state variables
    m_enabled = 1;
    m_prevcnd = 0;
    m_prevstm = 0;
    m_emitline = true;

    // clear known extensions list
    m_extensions.clear();

    // initial version is 0
    m_versionnumber = 0;
    m_versionprofile.clear();

    // reset macro list and add defaults
    m_macros.insert( std::pair<std::string, macro>( "__LINE__", expand_line ) );
    m_macros.insert( std::pair<std::string, macro>( "__FILE__", expand_file ) );
    m_macros.insert( std::pair<std::string, macro>( "__VERSION__", expand_version ) );

    tokenizer ts( source.begin(), source.end(), 1, 0 );
    token parsed = parse( ts );

    if( parsed == T_ERROR )
        return string();

    // construct the final output
    std::ostringstream out;

    // emit version number if one was found
    if( m_versionnumber != 0 )
    {
        out << "#version " << m_versionnumber;

        if( !m_versionprofile.empty() )
            out << ' ' << m_versionprofile;

        out << '\n' << '\n';
    }

    // list extensions found specifications
    if( m_extensions.size() )
    {
        std::map<token,token>::iterator ei;

        for( ei=m_extensions.begin(); ei!=m_extensions.end(); ++ei )
            out << "#extension " << ei->first << " : " << ei->second << '\n';

        out << '\n';
    }

    // list source strings
    out << "// source strings:" << '\n';

    for( size_t s=0; s<m_srcstrings.size(); ++s )
        out << "// " << std::setw(2) << s << ": <" << m_srcstrings[s] << ">\n";

    out << '\n';

    // done
    return out.str() + parsed;
}

// -------------------------------------------------------------------------

string GLSLPreprocessor::process_file( const std::string& filename )
{
    token source = read_file( filename );

    if( source == T_ERROR )
        return string();

    return process( source, &filename );
}

} // namespace ugl
