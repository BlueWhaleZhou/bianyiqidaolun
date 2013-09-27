/* ----------------------------PROJECT DESCRIPTION -------------------------
   In this project, you are asked to finish an INCOMPLETE lexical analyzer,
   certain parts are not coded and you should read the code and fill in the gaps.
   I am going to start by describing the tokens. Then I will discuss some
   difficulty in recognizing tokens that have overlapping prefixes. Then I
   will give you the requirements for the project.

   It is very important that you read everything carefully. If you are in a
   rush to finish the project, you cannot afford working fast! You should
   work deliberately and carefully.

   Note: the provided lexical analyzer executes and produces output, but it
   is not always correct output. After you add your code, it should produce
   the correct output in all cases.

   ALPHABET

   the alphabet is
                    {0 , 1, 2, 3, 4, 5, 6, 7, 8, 9,
                     a , b , c , ... , z ,
                     A , B, C , ... , Z ,
                     < , > , = , + , - , _ , * , / ,
                     ( , ) , [ , ] ,
                     ',' , :  , ; , . }

   TOKENS

   In the description of the tokens, we distinguish between regular expression
   symbols and literals. For example 0+9 represents the set {0,9}, whereas
   0'+'9 represents {0+9}, which is a set that contains the single string: 0+9. 
   Literal regular expressions operator symbols are included between single 
   quotation marks to distinguish them from regular expression operators.
 
   The tokens are

                    VAR        = (V)(A)(R)
                    BEGIN      = (B)(E)(G)(I)(N)
                    END        = (E)(N)(D)
                    ASSIGN     = (A)(S)(S)(I)(G)(N)
                    IF         = (I)(F)
                    WHILE      = (W)(H)(I)(L)(E)
                    DO         = (D)(O)
                    THEN       = (T)(H)(E)(N)
                    PRINT      = (P)(R)(I)(N)(T)
                    INT        = (I)(N)(T)
                    REAL       = (R)(E)(A)(L)
                    STRING     = (S)(T)(R)(I)(N)(G)
                    PLUS       = '+'
                    MINUS      =  -
                    UNDERSCORE =  _
                    DIV        =  /
                    MULT       = '*'
                    EQUAL      = '='
                    COLON      =  :
                    COMMA      =  ,
                    SEMICOLON  =  ;
                    LBRAC      =  [
                    RBRAC      =  ]
                    LPAREN     =  '('
                    RPAREN     =  ')'
                    NOTEQUAL   =  <>
                    GREATER    =  >
                    LESS       =  <
                    LTEQ       =  <=
                    GTEQ       =  >=
                    LSHIFT     =  <<
                    RSHIFT     =  >>
                    DOT        =  '.'
                    NUM        =  pdigit digit* + 0
                    REALNUM    =  NUM DOT digit digit*
                    ID         =  letter (letter+digit)*
                    BASENUM    =  base02 + base08 + base16

                where
                        base02      = (02)(x)((pdigit2 digit2*) + 0)
                        base08      = (08)(x)((pdigit8 digit8*) + 0)
                        base16      = (16)(x)((pdigit16 digit16*) + 0)
                        pdigit  = 1+2+3+4+5+6+7+8+9
                        digit   = 0+1+2+3+4+5+6+7+8+9
                        letter  = a+b+...+z+A+B+...+Z
                        pdigit2 = 1
                        digit2  = 0+1
                        pdigit8 = 1+2+3+4+5+6+7
                        digit8  = 0+1+2+3+4+5+6+7
                        pdigit16 = 1+2+3+4+5+6+7+8+9+A+B+C+D+E+F
                        digit16  = 0+1+2+3+4+5+6+7+8+9+A+B+C+D+E+F

Note that 0.00 is allowed as REALNUM, but 00.1 is not allowed
Also, note that token definitions are case-sensitive. So, "while" is not
a WHILE token.

LEXICAL ANALYSIS AND ITS COMPLICATIONS
The main function of lexical analysis is to identify the next token in the remainder of 
the input.  In the program you are given, the function that does that is the getToken() 
function.  If different prefixes of the remaining input match tokens, we look for the 
longest possible matchnig tokem token. For example, if the input is "123.45", we do not 
return three tokens NUM (123), DOT, and NUM(45) because "123.45" is a REALNUM and is a 
longer matching token. As another example, for input "W222", ID is the longest possible 
token starting at the beginning of the input.

Reading the input is done using a function that reads characters one at a time. 
In C, getchar() is such a function. It returns the next input character.

Identifying the longest token is not not always straightforward. For example, consider 
the input

              111.a

As we start reading the digits, we know that we are looking either for NUM
or for a REALNUM. When '.' is read, we think that there might still be a chance
that a REALNUM will be identified. When the 'a' is read, we realize that we should
identify 111 as a NUM and '.' as a DOT.

In general, this might require backtracking multiple characters (alphabet symbols). 
For the set of tokens we are considering, we need to backtrack multiple characters. 
One way to achieve that in the case of REALNUM is to keep track of every position 
in which we have identified a "maximal" token (a token that cannot be extended further). 
For example, in the example above, 111 is a maximal NUM token because for the given 
input it cannot be extended further into a longer NUM token. The token returned is 
the longest maximal token. If the input were:

      111.1

then both 111 and 111.1 are maximal tokens; 111 is a maximal NUM token and 111.1 is a 
maximal REALNUM token. We choose the longer between the two, namely 111.1.

To simplify the identfication of tokens, we should have the cursor pointing
to the next character after the token just identidied. For example, for the 
input "111+", we need to read the '+' to determine that the token is 111. 
Instead of keeping the '+' that we read in a variable of the lexical analyzer, 
we "return it" to the input stream so that next time we call getToken() it 
will read the '+'. A similar approach would work for REALNUM but the solution 
is a little more complicated because we need to return more than one character (see below). 

In the C language, this can ve achieved using the ungetc() function.
Note that the ungetc() function can be called multiple times, but characters that are 
ungot are available for reading operations in the reverse order they were put back into i
the iinput stream.

So, if you execute 

             ungetc('a',stdin); ungetc('b',stdin); 
             x = getchar(); y = getchar();

then x is 'b' and y is 'a'.


Another complication arises when the input exactly matches more than one token
in which case the longest prefix matching rule does not help. 
For example, "WHILE" is a WHILE token, but it also matches the format of ID (identifier). 
In this case, the practice is to identify (return) the token that appears first in the 
list of tokens. In our list WHILE appears before ID, so we identify "WHILE" as WHILE and 
not as ID. However note that input "WHILEHERE" is treated as a single ID token because 
of the longest matching prefix rule.

ABOUT BASENUM

BASENUM is a base specifier (02, 08 or 16) followed by a number in that base e.g. 02x1000111
is 71 expressed in base 2 (binary) and 16xFC7 is 4039 epressed in base 16 (hexadecimal).
We allow bases 2, 8 and 16 and the digits used in the number must conform to the base
otherwise it is not considered a BASENUM. For example 08xFC7 is not a BASENUM because the
base is 8 but the number does not conform to base 8 (remember that for base 8, we can only 
use 0,1,2, 3,4,5,6,7) so 08xFC7 should be interpreted as a NUM (0) followed by another 
NUM (8) and then an ID (xFC7). Here are some examples:

         16xZAB		Should be interpreted as NUM (16) followed by ID (xZAB)

         02x423		Should be interpreted as NUM (0) followed by NUM (2) 
                    followed by ID (x423)

         08xA785	Should be interpreted as NUM (0) followed by NUM (8) 
                    followed by ID (xA785)

         2x11100	Should be interpreted as NUM (2) followed by ID (x11100). 
                    Note that the base is always 2 digits

         02X1100	Should be interpreted as NUM (0) followed by NUM (2) followed 
                    by ID (X1100). Note that X is a capital letter

         02x1034	is a BASENUM (02x10) followed by NUM (34)

         16x00000	is a BASENUM (16x0) followed by four NUM (0) tokens

The following examples are valid BASENUM:

         02x10100011
         16x0
         16xA0FC0
         08x0
         02x0
         16x1F00000000000000000
         08x12345670


REQUIREMENTS

You are asked to finish the getToken() and associated functions to identify ALL 
tokens defined above. The output of the program is a list of tokens and the line
numbers in which they appear. For ID, NUM, REALNUM and BASENUM the actual
token string should be printed.

Your program will read the input from standard input (you do not need to modify
how characters are read). To read input from a file, you can simply execute the
following from the command prompt

        ./a.out <filein >fileout

This will execute the program and treats filein as standard input and fileout as
standard output. So, instead of diplaying output on the terminal, it will simply
write it to file fileout.

You should run and test you program on the general machines. To login to general,
you can execute "ssh username@general.asu.edu" from a command prompt
(where username is your asurite user name) or run the ssh program and connect
to general.

To compile your program, you execute


       gcc scanner.c

from the command prompt.

To test your program, I provide you with a set of test cases and the expected output
for these cases. Your program will be tested on these cases and some other cases.

To automate testing your program on the many cases, I provided you with a shell script
that will automatically run your program on all the test cases and compares your programs
output with the expected output. If they match, it reports success, otherwise, it reports
failures and shows where the expected and produced outputs differ. The script expects
that your executable name is a.out and that the test cases are in a directory called tests.
The script file, the executable and the tests directory should all be in the same directory
for testing. In other words, if your project directory is project2, then a.out, test1.sh
and the tests idirectory are all in the directory project2. To execute the script, 
simply type

      ./test1.sh

from the command prompt. If it complains about test1.sh having no permission, execute

      chmod 700 test1.sh

from the command prompt. You only need to execute chmod once (if you are interested to know, 
chmod changes the file permission to be executable).

The result of executing test1.sh is a list of all the files that pass the tests
and those that do not pass with a comparison between expected output and output of
your code. At the end, there is a number. This is the number of the test cases that pass.

IMPORTANT

read the syllabus for instructions on how to name your files. This is
VERY IMPORTANT.

Also, please read the syllabus on grading of programming assignments.
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h> // Related to string operation
#include <ctype.h> // Related to isspace()

#define TRUE 1
#define FALSE 0

//----------------------------- token types ------------------------------
#define KEYWORDS 12
#define RESERVED 38
#define VAR 1
#define BEGIN 2
#define END 3
#define ASSIGN 4
#define IF 5
#define WHILE 6
#define DO 7
#define THEN 8
#define PRINT 9
#define INT 10
#define REAL 11
#define STRING 12
#define PLUS 13
#define MINUS 14
#define UNDERSCORE 15
#define DIV 16
#define MULT 17
#define EQUAL 18
#define COLON 19
#define COMMA 20
#define SEMICOLON 21
#define LBRAC 22
#define RBRAC 23
#define LPAREN 24
#define RPAREN 25
#define NOTEQUAL 26
#define GREATER 27
#define LESS 28
#define LTEQ 29
#define GTEQ 30
#define LSHIFT 31
#define RSHIFT 32
#define DOT 33
#define ID 34
#define NUM 35
#define REALNUM 36
#define BASENUM 37
#define ERROR 38

//------------------- reserved words and token strings -----------------------
char *reserved[] =
	{	"",
		"VAR",
		"BEGIN",
		"END",
		"ASSIGN",
		"IF",
		"WHILE",
		"DO",
		"THEN",
		"PRINT",
		"INT",
		"REAL",
		"STRING",
		"+",
		"-",
		"_",
		"/",
		"*",
		"=",
		":",
		",",
		";",
		"[",
		"]",
		"(",
		")",
		"<>",
		">",
		"<",
		"<=",
		">=",
		"<<",
		">>",
		".",
		"ID",
		"NUM",
		"REALNUM",
		"BASENUM",
		"ERROR"
		};

int printToken(int ttype)
{
	if (ttype <= RESERVED)
	{
		printf("%s\n",reserved[ttype]);
		return 1;
	}
	else
		return 0;
}
//---------------------------------------------------------

//---------------------------------------------------------
// Global Variables associated with the next input token
#define MAX_TOKEN_LENGTH 100

char token[MAX_TOKEN_LENGTH];     // token string
int  ttype;                       // token type

int  tokenLength;

int line_no = 1;

//----------------------------------------------------------
int skipSpace()
{
	char c;

	c = getchar();
	line_no += (c == '\n');  //if ==, +=1; else +=0
	while (!feof(stdin) && isspace(c))
	{
		c = getchar();
        line_no += (c == '\n');
	}

	// return character to input buffer if eof is not reached
	if (!feof(stdin))
		ungetc(stdin);
}

int isKeyword(char *s)
{
	int i;

	for (i = 1; i <= KEYWORDS; i++)
		if (strcmp(reserved[i],s) == 0)
	   		return i;
	return FALSE;
}

int scan_number()
{
	// This function does not take BASENUM into account!
	// You should consider adding functionality for
	// scanning BASENUM. Depending on how you want to handle
	// the identifying of BASENUM, you might decide to rewrite the function

	char c;
	char c2,c3,c4;

	c = getchar();
	if (isdigit(c))
	{	// The following code scans for a NUM 
		
		token[tokenLength] = c;
		tokenLength++;
		token[tokenLength] = '\0';

		//token started with 0
		if (c == '0')
		{
			// 0 is a NUM by itself			
			c2=getchar();
			//BASENUM 02 and 08	
			if(c2=='2'){
				c3=getchar();
				if(c3=='x'){
					c4=getchar();
					if(c4=='0'||c4=='1'){                           //02x0 or 02x1
						token[tokenLength] = c2;
						tokenLength++;
						token[tokenLength] = c3;
						tokenLength++;
						token[tokenLength] = c4;
						tokenLength++;
						token[tokenLength] = '\0';
						if(c4=='1'){									//02x1 and continue to read the characters behind them
							c=getchar();								//if they are 1s and 0s
							while(c=='1'||c=='0'){
								token[tokenLength] = c;
								tokenLength++;
								c=getchar();
							}	
							ungetc(c,stdin);
							token[tokenLength] = '\0';
						}   
						return BASENUM;							//return 02x0 or 02x1-------- (-=1 or 0)
					}
					else{													//if c4 is not 0 or 1, then this token will not be base-2 num
						ungetc(c4,stdin);							//so return all the c4, c3, and c2 to the stream
						ungetc(c3,stdin);
						ungetc(c2,stdin);
					}
				}
				else{														//if c3 is not x, this token will not be base-2 num
					ungetc(c3,stdin);								//so return c3 and c2 to the stream
					ungetc(c2,stdin);
				}			
			}
			else if(c2=='8'){
				c3=getchar();
				if(c3=='x'){
					c4=getchar();
					if(c4=='0'||(c4>=49&&c4<=55)){		//08x0 or 08x(1~7)
						token[tokenLength] = c2;
						tokenLength++;
						token[tokenLength] = c3;
						tokenLength++;
						token[tokenLength] = c4;
						tokenLength++;
						token[tokenLength] = '\0';
						if(c4>=49&&c4<=55){					//if the first digit of the 8-base num is 1~7
							c=getchar();								//read the characters behind until a character which is not a digit 
							while(c>=48&&c<=55){				//between 0 and 7
								token[tokenLength] = c;
								tokenLength++;
								c=getchar();
							}	
							ungetc(c,stdin);
							token[tokenLength] = '\0';
						}
						return BASENUM;							
					}
					else{
						ungetc(c4,stdin);
						ungetc(c3,stdin);
						ungetc(c2,stdin);
					}				
				}
				else{
					ungetc(c3,stdin);
					ungetc(c2,stdin);
				}
			}
			else{
				ungetc(c2,stdin);
			}
		}

		//token started with 1
		else if(c=='1'){													
			c2=getchar();
			if(c2=='6'){
				token[tokenLength] = c2;			//when c2='6', attach it to the token regardless the following characters
				tokenLength++;
				token[tokenLength] = '\0';
				c3=getchar();
				if(c3=='x'){
					c4=getchar();
					if(c4=='0'||((c4>=49&&c4<=57)||(c4>=65&&c4<=70))){	//16x0 or 16x(1~F)
						token[tokenLength] = c3;
						tokenLength++;
						token[tokenLength] = c4;
						tokenLength++;
						token[tokenLength] = '\0';
						if((c4>=49&&c4<=57)||(c4>=65&&c4<=70)){	//if the first digit of the 16-base num is 1~F
							c=getchar();												//continue to read until a character not between 0 and F
							while((c>=48&&c<=57)||(c>=65&&c<=70)){
								token[tokenLength] = c;
								tokenLength++;
								c=getchar();
							}	
							ungetc(c,stdin);
							token[tokenLength] = '\0';
						}
						return BASENUM;	
					}
					else{										//if c4 is not beteen 0 and F, this is not a 16-basenum, the num will be 16
						ungetc(c4,stdin);
						ungetc(c3,stdin);
						return NUM;
					}
				}
				else{
					while(isdigit(c3)){					//if c3 is not x and is a digit, the token will be a num or a real num started with 16						
						token[tokenLength] = c3;
						tokenLength++;
						token[tokenLength] = '\0';
						c3=getchar();
					}
					ungetc(c3,stdin);
				}
			}
			else{
				while (isdigit(c2)){						//if c2 is not 6, the token might be a num or a realnum startedwith 1
					token[tokenLength] = c2;
					tokenLength++;
					token[tokenLength] = '\0';
					c2=getchar();
				}
				ungetc(c2,stdin);
			}
		}
		//c=digit except 0 and 1
		else{
			c=getchar();
			while(isdigit(c)){
				token[tokenLength] = c;
				tokenLength++;
				token[tokenLength] = '\0';
				c=getchar();
			}
			ungetc(c,stdin);
		}
		c = getchar();
		if (c == '.')
		{
			c = getchar();
			if (isdigit(c))
			{
				token[tokenLength] = '.';
				tokenLength++;
				while (isdigit(c))
				{
					token[tokenLength] = c;
					tokenLength++;
					c = getchar();
				}
				token[tokenLength] = '\0';
				if (!feof(stdin))
					ungetc(c,stdin);
				return REALNUM;
			}
			else
			{
				ungetc(c, stdin);    // note that ungetc returns characters on a stack, so we first
				c = '.';             // return the second character and set c to '.' and return c again
				ungetc(c,stdin);
				return  NUM;
			}
		}
		else
		{
			ungetc(c, stdin);
			return NUM;
		}
	}
	else
		return ERROR;	
}

int getToken()
{
	char c;
	int ttype;

	skipSpace();

	tokenLength = 0;

	c = getchar();

	switch (c)
    {	case '.': return DOT;
		case '+': return PLUS;
		case '-': return MINUS;
		case '_': return UNDERSCORE;
		case '/': return DIV;
		case '*': return MULT;
		case '=': return EQUAL;
		case ':': return COLON;
		case ',': return COMMA;
		case ';': return SEMICOLON;
		case '[': return LBRAC;
		case ']': return RBRAC;
		case '(': return LPAREN;
		case ')': return RPAREN;
		case '<':
                 c = getchar();
                 if (c == '=')
                 	 return LTEQ;
                 else if (c == '>')
					 return NOTEQUAL;

				 //code for LSHIFT
				 else if (c=='<')
					 return LSHIFT;

                 else
                 {
                     ungetc(c,stdin);
                     return LESS;
                 }

		   // Add code to handle LSHIFT

		case '>':
                 c = getchar();
                 if (c == '=')
                     return GTEQ;

				  //code for RSHIFT
				 else if (c=='>')
					 return RSHIFT;

                 else
                 {
                     ungetc(c, stdin);
                     return GREATER;
                 }

		   // Add code to handle RSHIFT

		default :
				if (isdigit(c))
				{
					ungetc(c,stdin);
					return scan_number();
				}
				else if (isalpha(c)) // token is either keyword or ID
				{
					while (isalnum(c))
					{
						token[tokenLength] = c;
						tokenLength++;
						c = getchar();
					}
					token[tokenLength] = '\0';

					ttype = isKeyword(token);
					if (ttype == 0)
					   ttype = ID;
					// c is the last character read past the token
					if (!feof(stdin))
					   ungetc(c,stdin);
					return ttype;
				}
				else if (c == EOF)
				  return EOF;
				else
				  return ERROR;
	}	// End Switch
}


main()
{
	int ttype;

	ttype = getToken();
	while (ttype != EOF)
	{
		printf("%d ",line_no);
		if ((ttype == NUM) | (ttype == ID) | (ttype == REALNUM) | (ttype == BASENUM))
			printf("%s \n", token);
		else
			printToken(ttype);
		ttype = getToken();
	}
}