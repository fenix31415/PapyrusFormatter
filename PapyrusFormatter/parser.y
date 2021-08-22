%{
    #include <iostream>
    #include <stdio.h>
    #include <algorithm>
    #include <string>
    #include <vector>
    
    #include "Utils.h"
    #include "Header.h"
    
    using namespace std;
    
    #define YYSTYPE Node*
    
    #define YYERROR_VERBOSE 1
    #define YYINITDEPTH 500
    
    int  wrapRet = 1;
    
    int yylex(void);
    extern "C" {
        int yywrap( void ) {
            return wrapRet;
        }
    }
    void yyerror(const char *str) {
         cout << "[@#$$!@#^&^!#$@] RRParserRR: " << str << endl;
    }

%}

%token SCRIPTNAME EXTENDS

%token NATIVE GLOBAL AUTO AUTOREADONLY HIDDEN CONDITIONAL CUSTOMEVENT IMPORT
%token FUNCTION ENDFUNCTION IF ENDIF ELSE ELSEIF WHILE ENDWHILE RETURN ENDEVENT EVENT ENDPROPERTY PROPERTY STATE ENDSTATE

%token BASETYPE

%token STRING BOOL NONE INTEGER FLOAT

%token MUL AS
%token NEW
%token ADD CMP
%token LB "("
%token RB ")"
%token LQB "["
%token RQB "]"
%token EQ "="
%token COMMA ","
%token DOT "."
%token AND "&&"
%token OR "||"
%token UNAR "!"

%token OTHER DOCSTRING WS EOL

%token ID

%start all_file

%%

all_file: Program { $$ = $1; $$->print(outname); delete $$; }

Program
  : Header mb_ProgramBody { $$ = new NodeConstList({$1, $2}, {"", "\n", ""});}

mb_ProgramBody:
    %empty{ $$ = new Node_mb();}
    | EOL some_Elements mb_wss{ $$ = new NodeWS_rem($2, $3, true); }
    | EOL some_Elements mb_wss Element{ $2->addch(new NodeWSl_enh($4, $3)); $$ = $2; }

Header
  : some_eols header_start header_end
  { $$ = new NodeNws(new NodeConstList({$2, $3}), TrimOptionsPresets::l__line, $1, false); }

header_start: SCRIPTNAME wss ID { $$ = new NodeConstList({new NodeWSoneline($1, $2), $3}); }

mb_extends:  %empty { $$ = new Node_mb(); } | EXTENDS wss ID mb_wss { $$ = new NodeConstList({new NodeWSoneline($1, $2), new NodeWSr_enh($3, $4)}, {" "}); }

header_end
  :                 %empty { $$ = new Node_mb(); }
  | wss mb_extends { $$ = new NodeWSr_enh($2, $1, false); }
  | wss mb_extends scriptFlag some_scriptFlags mb_wss
  { $$ = new NodeConstList({new NodeWSr_enh($2, $1, false), new NodeConstList({$3, new NodeWSr_enh($4, $5)}, {}, {"", " "})}, {"", " "}, {"", " "}); }

some_Elements
  :                           %empty { $$ = new NodeProgStatements(false); }
  | some_Elements mb_wss Element EOL { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }
  | some_Elements mb_wss         EOL{ $$ = $1; $$->addch(new NodeWScomment($2)); }

Element
    : IMPORT wss ID mb_wss{ $$ = new NodeConstListT("Import", {new NodeWSoneline($1, $2), new NodeWSr_enh($3, $4)}, {"", "", "\n"});}
  | property               { $$ = $1; }
  | function               { $$ = $1; }
  | globalDefin            { $$ = $1; }
  | event                  { $$ = $1; }
  | State                  { $$ = $1; }
  | DOCSTRING mb_wss       { $$ = new NodeWSnewline($1, $2, true); }


globalDefin: type ID mb_wss mb_initExpr globalDefin_end{ $$ = new NodeConstListT("GlobalDefin", {$1, new NodeWSr_enh($2, $3), $4, $5}, {"", " ", "", "", "\n"}, {"","",""," ", "", ""});}
globalDefin_end
  :                                         %empty { $$ = new Node_mb(); }
  | propertyFlag some_propertyFlags mb_wss { $$ = new NodeWSoneline(new NodeConstList({$1, new NodeWSr_enh($2, $3)}, {}, {"", " "}), new NodeConst(" "), false); }

// ------------  STATES  ------------------

State
  : state_start mb_wss EOL some_StateElements mb_wss ENDSTATE mb_wss
  { $$ = new NodeConstListT("State", {$1, new NodeWSr_enh(new Node_mb(), $2), $4, new NodeWSr_enh(new NodeWSr_enh($6, $7), $5, false)}, {"","","", "", "\n"});}

state_start: mb_auto STATE wss ID  { $$ = new NodeConstList({$1, new NodeWSoneline($4, $3, false)}, {"", "state", "\n"});}
some_StateElements
  :                                                  %empty { $$ = new NodeProgStatements(); }
  | some_StateElements mb_wss function         EOL  { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }
  | some_StateElements mb_wss event            EOL  { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }
  | some_StateElements mb_wss DOCSTRING mb_wss EOL  { $$ = $1; $$->addch(new NodeWSl_enh(new NodeWSr_enh($3, $4), $2)); }
  | some_StateElements mb_wss                  EOL{ $$ = $1; $$->addch(new NodeWScomment($2)); }
mb_auto:  %empty { $$ = new Node_mb(); } | AUTO wss { $$ = new NodeWSoneline($1, $2); }

// ^^^^^^^^^^^^^^  STATES  ^^^^^^^^^^^^^^^^

// ------------  FLAGS  -----------------

scriptFlag
  : HIDDEN       { $$ = $1; }
  | CONDITIONAL  { $$ = $1; }
propertyFlag
  : HIDDEN       { $$ = $1; }
  | CONDITIONAL  { $$ = $1; }
propertyFlag_auto
  : AUTO         { $$ = $1; }
  | AUTOREADONLY { $$ = $1; }
//variableFlag
//  : CONDITIONAL  { $$ = $1; }
functionFlag     // + NATIVE
  : GLOBAL       { $$ = $1; }

some_scriptFlags:    %empty { $$ = new NodeList(); } | some_scriptFlags   wss scriptFlag      { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }
some_functionFlags:  %empty { $$ = new NodeList(); } | some_functionFlags wss functionFlag    { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }
some_propertyFlags:  %empty { $$ = new NodeList(); } | some_propertyFlags wss propertyFlag    { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }
//some_variableFlags:  %empty { $$ = new NodeList(); } | some_variableFlags wss variableFlag    { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }


// ^^^^^^^^^^^^^^  FLAGS  ^^^^^^^^^^^^^^^^^


// ------------  PROPERTIES  -------------------

property
    : property_start mb_wss mb_initStatic propertyFlag_auto property_end{ $$ = new NodeConstListT($4->value.size() == 4 ? "NodePropAuto" : "NodePropRead", {new NodeWSr_enh($1, $2), $3, $4, $5}, {"", "", " ", "", "\n"}, {"", " "});}
  | property_start property_end EOL some_functions mb_wss ENDPROPERTY mb_wss { $$ = new NodeConstListT("NodePropBig", {$1, $2, $4, new NodeWSl_enh(new NodeWSr_enh($6, $7), $5)}, {"","","\n","","\n"});}

property_start
  : type PROPERTY wss ID { $$ = new NodeConstList({$1, new NodeWSoneline($4, $3, false)}, {"", " property"}); }
property_end
  :                                     mb_wss { $$ = new NodeWSr_enh(new Node_mb(), $1); }
  | wss propertyFlag some_propertyFlags mb_wss { $$ = new NodeConstList({new NodeWSoneline($2, $1, false), new NodeWSr_enh($3, $4)}, {}, {"", " "}); }

// ^^^^^^^^^^^^^  PROPERTIES  ^^^^^^^^^^^^^^^^^^

// -----------  INITIALIZATION  --------------

initConst: "=" mb_wss SignedConst{ $$ = new NodeConstList({new NodeWSmb_line($1, $2), $3}); }
initNew:   "=" mb_wss new_expr   { $$ = new NodeConstList({new NodeWSmb_line($1, $2), $3}); }
initExpr:  "=" mb_wss Expression { $$ = new NodeConstList({new NodeWSoneline($1, $2), $3}); }

initStatic
  : initConst   { $$ = $1; }
  | initNew     { $$ = $1; }

mb_initExpr:     %empty { $$ = new Node_mb(); } | initExpr                { $$ = new NodeWSoneline($1, new NodeConst(" "), false); }
mb_initStatic:   %empty { $$ = new Node_mb(); } | initStatic mb_wss       { $$ = new NodeConstList({new NodeWSr_enh($1, $2)}); }
mb_initializer:  %empty { $$ = new Node_mb(); } | ID mb_wss mb_initStatic { $$ = new NodeConstList({new NodeWSr_enh($1, $2), $3}); }

// ^^^^^^^^^^^^^^ INITIALIZATION  ^^^^^^^^^^^^^^^^

// -------------- PARAMETERS & ARGS  --------------

argList: "(" mb_wss mb_inArgList ")"  { $$ = new Node_enh("(", $2, $3, ")"); }

mb_inArgList:                    %empty { $$ = new Node_mb(); } | inArgList { $$ = new Node_mb($1); }

inArgList
  : argument                      { $$ = new NodeList(false); $$->addch($1); }
  | inArgList "," mb_wss argument { $$ = $1; $$->addch(new NodeWSoneline($4, $3, false)); }

argument
    : ID mb_wss "=" mb_wss Expression{ $$ = new NodeBin(new NodeWSmb_line($1, $2), new NodeWSmb_line($3, $4), $5, ""); }
  | Expression                         { $$ = $1; }

parameter:
  type mb_initializer           { $$ = new NodeParam($1, $2); }

InParamList
  : parameter                        { $$ = new NodeList(false); $$->addch($1); }
  | InParamList "," mb_wss parameter { $$ = $1; $$->addch(new NodeWSoneline($4, $3, false)); }

paramList
  : "(" mb_wss mb_InParamList ")" mb_wss { $$ = new Node_enhws($3, "(", $2, ")", $5); }

mb_InParamList:                %empty { $$ = new Node_mb(); } | InParamList { $$ = new Node_mb($1); }

// ^^^^^^^^^^^^  PARAMETERS & ARGS  ^^^^^^^^^^^^^^^^^^

// ------------- FUNCTIONS & EVENTS  ------------------

function
  : functionHeader EOL Statements mb_wss ENDFUNCTION mb_wss  { $$ = new NodeFunc($1, $3, new NodeWSl_enh(new NodeWSnewline($5, $6, true), $4)); }
  | functionHeader NATIVE mb_wssfunctionflags                { $$ = new NodeConstListT("NodeFunc_native", {$1, $3}, {""," native\n"});}

functionHeader
  : mb_type FUNCTION wss ID mb_wss paramList mb_functionFlags
  { $$ = new NodeFuncHeader($1, new NodeWSoneline($2, $3), new NodeWSmb_line($4, $5), $6, $7); }
mb_wssfunctionflags
  :  %empty { $$ = new Node_mb(); }
  | wss mb_functionFlags { $$ = new Node_mb(new NodeWSr_enh($2, $1, false)); }
mb_functionFlags
  :  %empty { $$ = new Node_mb(); }
  | functionFlag some_functionFlags mb_wss { $$ = new Node_mb(new NodeConstList({$1, new NodeWSr_enh($2, $3)}, {" "}, {"", " "})); }

some_functions
  :                                              %empty { $$ = new NodeProgStatements(); }
  | some_functions mb_wss function EOL          { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }
  | some_functions mb_wss DOCSTRING mb_wss EOL  { $$ = $1; $$->addch(new NodeWSl_enh(new NodeWSr_enh($3, $4), $2)); }
  | some_functions mb_wss EOL                   { $$ = $1; $$->addch(new NodeWScomment($2));}

// --------  EVENTS  --------

event
  : eventHeader EOL Statements mb_wss ENDEVENT mb_wss { $$ = new NodeFunc($1, $3, new NodeWSl_enh(new NodeWSr_enh($5, $6), $4)); }
  | eventHeader NATIVE mb_wss                         { $$ = new NodeConstListT("NodeFunc", {$1, $3}, {""," native"});}

eventHeader
  : EVENT wss ID mb_wss paramList
  { $$ = new NodeFuncHeader(new Node_mb(), new NodeWSoneline($1, $2), new NodeWSmb_line($3, $4), $5, new Node_mb()); }

// ^^^^^^^^^^^^^  FUNCTIONS & EVENTS  ^^^^^^^^^^^^^^^^^^^


// -------------  TYPES  -----------------

type
  : type_s                  { $$ = $1; }
  | arrayType               { $$ = $1; }

type_s: ID       mb_wss     { $$ = new NodeWSr_enh($1, $2); }
      | BASETYPE mb_wss     { $$ = new NodeWSr_enh($1, $2); }

arrayType
  : ID       mb_wss "[" mb_wss "]" mb_wss { $$ = new NodeConstList({new NodeWSmb_line($1, $2), new Node_enhws(new Node_mb(), "[", $4, "]", $6)}); }
  | BASETYPE mb_wss "[" mb_wss "]" mb_wss { $$ = new NodeConstList({new NodeWSmb_line($1, $2), new Node_enhws(new Node_mb(), "[", $4, "]", $6)}); }

mb_type:                     %empty { $$ = new Node_mb(); } | type { $$ = new Node_mb($1); }

// ^^^^^^^^^^^^^^^  TYPES  ^^^^^^^^^^^^^^^^

// -------------  STATEMENTS  --------------

Statements
  :                                  %empty { $$ = new NodeListEndl(NodeListEndl::Options::AllEnable | NodeListEndl::Options::AddShift); }
  | Statements mb_wss Statement EOL { $$ = $1; $$->addch(new NodeWSl_enh($3, $2)); }
  | Statements mb_wss           EOL{ $$ = $1; $$->addch(new NodeWScomment($2)); }

Statement
  : ifStatement mb_wss           { $$ = new NodeWSr_enh($1, $2); }
  | whileStatement mb_wss        { $$ = new NodeWSr_enh($1, $2); }
  | RETURN mb_Expression         { $$ = new NodeConstList({$1, $2}); }
  | Expression                   { $$ = $1; }
  | type ID mb_wss mb_initExpr   { $$ = new NodeConstList({$1, new NodeWSr_enh($2, $3), $4}, {"", " "}); }
  | lvalue "=" mb_wss Expression { $$ = new NodeBin($1, new NodeWSoneline($2, $3), $4); }
  | DOCSTRING mb_wss             { $$ = new NodeWSr_enh($1, $2); }


lvalue
  : dotAtom               { $$ = $1; }

mb_Expression: mb_wss { $$ = new NodeWSr_enh(new Node_mb(), $1); } | wss Expression { $$ = new Node_mb(new NodeWSoneline($2, $1, false)); }

ifStatement
  : IF mb_wss Expression EOL Statements mb_wss some_elseIfBlock mb_elseBlock ENDIF
  { $$ = new NodeIf(new NodeWSoneline($3, $2, false), $5, $6, $7, $8); }

some_elseIfBlock:  %empty { $$ = new NodeListEndl(NodeListEndl::Options::Disable); } | some_elseIfBlock elseIfBlock  { $$ = $1; $$->addch($2); }
elseIfBlock
  : ELSEIF mb_wss Expression EOL Statements mb_wss
  { $$ = new NodeElseIf(new NodeWSoneline($3, $2, false), $5, $6); }

mb_elseBlock:  %empty { $$ = new Node_mb(); } |  elseBlock  { $$ = new Node_mb($1); }
elseBlock: ELSE mb_wss EOL Statements mb_wss { $$ = new NodeElse(new NodeWSr_enh($1, $2), $4, $5); }

whileStatement:
  WHILE mb_wss Expression EOL Statements mb_wss ENDWHILE
  { $$ = new NodeWhile(new NodeWSoneline($3, $2, false), $5, new NodeWSl_enh($7, $6)); }

// ^^^^^^^^^^^^^^  STATEMENTS  ^^^^^^^^^^^^^^^^^

// ------------  EXPRESSIONS  -----------------

Expression: OrExpr { $$ = $1; }

OrExpr
  : AndExpr                      { $$ = $1; }
  | AndExpr "||" mb_wss OrExpr   { $$ = new NodeBin($1, new NodeWSoneline($2, $3), $4); }

AndExpr
  : BolExpr                      { $$ = $1; }
  | BolExpr "&&" mb_wss AndExpr  { $$ = new NodeBin($1, new NodeWSoneline($2, $3), $4); }

BolExpr
  : AddExpr                      { $$ = $1; }
  | AddExpr CMP mb_wss BolExpr   { $$ = new NodeBin($1, new NodeWSoneline($2, $3), $4); }

AddExpr
  : MulExpr                      { $$ = $1; }
  | MulExpr ADD mb_wss AddExpr   { $$ = new NodeBin($1, new NodeWSoneline($2, $3), $4); }

MulExpr
  : UnarExpr                     { $$ = $1; }
  | UnarExpr MUL mb_wss MulExpr  { $$ = new NodeBin($1, new NodeWSoneline($2, $3), $4); }

UnarExpr
  : cast                         { $$ = $1; }
  | UNAR mb_wss cast             { $$ = new NodeConstList({new NodeWSmb_line($1, $2), $3}); }
  | ADD  mb_wss cast             { $$ = new NodeConstList({new NodeWSmb_line($1, $2), $3}); }

cast
  : dotAtom                      { $$ = $1; }
  | dotAtom AS mb_wss type_s     { $$ = new NodeBin($1, new NodeWSoneline($2, $3), $4); }

dotAtom
  : arrayAtom some_dots          { $$ = new NodeConstList({$1, $2}, {}, {"","."}); }
  | Const mb_wss                 { $$ = new NodeWSr_enh($1, $2); }

some_dots:  %empty { $$ = new NodeList(); } | some_dots "." mb_wss arrayAtom { $$ = $1; $$->addch(new NodeWSmb_line($4, $3, false)); }

arrayAtom
  : atom              mb_arrExprSuff { $$ = new NodeConstList({$1, $2}); }
  | ID                mb_arrExprSuff { $$ = new NodeConstList({$1, $2}); }
  | ID mb_wss argList mb_arrExprSuff { $$ = new NodeConstList({new NodeConstList({new NodeWSmb_line($1, $2), $3}, {}, {"", ","}), $4}); }

atom
  : "(" mb_wss Expression ")" { $$ = new Node_enh("(", $2, new Node_mb($3), ")"); }
  | new_expr                          { $$ = $1; }

mb_arrExprSuff: mb_wss { $$ = new NodeWSr_enh(new Node_mb(), $1); } | mb_wss arrExprSuff { $$ = new NodeWSmb_line($2, $1, false); }
arrExprSuff: "[" mb_wss Expression "]" mb_wss { $$ = new Node_enhws(new Node_mb($3), "[", $2, "]", $5); }

new_expr
  : NEW wss type_s "[" mb_wss INTEGER mb_wss "]"
  { $$ = new NodeConstList({new NodeWSoneline($3, $2, false), new Node_enh("[", $5, new NodeWSr_enh($6, $7), "]")}, {"new"}); }

Const
  : Constant{ $$ = $1; }
  | STRING  { $$ = $1; }
  | BOOL    { $$ = $1; }
  | NONE    { $$ = $1; }

SignedConst
  : Const{ $$ = $1; }
  | ADD mb_wss Constant{ $$ = new NodeConstList({new NodeWSmb_line($1, $2), $3}); }

Constant: FLOAT{ $$ = $1; }
          | INTEGER{ $$ = $1; }

// ^^^^^^^^^^^  EXPRESSIONS  ^^^^^^^^^^^^^^^^

// -----------  EOLS  ----------------

wss: WS some_wss { $$ = $2; ((NodeWSS*)$$)->appendl($1); }

mb_wss:  %empty { $$ = new NodeWSS(); } | wss { $$ = $1; }

some_wss:  %empty { $$ = new NodeWSS(); } | some_wss WS { $$ = $1; ((NodeWSS*)$$)->appendr($2); }

some_eols
  :  %empty { $$ = new NodeWSS(); }
  | some_eols EOL { $$ = $1; ((NodeWSS*)$$)->appendr($2); }
  | some_eols WS  { $$ = $1; ((NodeWSS*)$$)->appendr($2); }

// ^^^^^^^^^^^  EOLS  ^^^^^^^^^^^^^^^

%%

int main(int argc, char** argv)
{// clear && bison -d parser.y --verbose -Wno-other && flex lexer.lex && g++ -std=c++17 -c lex.yy.c parser.tab.c && g++ -fsanitize=address -std=c++17 -o parser lex.yy.o parser.tab.o -ll && ./parser < test.psc > out.txt
#ifdef DEBUG
      yydebug = 1;
#endif // DEBUG
#ifdef MYFILES
      return main_impl_dbg();
#else
      return main_impl(argc, argv);
#endif // MYFILES

}