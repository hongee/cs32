#include <string>
#include <stack>
#include <cctype>
#include <cassert>
#include <iostream>
#include "Map.h"

using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result);
  // Evaluates an integer arithmetic expression
  // Precondition: infix is an infix integer arithmetic
  //   expression consisting of single lower case letter operands,
  //   parentheses, and the operators +, -, *, /, with embedded blanks
  //   allowed for readability.
  // Postcondition: If infix is a syntactically valid infix integer
  //   expression whose only operands are single lower case letters
  //   (whether or not they appear in the values map), then postfix is
  //   set to the postfix form of the expression; otherwise postfix may
  //   or may not be changed, result is unchanged, and the function
  //   returns 1.  If infix is syntactically valid but contains at
  //   least one lower case letter operand that does not appear in the
  //   values map, then result is unchanged and the function returns 2.
  //   If infix is syntactically valid and all its lower case operand
  //   letters appear in the values map, then if evaluating the
  //   expression (using for each letter in the expression the value in
  //   the map that corresponds to it) attempts to divide by zero, then
  //   result is unchanged and the function returns 3; otherwise,
  //   result is set to the value of the expression and the function
  //   returns 0.

int convert_precedence(const char& ch);

bool check_precedence(const char& top, const char& current);

int convert_infix(const string& infix, const Map& values, string& postfix);


int main() {
  assert(check_precedence('*', '-'));
  assert(!check_precedence('+', '/'));

  char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
  int  vals[] = {  3,  -9,   6,   2,   4,   1  };
  Map m;
  for (int k = 0; vars[k] != '#'; k++)
  m.insert(vars[k], vals[k]);
  string pf;
  int answer;

  assert(evaluate("a+ e", m, pf, answer) == 0 &&
                          pf == "ae+"  &&  answer == -6);
  answer = 999;
  assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
  assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
  assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
  assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
  assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
  assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
  assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
  assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
  assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
  assert(evaluate("a*b", m, pf, answer) == 2  &&
                          pf == "ab*"  &&  answer == 999);
  assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
                          pf == "yoau-*+"  &&  answer == -1);
  answer = 999;
  assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                                pf == "oyy-/"  &&  answer == 999);
  assert(evaluate(" a  ", m, pf, answer) == 0  &&
                          pf == "a"  &&  answer == 3);
  assert(evaluate("((a))", m, pf, answer) == 0  &&
                          pf == "a"  &&  answer == 3);
  cout << "Passed all tests" << endl;
}

int convert_precedence(const char& ch) {
  switch(ch) {
    case '-':
      return 1;
      break;
    case '+':
      return 2;
      break;
    case '*':
      return 3;
      break;
    case '/':
      return 4;
      break;
  }
}

bool check_precedence(const char& top, const char& current) {
  int top_p = convert_precedence(top);
  int curr_p = convert_precedence(current);
  return top_p >= curr_p;
}

int convert_infix(const string& infix, const Map& values, string& postfix) {

  const int OPERAND = 1;
  const int OPERATOR = 2;
  const int OPENPAREN = 3;
  const int CLOSEPAREN = 4;

  stack<char> operatorStack;

  int numOpenParenthesis = 0;
  int typePrevious = 0;
  int returnVal = 0;
  postfix = "";

  for(int i=0;i<infix.size();i++) {
    const char& ch = infix[i];
    //cerr << ch << " | ";

    if(islower(ch)) //some letter
    {
      //does not exist in map
      if(!values.contains(ch)) returnVal = 2;
      //letter cant have a previous letter, and can't come after a close paren (multiply needs to be explicit)
      if(typePrevious == OPERAND || typePrevious == CLOSEPAREN) return 1;

      postfix += ch;
      typePrevious = OPERAND;
    }
    else if(ch == '(')
    {
      //open paren can't be preced by operand/close paren since multiplication needs to be explicit
      if(typePrevious == OPERAND || typePrevious == CLOSEPAREN) return 1;

      operatorStack.push(ch);
      numOpenParenthesis++;
      typePrevious = OPENPAREN;
    }
    else if(ch == ')')
    {
      //no empty brackets/no dangling operators/cant be first
      if(typePrevious == OPENPAREN || typePrevious == OPERATOR || typePrevious == 0) return 1;

      if(numOpenParenthesis <= 0) { //attempting to close parenthesis without an open parenthesis!
        return 1;
      } else {
        numOpenParenthesis--;
      }

      //remove operators
      while(!operatorStack.empty() && operatorStack.top() != '(')
      {
        postfix += operatorStack.top();
        operatorStack.pop();
      }
      operatorStack.pop();
      typePrevious = CLOSEPAREN;
    }
    else
    {
      switch(ch) {
        case '+':
        case '-':
        case '*':
        case '/':
          //can't be first, cant be first in parenthesis, cant have double operators
          if(typePrevious == 0 || typePrevious == OPENPAREN || typePrevious == OPERATOR) return 1;

          while(!operatorStack.empty() && operatorStack.top() != '(' && check_precedence(operatorStack.top(),ch))
          {
            postfix += operatorStack.top();
            operatorStack.pop();
          }
          operatorStack.push(ch);
          typePrevious = OPERATOR;
          break;
        case ' ':
          continue;
          break;
        default:
          return 1;
      }
    }
  }
  //last checks - if previous is an operator, an open paren, is completely blank, or there's still dangling parenthesis
  if(typePrevious == OPERATOR || typePrevious == OPENPAREN || typePrevious == 0 || numOpenParenthesis != 0) return 1;

  //finish the stack
  while(!operatorStack.empty()) {
    postfix += operatorStack.top();
    operatorStack.pop();
  }
  return returnVal;
}

int evaluate(string infix, const Map& values, string& postfix, int& result) {
  int convertResult = convert_infix(infix, values, postfix);
  if(convertResult != 0) return convertResult;

  stack<int> operandStack;
  for(int i = 0; i < postfix.length(); i++)
  {
    const char& ch = postfix[i];

    if(islower(ch))
    {
      //convert variable to the number
      int v;
      values.get(ch, v);
      operandStack.push(v);
    }
    else
    {
      int operand2 = operandStack.top();
      operandStack.pop();
      int operand1 = operandStack.top();
      operandStack.pop();
      int result = 0;
      switch (ch) {
        case '+':
          result = operand1 + operand2;
          break;
        case '-':
          result = operand1 - operand2;
          break;
        case '*':
          result = operand1 * operand2;
          break;
        case '/':
          if(operand2 == 0) return 3;
          result = operand1 / operand2;
          break;
      }
      operandStack.push(result);
    }
  }
  result = operandStack.top();
  operandStack.pop();
  return 0;
}
