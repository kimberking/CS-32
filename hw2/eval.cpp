#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

string removeSpace(string infix)   // removes the spaces
{
	string a = "";
	for (int j = 0; j < infix.length(); j++)
		if (infix[j] != ' ')
			a = a + infix[j];
	return a;
}

bool isValid(string infix)
{
	string infix2 = removeSpace(infix);    // Make sure there are no spaces

	for (int j = 0; j < infix2.length(); j++) // Loop through the infix without spaces
	{
		if ( (infix2[infix2.length() - 1] == '|') || (infix2[infix2.length() - 1] == '&') )  // check special case																										
			return false;

		if ( (infix2[j] == '&') || (infix2[j] == '|'))  // order
															
		{
			if ((infix2[j - 1] != 'T' && infix2[j - 1] != 'F' && infix2[j - 1] != ')') || ((infix2[j + 1]) != 'T' && infix2[j + 1] != 'F' && infix2[j + 1] != '!'
				&& infix2[j + 1] != '('))
				return false;
		}
		else if ( ( infix2[j] == 'F') || (infix2[j] == 'T') ) // cant have t f and t f
		{
			if ( (infix2[j + 1] == 'T') || (infix2[j + 1] == 'F') || (infix2[j + 1] == '(' )|| ( infix2[j + 1] == '!') )
				return false;
		}
	}
	return true;
}

int Order(char letter)
{
	if (letter == '!') {
		return 2;
	}

	else if (letter == '&') {
		return 1;
	}
	else if (letter == '|') {
		return 0;
	}
	else {
		return -1;
	}
}

string infixToPostfix(string infix, string& postfix)
{
	postfix = "";   

	stack<char> aStack;  // create a stack

	for (int j = 0; j < infix.length(); j++) {

		char letter = infix[j];
	
		switch (letter) {
	
		case 'T':       // if t or f
			postfix = postfix + letter;
			break;
	
		case 'F':
			postfix = postfix + letter;
			break;
	
		case '(':       // if (
			aStack.push(letter);
			break;
	
		case ')':
			while (aStack.top() != '(')  // if it isnt (
			{
				postfix = postfix +  aStack.top();
				aStack.pop();
			}
			aStack.pop();    // pop (
			break;
		
		case '!':  
			while (!aStack.empty() && aStack.top() != '(' &&
				Order(letter) <= Order(aStack.top()))
			{
				postfix = postfix + aStack.top();
				aStack.pop();
			}
			aStack.push(letter); // push onto stack
			break;
		
		case '&':
			while (!aStack.empty() && aStack.top() != '(' &&
				Order(letter) <= Order(aStack.top()))
			{
				postfix = postfix + aStack.top();
				aStack.pop();
			}
			aStack.push(letter); // push onto stack
			break;
		
		case '|':
			while (!aStack.empty() && aStack.top() != '(' &&
				Order(letter) <= Order(aStack.top()))
			{
				postfix = postfix+ aStack.top();
				aStack.pop();
			}
			aStack.push(letter); // push onto stack
			break;

		default:
			break;
		}
	}

	while (!aStack.empty())  // when it is finally empty
	{
		postfix = postfix + aStack.top();
		aStack.pop();
	}
	return postfix;
}

int evaluate(string infix, string& postfix, bool& result)
{
	if (isValid(infix)) //if it is valid
	{
		postfix = infixToPostfix(infix, postfix);    // convert to postfix
		if (postfix.empty())        // if it is empty
			return 1;

		stack <bool> myStack;   // create a stack
		for (int j = 0; j < postfix.length(); j++)
		{
			char letter = postfix[j];

			if (letter == 'T')      // true so push
				myStack.push(true);

			else if (letter == 'F')   // false so push
				myStack.push(false);

			else if (letter == '!')
			{
				bool temp = myStack.top(); // store at the top and push back the opposite of it
				myStack.pop();
				myStack.push(!temp);
			}

			else
			{
				bool operand2 = myStack.top();
				myStack.pop();
				if (myStack.empty())   // check )
					return 1;

				bool operand1 = myStack.top();
				myStack.pop();

				if (postfix[j] == '|')  // if we see an or 
				{
					if (operand1 || operand2)   // evalute the top two
						myStack.push(true);
					else
						myStack.push(false);
				}

				if (postfix[j] == '&')  // if we see an &
				{
					if (operand1 && operand2)   // same as the ||
						myStack.push(true);
					else
						myStack.push(false);
				}
			}
		}

		result = myStack.top();    // end
	
		return 0;
	}
	return 1;
}

int main()
{
	string pf;
	bool answer;
	assert(evaluate("T| F", pf, answer) == 0 && pf == "TF|"  &&  answer);
	assert(evaluate("T|", pf, answer) == 1);
	assert(evaluate("F F", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("()", pf, answer) == 1);
	assert(evaluate("T(F|T)", pf, answer) == 1);
	assert(evaluate("T(&T)", pf, answer) == 1);
	assert(evaluate("(T&(F|F)", pf, answer) == 1);
	assert(evaluate("", pf, answer) == 1);
	assert(evaluate("F  |  !F & (T&F) ", pf, answer) == 0
		&& pf == "FF!TF&&|" && !answer);
	assert(evaluate(" F  ", pf, answer) == 0 && pf == "F" && !answer);
	assert(evaluate("((T))", pf, answer) == 0 && pf == "T"  &&  answer);
	cout << "Passed all tests" << endl;
}