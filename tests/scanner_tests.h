#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include "../include/read_input.h"
#include "../include/token.h"
#include "../include/scanner.h"

using namespace std;
namespace scanner = fcal::scanner;
class ScannerTestSuite : public CxxTest::TestSuite
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    scanner::Scanner *s;
    void test_setup_code () {
      s = new scanner::Scanner();
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You should test the regular expressions you use to make and match
       regular expressions,and the functions used by makeRegex and
       matchRegex regex_tests.h.
       However, You will likely need to write several tests to
       adequately test  the functions that are called from "scan".
       Once you are confident that the components used by "scan"
       work properly, then you can run tests on the "scan" method itself.
    */

    // You should place tests for these other functions used by
    // scan below.


    /* Below is one of the tests for these components in the project
       solution created by your instructor.  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly () {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{");
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */



    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (scanner::Token *tks) {
        scanner::Token *currentToken = tks;
        while (currentToken != NULL) {
          if (currentToken->terminal() == scanner::kLexicalError) {
              printf("problem: %s\n",currentToken->lexeme().c_str());
              fflush(stdout);
                                return false;
            }
            else {
              currentToken = currentToken->next();
            }
        }
        return true;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned.
    */
    void scanFileNoLexicalErrors(const char* filename) {
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT (text);
        scanner::Token *tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        TS_ASSERT(noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
  bool sameTerminals(scanner::Token *tks, int numTerms,
                     scanner::TokenType *ts) {
        scanner::Token *currentToken = tks;
        int termIndex = 0;
        while(currentToken != NULL && termIndex < numTerms) {
			
            if(currentToken->terminal() != ts[termIndex]) {
                printf("item at index: %i: terminal: %i should be terminal: %i\n",termIndex,currentToken->terminal(), ts[termIndex]);
				fflush(stdout);
                return false;
            }
            else {
                ++ termIndex;
                currentToken = currentToken->next();
            }
        }
        return true;
    }


    void test_simple_terminal_IntKwd() {
        scanner::Token *tks = s->Scan("int");
        TS_ASSERT(tks != NULL);
    }

    void test_terminal_IntKwd() {
        scanner::Token *tks = s->Scan("int");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {scanner::kIntKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    
    void test_terminal_floatKwd() {
        scanner::Token *tks = s->Scan("float ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kFloatKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }
    
    void test_terminal_stringKwd() {
        scanner::Token *tks = s->Scan("string ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kStringKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }    

    void test_terminal_matrixKwd() {
        scanner::Token *tks = s->Scan("matrix ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kMatrixKwd };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_letKwd() {
        scanner::Token *tks = s->Scan("let ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kLetKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_inKwd() {
        scanner::Token *tks = s->Scan("in ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kInKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_endKwd() {
        scanner::Token *tks = s->Scan("end ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kEndKwd };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_ifKwd() {
        scanner::Token *tks = s->Scan("if ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kIfKwd };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }


    void test_terminal_thenKwd() {
        scanner::Token *tks = s->Scan("then ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kThenKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }
    
    void test_terminal_elseKwd() {
        scanner::Token *tks = s->Scan("else ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kElseKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }   
    
    void test_terminal_repeatKwd() {
        scanner::Token *tks = s->Scan("repeat ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kRepeatKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_printKwd() {
        scanner::Token *tks = s->Scan("print ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kPrintKwd };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_tokwd() {
        scanner::Token *tks = s->Scan("to ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kToKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_IntConst() {
        scanner::Token *tks = s->Scan("22 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kIntConst };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_floatConst() {
        scanner::Token *tks = s->Scan("2.035 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kFloatConst };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }    
    
    void test_terminal_stringConst() {
        scanner::Token *tks = s->Scan("\"stringfoo\".");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kStringConst };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }    
   
    void test_terminal_variableName() {
        scanner::Token *tks = s->Scan("foobar");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kVariableName };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_LeftParen() {
        scanner::Token *tks = s->Scan("( ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kLeftParen};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_rightParen() {
        scanner::Token *tks = s->Scan(") ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kRightParen};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_leftCurly() {
        scanner::Token *tks = s->Scan("{ ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kLeftCurly};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_rightCurly() {
        scanner::Token *tks = s->Scan("} ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kRightCurly};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_LeftSquare() {
        scanner::Token *tks = s->Scan("[ ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kLeftSquare };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_RightSquare() {
        scanner::Token *tks = s->Scan("] ");
        TS_ASSERT(tks != NULL); 
        scanner::TokenType ts[] = { scanner::kRightSquare };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_semiColon() {
        scanner::Token *tks = s->Scan("; ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kSemiColon };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_Colon() {
        scanner::Token *tks = s->Scan(": ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kColon };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_Assign() {
        scanner::Token *tks = s->Scan("= ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kAssign };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_kPlusSign() {
        scanner::Token *tks = s->Scan("+ ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kPlusSign };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_Star() {
        scanner::Token *tks = s->Scan("* ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kStar };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_dash() {
        scanner::Token *tks = s->Scan("^ ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kDash };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_forwardSlash() {
        scanner::Token *tks = s->Scan("\\/ ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kForwardSlash };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_LessThan() {
        scanner::Token *tks = s->Scan("< ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kLessThan };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_lessThanEqual() {
        scanner::Token *tks = s->Scan("<= ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kLessThanEqual };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

    void test_terminal_greaterThan() {
        scanner::Token *tks = s->Scan("> ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kGreaterThan};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }


    void test_terminal_equalsEquals() {
        scanner::Token *tks = s->Scan(">= ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kEqualsEquals };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }
    
      void test_terminal_ntEquals() {
        scanner::Token *tks = s->Scan("!= ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kNotEquals};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }  
    
      void test_terminal_andOp() {
        scanner::Token *tks = s->Scan("&& ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kAndOp};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }   
    
      void test_terminal_orOp() {
        scanner::Token *tks = s->Scan("|| ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kOrOp };
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }   
     
       void test_terminal_notOp() {
        scanner::Token *tks = s->Scan("!");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kNotOp};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

       void test_terminal_bool() {
        scanner::Token *tks = s->Scan("bool ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kBoolKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

       void test_terminal_true() {
        scanner::Token *tks = s->Scan("true ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kTrueKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

       void test_terminal_false() {
        scanner::Token *tks = s->Scan("false ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kFalseKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

        void test_terminal_while() {
        scanner::Token *tks = s->Scan("while ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kWhileKwd};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }




    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty() {
        scanner::Token *tks = s->Scan("  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    void test_scan_empty_comment() {
        scanner::Token *tks = s->Scan(" /* a comment */ ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    // When a lexical error occurs, the scanner creates a token with a
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors() {
        scanner::Token *tks = s->Scan("$&1  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "$");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "&");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kIntConst);
        TS_ASSERT_EQUALS(tks->lexeme(), "1");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }


    // A test for scanning numbers and a variable.

    void test_scan_nums_vars() {
        scanner::Token *tks = s->Scan(" 123 x 12.34 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kIntConst, scanner::kVariableName,
                                           scanner::kFloatConst, scanner::kEndOfFile };
        TS_ASSERT( sameTerminals( tks, 4, ts));
    }


    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexemes
       are correct.
     */

    void test_scan_bad_syntax_good_tokens() {
        const char *filename = "./samples/bad_syntax_good_tokens.dsl";
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = NULL;
        tks = s->Scan( text);
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {
          scanner::kIntConst, scanner::kIntConst, scanner::kIntConst, scanner::kIntConst,
          scanner::kStringConst, scanner::kStringConst, scanner::kStringConst,
          scanner::kFloatConst, scanner::kFloatConst, scanner::kFloatConst,
          scanner::kMatrixKwd,
          scanner::kSemiColon, scanner::kColon, scanner::kToKwd,
          scanner::kLeftCurly, scanner::kLeftParen, scanner::kRightCurly, scanner::kRightParen,
          scanner::kPlusSign, scanner::kStar, scanner::kDash, scanner::kForwardSlash,
          scanner::kEqualsEquals, scanner::kLessThanEqual,
          scanner::kGreaterThanEqual, scanner::kNotEquals,
          scanner::kAssign,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kIntKwd, scanner::kFloatKwd, scanner::kStringKwd,
          scanner::kEndOfFile
        };
        int count = 38;
        TS_ASSERT( sameTerminals( tks, count, ts));
    }

    void test_scan_sample_forestLoss() {
        scanFileNoLexicalErrors("./samples/forest_loss_v2.dsl");
    }

	void test_terminal_eof() {
        scanner::Token *tks = s->Scan("\\z");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kEndOfFile};
        TS_ASSERT( sameTerminals( tks, 0, ts));
    }

};
