/* lab3.c
 * Tolga
 * Bilgis
 * ECE 2220, Fall 2025
 * PP3
 * Subject: ECE2220,PP3
 *
 * Purpose:  Program that takes 2 strings as operands and an operator as input and outputs a new strings where it performs that operation for each index
 *
 * Assumptions:
 *  #1   The user is prompted to enter a pseudo arithmetic command.  The
 *       input must be verified to be grammatically correct.
 *
 *  #2:  The string and character type library cannot be used under
 *       any circumstances.  You are encouraged to develop your own
 *       functions to perform any similar operations that are needed.
 *
 *  #3   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs: N/A
 *
 * Notes: N/A
 *
 * See the ECE 2220 programming guide
 *
 * If your formatting is not consistent you must fix it.  You can easily
 * reformat (and automatically indent) your code using the astyle
 * command.  In a terminal on the command line do
 *
 *     astyle --style=kr lab3.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up
 * your own style.
 */

// do not include any additional libraries
#include <stdio.h>

// do not change these constants
#define MAXLINE 80
#define MAXOPER 13

// named constants and strings
enum operations {NOOP, ADD, MUL, DIV, POW};
const char *operation_str[] = {"Invalid", "+", "*", "/", "^"};

// function prototypes
int process_input(const char *input, char *op_left, char *op_right);
void calc_output(const char *op_l, int op, const char *op_r, char *result);

// do not change any code in main.  We are grading based on the format
// of the output as given in the printfs in main.
int main() {
    char input_line[MAXLINE];
    char left_operand[MAXOPER];
    char right_operand[MAXOPER];
    char answer[MAXOPER];
    int  operator;

    printf("\nPP3: Arithmetic on GF(47) with + * / ^ using letters\n");
    printf("Commands:\n\tabc+bbc\n\tturtle/frog\n\ttiger^one");
    printf("\tOperands are no more than 12 letters and no spaces\n");
    printf("\tCtrl-d to quit\n\n");
    printf("> ");

    // each call to fgets collects one line of input and stores in input_line
    // BEWARE: fgets includes the end-of-line character '\n' in input_line
    while (fgets(input_line, sizeof input_line, stdin) != NULL) {

        // clear for next round
        left_operand[0] = right_operand[0] = answer[0] = '\0';

        // check for valid grammar
        operator = process_input(input_line, left_operand, right_operand);

        if (operator == ADD || operator == MUL
                || operator == DIV || operator == POW) {

            // print parsed input
            printf("'%s'", left_operand);
            printf(" %s ", operation_str[operator]);
            printf("'%s' => ", right_operand);

            // perform pseudo arithmetic
            calc_output(left_operand, operator, right_operand, answer);

            // print result
            printf("'%s'\n\n", answer);
        } else {
            printf("# %s", input_line);
        }
        printf("> ");
    }
    printf("\nGoodbye\n");
    return 0;
}

/* Parse input of the form SOS where S is a string (i.e., the operand) 
 * and O (i.e., the operator) is a character.
 *
 * A string S must consist of up to 12 valid symbols a-z and A-U.
 * The operator O must be one character from: + * / ^
 * Any other characters found in the input, including spaces, are
 * grammatically incorrect and invalidate the input.
 *
 * There must be NO spaces anywhere in the input, including between
 * either SO, OS, or leading or trailing spaces.
 *
 * Input: The input string is collected using fgets.  Recall the end-of-line
 *        character is included in the input string and marks the end of
 *        the input.  This string must not be changed.
 *
 * Output: There are three outputs from this function.
 *
 *   The return value is one of NOOP, ADD, MUL, DIV, POW which are
 *      named constants.  If the input is invalid for any reason
 *      then the output must be NOOP.  Otherwise the return value
 *      corresponds to operand O.
 *
 *   If the input is grammatically correct, then two strings are also
 *   returned, one for each of the left and right operands.  If the input
 *   in invalid the two output strings are undefined.
 */
int process_input(const char *input, char *op_left, char *op_right) {
    // replace the following with your code
    int l = 0;
    const char *cur = input;
    int phase = 1;
    enum operations op = NOOP;

    while (*cur != '\n' && *cur != '\0') {
        if (phase == 1) {
            if ((*cur >= 'a' && *cur <= 'z') || (*cur >= 'A' && *cur <= 'U')) {
                if (l >= MAXOPER - 1) return NOOP;
                op_left[l] = *cur;
            } else if (*cur == '+') {
                op = ADD;
                phase = 2;
                op_left[l] = '\0';
                l = -1;
            } else if (*cur == '*') {
                op = MUL;
                phase = 2;
                op_left[l] = '\0';
                l = -1;
            } else if (*cur == '/') {
                op = DIV;
                phase = 2;
                op_left[l] = '\0';
                l = -1;
            } else if (*cur == '^') {
                op = POW;
                phase = 2;
                op_left[l] = '\0';
                l = -1;
            } else {
                return NOOP;
            }
        } else if (phase == 2) {
            if ((*cur >= 'a' && *cur <= 'z') || (*cur >= 'A' && *cur <= 'U')) {
                if (l >= MAXOPER - 1) return NOOP;
                op_right[l] = *cur;
            } else {
                return NOOP;
            }
        }
        l += 1;
        cur += 1;
    }

    if (phase == 2) op_right[l] = '\0';

    if (op_left[0] == '\0' || op_right[0] == '\0') return NOOP;

    return op;
}

/* Pseudo mathematical opertions on the two operands work as follows.
 *
 * Each character is converted to an integer in the range 1...46, where a is 0,
 * b is 1, c is 2, ..., z is 25.  The operation is then performed using
 * math on a finite field with no carries.
 *
 * If the two input strings are not the same length, then each output character
 * beyond the length of the shorter string should be a copy of the character
 * from the longer string but with the opposite case.
 *
 * Input: The two operand strings and the operator are assumed to be valid (and
 *        are verified as valid in the parse_input function).
 *
 * Output: The final string generated by the above rules is stored in the
 *         output string named result.  The input strings must not be
 *         changed.
 */
void calc_output(const char *l_op, int op, const char *r_op, char *result) {
 int l_l = 0;
    int l_r = 0;
    int l, i;
    while (l_op[l_l] != '\0') {
        l_l += 1;
    }
    while (r_op[l_r] != '\0') {
        l_r += 1;
    }

    if (l_r < l_l) {
        l = l_r;
    } else {
        l = l_l;
    }
    char l_op1[l];
    char r_op1[l];
    for (i = 0; i < l; i++) {
        if (l_op[i] >= 'a' && l_op[i] <= 'z') {
            l_op1[i] = (char)(l_op[i] - 'a');
        } else {
            l_op1[i] = (char)(l_op[i] - 'A' + 26);
        }

        if (r_op[i] >= 'a' && r_op[i] <= 'z') {
            r_op1[i] = (char)(r_op[i] - 'a');
        } else {
            r_op1[i] = (char)(r_op[i] - 'A' + 26);
        }
    }
    l_op1[i] = '\0';
    r_op1[i] = '\0';
    for (i = 0; i < l; i++) {
        int a = (int)l_op1[i];
        int b = (int)r_op1[i];
        int v = 0;
        if (op == ADD) {
            v = a + b;
            v = v % 47;
            if (v < 0) v += 47;
        }
        if (op == MUL) {
            v = (a * b) % 47;
            if (v < 0) v += 47;
        }
        if (op == DIV) {
            if ((b % 47) == 0) {
                v = 0;
            } else {
                int base = b % 47;
                if (base < 0) base += 47;
                int exp = 45;
                int inv = 1;
                while (exp > 0) {
                    if (exp & 1) {
                        inv = (inv * base) % 47;
                    }
                    base = (base * base) % 47;
                    exp >>= 1;
                }
                v = (a * inv) % 47;
                if (v < 0) v += 47;
            }
        }
        if (op == POW) {
            if (b == 0) {
                v = 1;
            } else {
                int base = a % 47;
                if (base < 0) base += 47;
                int exp = b;
                int res = 1;
                while (exp > 0) {
                    if (exp & 1) {
                        res = (res * base) % 47;
                    }
                    base = (base * base) % 47;
                    exp >>= 1;
                }
                v = res % 47;
                if (v < 0) v += 47;
            }
        }
        if (v <= 25) {
            result[i] = (char)('a' + v);
        } else {
            result[i] = (char)('A' + (v - 26));
        }
    }
    if (l_l > l_r) {
        int k = l_r;
        while (k < l_l) {
            char c = l_op[k];
            if (c >= 'a' && c <= 'z') {
                c = (char)(c - 'a' + 'A');
            } else {
                c = (char)(c - 'A' + 'a');
            }
            result[k] = c;
            k++;
        }
        result[l_l] = '\0';
    } else if (l_r > l_l) {
        int k = l_l;
        while (k < l_r) {
            char c = r_op[k];
            if (c >= 'a' && c <= 'z') {
                c = (char)(c - 'a' + 'A');
            } else {
                c = (char)(c - 'A' + 'a');
            }
            result[k] = c;
            k++;
        }
        result[l_r] = '\0';
    } else {
        result[l] = '\0';
    }
}
