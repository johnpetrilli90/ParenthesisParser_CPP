// ParenthesisParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: John Petrilli

#include "pch.h"
#include <iostream>
#include <string>
#include <cassert> 

using namespace std;

//Maximum size of fixed array 
const int MAX = 80;
//Ascii value of the 4 types of parenthesis 
const char  SmoothOpen = '(',
			SmoothClose = ')', 
			SquareOpen = '[', 
			SquareClose = ']', 
			CurlyOpen = '{',
			CurlyClose = '}', 
			AngleOpen = '<',
			AngleClose = '>';

//Open Parenthesis Class; If an open parenthesis
//appears, it will be added to this stack 
template<class T>
class OpenParen {
public:
	// create empty stack 
	OpenParen(){
		n = 0;
	}
	// push onto stack 
	void push(T x){
		assert(!full()); 
		Stack[n] = x; 
		n++;
	}
	// pop top element off stack 
	void pop(){
		assert(!empty());
		n--;
	}
	// look at top element 
	T peek(){
		assert(!empty());
		return (Stack[n-1]);
	}
	//return size of stack 
	int size(){
		return n;
	}
	// checks for empty stack 
	bool empty(){
		return (n == 0);
	}
	// checks for full stack 
	bool full(){
		return (n == MAX);
	}
	// clears values in stack void 
	void clear(){
		n = 0;
		assert(empty());
	}

private:
	T Stack[MAX] = { 0 };
	int n;
};

// This function tracks where a parenthesis error occurs
// and puts a carrot at the mistake(s)
void track(string& str, int j, int k) {
	string tracker;

	for (unsigned int i = 0; i < str.length(); i++) 
	{
		if (i == j || i == k)
			tracker.append("^");
		else
			tracker.append(" ");
	}
	cout << tracker << endl;
}

// This function loops through the user inputted string
// char by char and determines whether or not it’s a
// parenthesis and what type 
void Parse(string& s)
{
	OpenParen<char> CheckPar;	// Instance of OpenParen, checks string for parenthesis
	OpenParen<int> column;	// Keeps track of parenthesis location on string
	int err = 0;	// Tracks any parse errors that occur
	bool Mismatched = false;	// Keeps track of a mismatch error
	int StringLen = s.length();

	// looping through string by each char 
	for(unsigned int i=0; i<StringLen; i++)
	{
		// check for open paren, push onto stack if open paren 
		if(s[i]==SmoothOpen || s[i]==CurlyOpen ||s[i] == AngleOpen || s[i] == SquareOpen)
		{ 
			CheckPar.push(s[i]); 
			column.push(i);
		}
		// check for closed paren; If empty stack throw error message 
		else if(s[i]==SmoothClose || s[i]==CurlyClose || s[i] == AngleClose || s[i] == SquareClose)
		{ 
			if (CheckPar.empty())
			{
				track(s, i, -1);
				cout << "Unexpected Closed Paren at Column " << (i + 1) << endl;
				err++;
			}
			// checks for matching paren; if match pop open paren off stack
			// if doesn’t match send it to tracker function and set Mismatched = true
			// check for all four cases of parenthesis
			else {
				switch (s[i]) {
				case SmoothClose:
					if (CheckPar.peek() == SmoothOpen)
					{
						CheckPar.pop();
						column.pop();
					}
					else if (CheckPar.peek() == SquareOpen || CheckPar.peek() == AngleOpen || CheckPar.peek() == CurlyOpen)
					{
						track(s, i, column.peek());
						cout << "Mismatched Parenthesis, column " << column.peek() + 1 << " & " << (i + 1) << endl;
						Mismatched = true;
						err++;
					}
					break;
				case CurlyClose:
					if (CheckPar.peek() == CurlyOpen)
					{
						CheckPar.pop();
						column.pop();
					}
					else if (CheckPar.peek() == SquareOpen || CheckPar.peek() == AngleOpen || CheckPar.peek() == SmoothOpen) {
						track(s, i, column.peek());
						cout << "Mismatched Parenthesis, column " << column.peek() + 1 << " & " << (i + 1) << endl;
						Mismatched = true;
						err++;
					}
					break;
				case AngleClose:
					if (CheckPar.peek() == AngleOpen)
					{
						CheckPar.pop();
						column.pop();
					}
					else if (CheckPar.peek() == SquareOpen || CheckPar.peek() == SmoothOpen || CheckPar.peek() == CurlyOpen) {
						track(s, i, column.peek());
						cout << "Mismatched Parenthesis, column " << column.peek() + 1 << " & " << (i + 1) << endl;
						Mismatched = true;
						err++;
					}
					break;
				case SquareClose:
					if (CheckPar.peek() == SquareOpen)
					{
						CheckPar.pop();
						column.pop();
					}
					else if (CheckPar.peek() == SmoothOpen || CheckPar.peek() == AngleOpen || CheckPar.peek() == CurlyOpen) {
						track(s, i, column.peek());
						cout << "Mismatched Parenthesis, column " << column.peek() + 1 << " & " << (i + 1) << endl;
						Mismatched = true;
						err++;
					}
					break;
				}
			}
		}
	}
	// after loop check for paren on stack, if !empty: unmatched parenthesis 
	if (!CheckPar.empty() && !Mismatched)
	{
		track(s, column.peek(),-1);
		cout << "Unmatched Open Parenthesis at column " << column.peek() + 1 << endl << endl;
	}
	else if (!err) {
		cout << "No Parenthesization Errors. " << endl << endl;
	}
}

// Main function; User inputs string and string is passed to parsing function
int main() {
	string Input;
	cout << "Enter a string to parse, or a blank line to quit:" << endl; 
	while (cin.peek() != '\n') {
		getline(cin, Input);
		Parse(Input);
		cout << "Enter a string to parse, or a blank line to quit:" << endl;
	}

	return 0;
}

