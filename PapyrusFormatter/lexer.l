%{
    #include <string>
    #include <iostream>
    
    #include "UtilsLight.h"

    using namespace std;

    #define YYSTYPE Node*
    #include "parser.tab.h"
    extern YYSTYPE yylval;
%}

Q (Q|q)
W (W|w)
E (E|e)
R (R|r)
T (T|t)
Y (Y|y)
U (U|u)
I (I|i)
O (O|o)
P (P|p)
A (A|a)
S (S|s)
D (D|d)
F (F|f)
G (G|g)
H (H|h)
J (J|j)
K (K|k)
L (L|l)
Z (Z|z)
X (X|x)
C (C|c)
V (V|v)
B (B|b)
N (N|n)
M (M|m)

identifier    [_a-zA-Z][_a-zA-Z0-9]*
scriptname    {S}{C}{R}{I}{P}{T}{N}{A}{M}{E}
extends       {E}{X}{T}{E}{N}{D}{S}
native        {N}{A}{T}{I}{V}{E}
global        {G}{L}{O}{B}{A}{L}
auto          {A}{U}{T}{O}
autoreadonly  {A}{U}{T}{O}{R}{E}{A}{D}{O}{N}{L}{Y}
const         {C}{O}{N}{S}{T}
hidden        {H}{I}{D}{D}{E}{N}
conditional   {C}{O}{N}{D}{I}{T}{I}{O}{N}{A}{L}
int_t         {I}{N}{T}
float_t       {F}{L}{O}{A}{T}
bool_t        {B}{O}{O}{L}
string_t      {S}{T}{R}{I}{N}{G}
string_single "\'"[^']*"\'"
string_double "\""(\\.|[^"\\])*"\""
bool          ({T}{R}{U}{E})|({F}{A}{L}{S}{E})
none          {N}{O}{N}{E}
customevent   {C}{U}{S}{T}{O}{M}{E}{V}{E}{N}{T}
import        {I}{M}{P}{O}{R}{T}
function      {F}{U}{N}{C}{T}{I}{O}{N}
endfunction   {E}{N}{D}{F}{U}{N}{C}{T}{I}{O}{N}
scripteventname {S}{C}{R}{I}{P}{T}{E}{V}{E}{N}{T}{N}{A}{M}{E}
customeventname {C}{U}{S}{T}{O}{M}{E}{V}{E}{N}{T}{N}{A}{M}{E}
structvarname {S}{T}{R}{U}{C}{T}{V}{A}{R}{N}{A}{M}{E}
lenght        {L}{E}{N}{G}{T}{H}
digit         [0-9]
int           ({digit}+)|(0{X}({digit}|[a-fA-f])+)
float         ({digit}+\.{digit}+([eE][+-]{digit}+)?)
eq_op         "="|"*="|"/="|"%="|"+="|"-="
cmp           "<"|">"|"<="|">="|"=="|"!="
as            {A}{S}
new           {N}{E}{W}
continue      {C}{O}{N}{T}{I}{N}{U}{E}
break         {B}{R}{E}{A}{K}
if            {I}{F}
else          {E}{L}{S}{E}
endif         {E}{N}{D}{I}{F}
elseif        {E}{L}{S}{E}{I}{F}
while         {W}{H}{I}{L}{E}
endwhile      {E}{N}{D}{W}{H}{I}{L}{E}
return        {R}{E}{T}{U}{R}{N}
event         {E}{V}{E}{N}{T}
endevent      {E}{N}{D}{E}{V}{E}{N}{T}
property      {P}{R}{O}{P}{E}{R}{T}{Y}
endproperty   {E}{N}{D}{P}{R}{O}{P}{E}{R}{T}{Y}
state         {S}{T}{A}{T}{E}
endstate      {E}{N}{D}{S}{T}{A}{T}{E}
comm_m        ";/"([^/]|[\n]|(\/+([^/;]|[\n])))*\/+";"
comm_s        ";"[^/\n][^\n]*
docstring     "{"[^}]*"}"
ws            [ \t]+
cont_line     \\[ \t]*\n
eol           [;]?\n

%%

[(]             { return LB; }
[)]             { return RB; }
[[]             { return LQB; }
[]]             { return RQB; }
[,]             { return COMMA; }
[\.]            { return DOT; }
{eol}           { yylval = new NodeWS(yytext, 2); return EOL; }

{new}           { return NEW; }
{state}         { return STATE; }
{if}            { return IF; }
{elseif}        { return ELSEIF; }
{endif}         { return ENDIF; }
{while}         { return WHILE; }
{property}      { return PROPERTY; }
{customevent}   { return CUSTOMEVENT; }
{native}        { return NATIVE; }
{event}         { yylval = new NodeConst("event");        return EVENT; }
{function}      { yylval = new NodeConst("function");     return FUNCTION; }
{import}        { yylval = new NodeConst("import");       return IMPORT; }
{extends}       { yylval = new NodeConst("extends");      return EXTENDS; }
{scriptname}    { yylval = new NodeConst("Scriptname");   return SCRIPTNAME; }
{autoreadonly}  { yylval = new NodeConst("autoreadonly"); return AUTOREADONLY; }
{auto}          { yylval = new NodeConst("auto");         return AUTO; }
{endevent}      { yylval = new NodeConst("endevent");     return ENDEVENT; }
{else}          { yylval = new NodeConst("else");         return ELSE; }
{return}        { yylval = new NodeConst("return");       return RETURN; }
{endstate}      { yylval = new NodeConst("endstate");     return ENDSTATE; }
{endwhile}      { yylval = new NodeConst("endwhile");     return ENDWHILE; }
{endproperty}   { yylval = new NodeConst("endproperty");  return ENDPROPERTY; }
{endfunction}   { yylval = new NodeConst("endfunction");  return ENDFUNCTION; }

{float}         { yylval = new NodeConst(yytext); return FLOAT; }
{int}           { yylval = new NodeConst(yytext); return INTEGER; }
{bool}          { yylval = new NodeConst(yytext); return BOOL; }
{none}          { yylval = new NodeConst(yytext); return NONE; }
{conditional}   { yylval = new NodeConst(yytext); return CONDITIONAL; }
{global}        { yylval = new NodeConst(yytext); return GLOBAL; }
{hidden}        { yylval = new NodeConst(yytext); return HIDDEN; }

{as}            { yylval = new NodeConst(yytext); return AS; }
{eq_op}         { yylval = new NodeConst(yytext); return EQ; }
{cmp}           { yylval = new NodeConst(yytext); return CMP; }
[/%*]           { yylval = new NodeConst(yytext); return MUL; }
[+-]            { yylval = new NodeConst(yytext); return ADD; }
[!]             { yylval = new NodeConst(yytext); return UNAR; }
\&\&            { yylval = new NodeConst(yytext); return AND; }
\|\|            { yylval = new NodeConst(yytext); return OR; }

{int_t}         { yylval = new NodeConst(yytext); return BASETYPE; }
{float_t}       { yylval = new NodeConst(yytext); return BASETYPE; }
{bool_t}        { yylval = new NodeConst(yytext); return BASETYPE; }
{string_t}      { yylval = new NodeConst(yytext); return BASETYPE; }

{docstring}     { yylval = new NodeConst(yytext); return DOCSTRING; }
{identifier}    { yylval = new NodeConst(yytext); return ID; }
{string_double} { yylval = new NodeConst(yytext); return STRING; }

{ws}            { yylval = new NodeWS(yytext, 0); return WS; }
{comm_s}        { yylval = new NodeWS(yytext, 1); return WS; }
{comm_m}        { yylval = new NodeWS(yytext, 1); return WS; }
{cont_line}     { yylval = new NodeWS(yytext, 2); return WS; }

.               {
                    cout << "???? '" << yytext << "'" << endl;
                    return OTHER;
                }
                
%%

void openinfile(const char* name) {
    if (!name)
        return;
    fopen_s(&yyin, name, "r");
}

void closeintpile() {
    fclose(yyin);
}
