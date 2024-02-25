// AZFAR NAYYAN.
// 22L-7886. BSE-3C.
// DATA STRUCTURES ASSIGNMENT NO 02.
// STACKS.

// QUESTION NUMBER 01(ONE).

#include<iostream>
#include<string>
#include<fstream>

using namespace std;

template<class T>
class Stack {            // linked list based class for stack.

	struct Node {        // Nested Node for Linked List.
		T data;
		Node* next;
	public:
		Node() { next = NULL; }
		Node(T element) {
			data = element;
			next = NULL;
		}
	};

	Node* Top;
	int s;

public:
	Stack()   //Default Constructor.
	{
		Top = NULL;
		s = 0;
	}
	~Stack()   //Default Destructor.
	{
		while (Top)
		{
			Node* temp = Top;
			Top = Top->next;
			delete temp;
		}
	}
	bool IsEmpty()   //BOOL function to check if it's empty.
	{
		bool flag = false;
		if (Top == NULL) flag = true;

		return flag;
	}
	bool top(T& ele)  //Function to copy data at the TOP.
	{
		if (Top == NULL)
		{
			return false;
		}
		else if (Top != NULL)
		{
			ele = Top->data;
			return true;
		}
	}
	bool top1(T& ele)
	{
		if (Top == NULL)
		{
			return false;
		}
		else if (Top != NULL)
		{
			if (Top->next != NULL)
			{
				ele = Top->next->data;
				return true;
			}
			else return false;
		}
	}
	
	bool pop()      //POP'S the Top element of the stack.
	{
		if (Top == NULL)
		{
			return false;
		}
		else if (Top != NULL)
		{
			Node* temp = Top;
			if (Top->next == NULL)
			{
				Top = NULL;
			}
			else
			{
				Top = Top->next;
			}
			delete temp;
			return true;
		}
	}
	void push(T const& e)  //Push the element at the Top of the Stack.
	{
		if (Top == NULL)
		{
			Top = new Node(e);
		}
		else
		{
			Node* insert = new Node(e);
			insert->next = Top;
			Top = insert;
		}
	}
};

class XMLData                                       //XML DATA CLASS.
{
	string** inputArray;                            //I USED STACK INSIDE THE XML CLASS.
	int lines;
	Stack<string> Tags;
	Stack<string> Quotes;
	Stack<string> elementContentTagsNames;
	Stack<string> textContentTagsNames;

public:
	XMLData()                                             //CONSTRUCTOR.
	{
		lines = 0;
		inputArray = NULL;
	}
	void Input()                                        //GIVES TWO INPUT OPTIONS.
	{
		cout << "Chose Input Method: (1) File Input. (2) User Input.  " << endl;
		int Input;
		cin >> Input;

		if (Input != 1 && Input != 2)
		{
			do {
				cout << "Chose either (1) or (2) :" << endl;
				cin >> Input;
			} while (Input != 1 && Input != 2);
		}
		system("cls");

		if (Input == 1) FileInput();
		else if (Input == 2) UserInput();

	}
	void UserInput()                               //USER INPUT AS XML DATA.
	{
		cout << "User Input !" << endl;
		cout << "Enter the number of lines: ";
		cin >> lines;
		if (lines <= 0)
		{
			do {
				cout << "Enter greater than zero: ";
				cin >> lines;
			} while (lines <= 0);
		}
		cin.ignore(256, '\n');

		inputArray = new string * [lines];
		for (int i = 0; i < lines; i++) inputArray[i] = new string[1];

		for (int i = 0; i < lines; i++)
		{
			cout << "Enter Line " << i + 1 << " :";
			getline(cin, inputArray[i][0]);
		}
		system("cls");
	}
	void FileInput()                      //READS A TEXT FILE AS INPUT XML DATA.
	{
		cout << "File Input !" << endl;
		ifstream fr, read;
		fr.open("XMLData.txt");
		if (!fr.is_open())
		{
			cout << "FAILED TO OPEN XMLData.txt FILE ! " << endl;
			return;
		}

		string l;
		int L = 0;

		while (!fr.eof())
		{
			getline(fr, l);
			L++;
		}
		fr.close();

		lines = L;
		inputArray = new string * [lines];
		for (int i = 0; i < lines; i++) inputArray[i] = new string[1];

		int index = 0;

		read.open("XMLData.txt");
		while (!read.eof())
		{
			getline(read, inputArray[index][0]);
			index++;
		}
		read.close();
	}
	void printInput()                                           //PRINT FUNCTION.
	{
		cout << "THE INPUT XML DATA IS: " << endl << endl;
		for (int i = 0; i < lines; i++)
		{
			cout << "LINE " << i + 1 << " : ";
			cout << inputArray[i][0] << endl;
		}
		cout << endl;
	}

	void CheckXMLCode()                                                                    //CHECK XML FOR ERRORS LINE BY LINE.
	{
		bool isPrologDetected = CheckXMLProlog();                   //HEADER(PROLOG).

		Stack<int>elementalTag;
		int LineIndex = 1;
		if (isPrologDetected == false) LineIndex = 0;

		for (; LineIndex < lines; LineIndex++)                           //ITERATES LINE BY LINE.
		{
			string Line = inputArray[LineIndex][0];
			int LineSize = Line.length();
			bool printLine = true;

			for (int i = 0; i < LineSize; i++)
			{
				if (Line[i] == '<')         //STARTING OF A TAG. WILL PUSH AFTER PUSHING NAME IN EITHER ELEMENTAL STACK OR TEXT STACK.
				{

					if(Line[i + 1] == '!' && Line[i + 2] == '-' && Line[i + 3] == '-')    //IF WE DETECT A COMMENT.
					{
						int j = i + 4;
						bool comment = false;

						while (j < LineSize)
						{
							if (Line[j] == '-' && Line[j + 1] == '-' && Line[j + 2] == '>')
							{
								comment = true;
							}
							j++;
						}

						if (comment== false)
						{
							if (printLine == true)
							{
								cout << "ERROR AT LINE " << LineIndex + 1 << " :";
								printLine = false;
							}
							cout << "CommentError(<!---->). ";
						}
						return;
					}


					string name;
					int k = i + 1;
					bool isTextualorElemental = false;

					while ((Line[k] != ' ' && Line[k] != '>') && k < LineSize) {     //GETS THE INSIDE NAME AND WE DECIDE WHERE TO PUSH BY A BOOL.

						if (Line[k] != '/')name += Line[k];
						if (Line[k] == '/')
						{
							int j = k - 2;
							while (j >= 0)
							{
								if (Line[j] == '<' || Line[j] == '>') isTextualorElemental = true;
								j--;
							}
						}
						k++;

					}
					if (k < LineSize)
					{
						while (k < LineSize)
						{
							if (Line[k] == '<') isTextualorElemental = true;
							k++;
						}
					}
				
					Tags.push("<");


					if (isTextualorElemental == true)
					{
						string text;
						textContentTagsNames.top(text);

						if (text == name)
						{
							textContentTagsNames.pop();
						}
						else
							textContentTagsNames.push(name);
					}
					if (isTextualorElemental == false) {
						elementContentTagsNames.push(name);
						elementalTag.push(LineIndex + 1);
					}

				}
				if (Line[i] == '>')            //ENDING OF A TAG.
				{
					if (Tags.IsEmpty())
					{
						if (printLine == true)
						{
							cout << "ERROR AT LINE " << LineIndex + 1 << " :";
							printLine = false;
						}
						cout << "MissedTag(<,>). " << " ";
					}
					else Tags.pop();
				}
				if (Line[i] == '\"' || Line[i] == '\'')          //ATTRIBUTES QUOTES CHECK BY PUSHING A POPING QUOTES IF ANY.
				{
					string ele;
					Quotes.top(ele);
					if (ele == "\"" || ele == "\'")
					{
						Quotes.pop();
					}
					else
					{
						if (Line[i - 1] != '=')
						{
							if (printLine == true)
							{
								cout << "ERROR AT LINE " << LineIndex + 1 << " :";
								printLine = false;
							}
							cout << "No(=)Found. " << " ";
						}

						if (Line[i] == '\"') Quotes.push("\"");
						if (Line[i] == '\'') Quotes.push("\'");
					}
				}

			}

			string topEle, top1Ele;
			elementContentTagsNames.top(topEle);
			elementContentTagsNames.top1(top1Ele);
			if (topEle == top1Ele)
			{
				elementContentTagsNames.pop();
				elementContentTagsNames.pop();
				elementalTag.pop();
				elementalTag.pop();
			}
			
			if (!Quotes.IsEmpty())   //IF THE QUOTE STACK IS NOT EMPTY.
			{
				if (printLine == true)
				{
					cout << "ERROR AT LINE " << LineIndex + 1 << " :";
					printLine = false;
				}
				cout << "Mismatched Quotes(\"). ";
			}
			if (!Tags.IsEmpty())  //IF THE TAG STACK IS NOT EMPTY WE WILL HAVE A ERROR AT LINE.
			{
				if (printLine == true)
				{
					cout << "ERROR AT LINE " << LineIndex + 1 << " :";
					printLine = false;
				}
				cout << "MissedTag(<,>). ";
			}
			if (!textContentTagsNames.IsEmpty())   //IF THE TEXT TAG NAME STACK IS NOT EMPTY WILL COUT A ERROR.
			{
				if (printLine == true)
				{
					cout << "ERROR AT LINE " << LineIndex + 1 << " :";
					printLine = false;
				}
				string element;
				textContentTagsNames.top(element);
				cout << "Error (" << element << "). ";
				textContentTagsNames.pop();
			}

			if (printLine == false)cout << endl;

			while (!Quotes.IsEmpty()) Quotes.pop();                                //MAKING STACKS EMPTY FOR NEXT LINE.
			while (!Tags.IsEmpty()) Tags.pop();
			while (!textContentTagsNames.IsEmpty()) textContentTagsNames.pop();
		}

		while (!elementContentTagsNames.IsEmpty())    //CHECK MISTAKES IN ELEMENTAL TAGS.
		{
			int line;
			elementalTag.top(line);
			string element;
			elementContentTagsNames.top(element);

			cout << "ERROR AT LINE " << line << " (" << element << "). " << endl;

			elementalTag.pop();
			elementContentTagsNames.pop();
		}
	}
	
	bool CheckXMLProlog()
	{
		string Prolog = inputArray[0][0];
		int prologSize = Prolog.length();
		bool isProlog = false;

		 
		for (int i = 0; i < prologSize; i++) {             //Checks if the first line is a PROLOG or not.
			if (Prolog[i] == '?' && Prolog[i + 1] == 'x' && Prolog[i + 2] == 'm' && Prolog[i + 3] == 'l') isProlog = true;
			if (Prolog[i] == 'x' && Prolog[i + 1] == 'm' && Prolog[i + 2] == 'l') isProlog = true;
		}

		bool printPrologLine = true;

		if (isProlog == false) //IF NO HEADER WE RETURN FALSE IN CHECKXML FUNCTION TO DETERMINE IF XML FILE IS HEADER SUPPORTED OR NOT.
		{
			cout << "NO PROLOG DETECTED !!" << endl;
			cout << endl;
			return isProlog;
		}
		else if (isProlog == true)
		{
			Stack<string> prolog;
			Stack<string> quotes;

			for (int i = 0; i < prologSize; i++)
			{
				if (Prolog[i] == '<')
				{
					prolog.push("<");

					if (Prolog[i + 1] != '?')
					{
						if (printPrologLine == true)
						{
							cout << "ERROR AT LINE 01: (PROLOG) : ";
							printPrologLine = false;
						}
						cout << "MissedQuestionMark(?)." << " ";
					}
				}
				else if (Prolog[i] == '>')
				{
					if (prolog.IsEmpty())
					{
						if (printPrologLine == true)
						{
							cout << "ERROR AT LINE 01: (PROLOG) : ";
							printPrologLine = false;
						}
						cout << "MissedTag(<,>)." << " ";
					}
					else prolog.pop();
					
					if (Prolog[i - 1] != '?')
					{
						if (printPrologLine == true)
						{
							cout << "ERROR AT LINE 01: (PROLOG) : ";
							printPrologLine = false;
						}
						cout << "MissedQuestionMark(?)." << " ";
					}
				}

				if (Prolog[i] == '\"' || Prolog[i] == '\'')
				{
					string ele;
					quotes.top(ele);
					if (ele == "\"" || ele == "\'")
					{
						quotes.pop();
					}
					else
					{
						if (Prolog[i - 1] != '=') cout << "No(=)Found." << " ";

						if (Prolog[i] == '\"') quotes.push("\"");
						if (Prolog[i] == '\'') quotes.push("\'");
					}

				}
				if (Prolog[i] == '=')
				{
					if (Prolog[i + 1] != '\"' && Prolog[i + 1] != '\'')
					{
						if (printPrologLine == true)
						{
							cout << "ERROR AT LINE 01: (PROLOG) : ";
							printPrologLine = false;
						}
						cout << "MissedQuote(\")." << " ";
					}
				}
			}

			if (!(prolog.IsEmpty()))
			{
				if (printPrologLine == true)
				{
					cout << "ERROR AT LINE 01: (PROLOG) : ";
					printPrologLine = false;
				}
				cout << "MissedTag(<,>)." << " ";
			}
			if (!(quotes.IsEmpty()))
			{
				if (printPrologLine == true)
				{
					cout << "ERROR AT LINE 01: (PROLOG) : ";
					printPrologLine = false;
				}
				cout << "MissedQuote(\")" << " ";
			}
			cout << endl;

			return isProlog;
		}

	}
	
};



int main() {

	XMLData MyXMLData;           

	MyXMLData.Input();        //GETS INPUT.
	MyXMLData.printInput();   //PRINTS IT.

	MyXMLData.CheckXMLCode();     //CHECK FOR ERROR. REST IS DONE IN CLASSES.

	system("pause>0");
	return 0;

	//q1
}