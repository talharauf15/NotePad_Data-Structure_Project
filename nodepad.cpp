#include <iostream>
#include <string>
#include <fstream>
//#include <windows.h>

using namespace std;

const int page_width_in_clms = 12;//columns
int page_length_in_rows;//rows



struct Stack_Node
{
	string data;
	Stack_Node* link;
}*top;


class stack_linked_list_base {

public:

	stack_linked_list_base() { //constructor
		top = NULL;
	}


	bool isempty()	//checks if stack is empty or not
	{
		if (top == NULL)
			return true;
		else
			return false;
	}


	void push(string value)	//to push value in the stack
	{
		Stack_Node* temp = new Stack_Node;
		temp->data = value;
		temp->link = top;
		top = temp;
	}


	string pop()	//to pop value from the stack
	{
		string popped_value;
		if (isempty()) {
			cout << "\n\n=>Stack is Empty";
			return "empty";
		}
		else
		{
			Stack_Node* temp = top;
			top = top->link;
			popped_value = temp->data;
			free(temp);
		}
		return popped_value;
	}

	void Top_of_stack() //returns the top of stack
	{
		if (isempty())
			cout << "\n\n=>Stack is Empty";
		else
			cout << "\n\n=>Element at top is : " << top->data;
	}


	void displayStack() //displays the stack
	{
		cout << endl << endl;
		if (isempty())
			cout << "\n=>Stack is Empty";
		else
		{
			Stack_Node* temp = top;
			while (temp != NULL)
			{
				cout << temp->data << " ";
				temp = temp->link;
			}
			cout << "\n";
		}
	}

};

struct node {
	string text = "[    ]";	//stores text for each node
	node* right, * left, * up, * down; //required 4 pointers for the node
	int x = 0, y = 0; //x,y positions for each node
};


class _4DLinkedList {
	node* first, * last, * newnode;
	static int x_pos, y_pos;
	bool row_changed = false;
	string copy_String;
	stack_linked_list_base stk;
public:
	_4DLinkedList();
	~_4DLinkedList();
	void add_text();
	void delete_text();
	void search_text();
	void copy_text();
	void paste_text();
	void count_words();
	void create();//create page 
	void replace();
	void undo_text();
	void pushing_stack();
	void traversal_from_first_node();//Display
	void load_text();
	void save_text();
	void similarity();



};

int _4DLinkedList::x_pos = 1;
int _4DLinkedList::y_pos = 1;

_4DLinkedList::_4DLinkedList()//constructor
{
	first = last = NULL;
}

_4DLinkedList::~_4DLinkedList() {//destructor
	node* temp = new node;
	while (first != NULL) {
		temp = first;
		first = first->right;
		delete temp;
	}
	first = last = NULL;

}



void _4DLinkedList::load_text() {
	string word;
	int n;
	ifstream my_input_file;
	cout << "Press 1 to load starting file" << endl;
	cout << "Press any key to load saved file :" << endl;
	cin >> n;

	if(n==1)
		my_input_file.open("dummy.txt");
	else
		my_input_file.open("abc.txt");

	if (!(my_input_file.is_open()))
	{
		cout << "File cannot be opened.";
		return;
	}

	node* temp = first;
	while (my_input_file >> word && temp) {

		temp->text = word;
		temp = temp->right;
	}
	my_input_file.close();
	cout << "\n\ndata has been successfully loaded\n";

}

void _4DLinkedList::pushing_stack() //pushes the whole text into the stack
{
	node* temp = first;
	string word;
	while (temp)
	{
		word = temp->text;
		stk.push(word);
		temp = temp->right;
	}

}

void _4DLinkedList::undo_text()
{

	node* temp = last;
	string word;
	while (temp)
	{
		word = stk.pop();
		if (word == "empty") {
			cout << "\n\nUndo Unsuccessful\n";
			break;
		}
		temp->text = word;
		temp = temp->left;
	}

	if (word != "empty")
		cout << "\n\nUndo Successful\n";

}

void _4DLinkedList::create() {	//function to create nodes

	newnode = new node;

	//setting all directions to null
	newnode->right = NULL;
	newnode->left = NULL;
	newnode->up = NULL;
	newnode->down = NULL;

	newnode->x = x_pos; 
	newnode->y = y_pos++;	

	if (y_pos == (page_width_in_clms + 1))
	{
		newnode->x = x_pos++; //changes the row position for nodes greater than 20
		y_pos = 1;
		row_changed = true;
	}

	if (first == 0) {
		first = last = newnode;
	}
	else {

		last->right = newnode;
		last->right->left = last;
		last = newnode;

		if (row_changed)
		{
			
			node* temp = first;
			{
				temp->down = newnode;
				newnode->up = temp;
				temp = temp->right;

			}
		}

	}

}

void _4DLinkedList::traversal_from_first_node()
{	//traverses from first node to end node
	node* temp = first;
	int controller = (temp->y - 1);
	while (temp) {
		if (temp != NULL) {
			if ((controller == page_width_in_clms)) {
				cout << endl;
				controller = 0;
			}
		}
		controller++;

		//cout <<"(" << temp->x << "," << temp->y << ")" << " ";
		cout << temp->text << " ";
		temp = temp->right;
	}
}

void _4DLinkedList::add_text()
{
	pushing_stack();
	string _string;
	cout << "\n=>Enter the string: ";
	cin.ignore();
	getline(cin, _string);
	_string += " ";
	int x, y;
	cout << "\n\n=>Enter the x position of node(row no): ";
	cin >> x;
	cout << "\n=>Enter the y position of node(clm no): ";
	cin >> y;



	int total_words = 0;
	string word = "";
	for (auto x : _string)	//calculates the total number of given words
	{
		if (x == ' ')
			++total_words;
	}

	string* words = new string[total_words];

	int i = 0;

	for (auto x : _string)	//stores the words into dynamic string
	{
		if (x == ' ')
		{
			words[i++] = word;
			word = "";
		}
		else
			word += x;
	}

	node* temp1 = first;

	if ((x <1 or x>page_length_in_rows) or (y<1 or y>page_width_in_clms))	//checking position within range
	{
		cout << "\t\n Node positions out of bound, Plz remain within the limit\n\n";
		return;
	}

	while (temp1)//finding position
	{
		if (temp1->x == x && temp1->y == y)
		{
			break;
		}
		temp1 = temp1->right;

	}

	for (int i = 0; i < total_words; i++)//insert
	{
		if (temp1 == NULL)
			break;
		temp1->text = words[i];
		temp1 = temp1->right;

	}

}

void _4DLinkedList::delete_text()
{
	pushing_stack();
	int total_words = 0;
	string search_string;
	bool check_if_jump = false;
	bool still_matching = false;
	cout << "\n=>Enter the string: ";
	cin.ignore();
	getline(cin, search_string);
	search_string += " ";
	string word = "";


	for (auto x : search_string)	//calculates the total number of given words
	{
		if (x == ' ')
			++total_words;
	}

	string* words = new string[total_words];

	int i = 0;

	for (auto x : search_string)	//stores the words into dynamic string
	{
		if (x == ' ')
		{
			words[i++] = word;
			word = "";
		}
		else
			word += x;
	}


	node* temp = first;

repeat:

	while (temp) { //finds the temp pointer where first word is present
		if (temp->text == words[0])
		{
			still_matching = true;
			break;
		}
		else
			temp = temp->right;
	}

	if (total_words > 1 && (still_matching)) {
		for (int i = 1; i < total_words; i++) {

			if (temp != NULL) //controls temp null error
			{
				temp = temp->right;

				if (temp->text == words[i])
				{
					still_matching = true;
				}
				else
				{
					still_matching = false;
					break;

				}

			}

		}
	}

	if (still_matching)
	{
		for (int i = 0; i < total_words; i++)
		{
			temp->text = "";
			temp = temp->left;
		}
		cout << "\n=>Text has been deleted" << endl;

	}

	else
	{
		if (temp) {

			temp = temp->right;

			goto repeat;

		}

		cout << "\n=>Text not deleted, as matching string not found" << endl;

	}
}

void _4DLinkedList::search_text()
{
	int x_pos, y_pos;
	int total_words = 0;
	string search_string;
	bool check_if_jump = false;
	bool still_matching = false;
	cout << "\n=>Enter the string: ";
	cin.ignore();
	getline(cin, search_string);
	search_string += " ";
	string word = "";


	for (auto x : search_string)	//calculates the total number of given words
	{
		if (x == ' ')
			++total_words;
	}

	string* words = new string[total_words];

	int i = 0;

	for (auto x : search_string)	//stores the words into dynamic string
	{
		if (x == ' ')
		{
			words[i++] = word;
			word = "";
		}
		else
			word += x;
	}


	node* temp = first;

jump:

	still_matching = false;
	while (temp) { //finds the temp pointer where first word is present
		if (temp->text == words[0])
		{
			x_pos = temp->x;
			y_pos = temp->y;
			still_matching = true;
			break;
		}
		else
			temp = temp->right;
	}

	if (total_words > 1 && (still_matching)) {
		for (int i = 1; i < total_words; i++) {

			if (temp != NULL) //controls temp null error
			{
				temp = temp->right;

				if (temp->text == words[i])
				{
					still_matching = true;
				}
				else
				{
					still_matching = false;
					break;

				}

			}

		}
	}

	else if (still_matching)
		cout << "\n=>String found at" << " row no: " << x_pos << " and word no: " << y_pos << endl;


	if (total_words > 1)
		if (still_matching)
			cout << "\n=>String found at" << " row no: " << x_pos << " and word no: " << y_pos << endl;


	if ((!still_matching) && temp == NULL && !check_if_jump) {
		cout << "\n\n=>String not found\n";
	}
	else if (temp != NULL)
	{
		check_if_jump = true;
		if (still_matching)
			temp = temp->right;
		goto jump;
	}

}

void _4DLinkedList::copy_text()
{
	copy_String = "";
	int x, y, x1, y1;
	cout << "\n\n=>Enter the starting position of x(row no): ";
	cin >> x;
	cout << "\n=>Enter the starting position of y(clm no): ";
	cin >> y;


	node* temp = first;
	if ((x <1 or x>page_length_in_rows) or (y<1 or y>page_width_in_clms))
	{
		cout << "\t\n Node positions out of bound, Plz remain within the limit\n\n";
		return;
	}

	cout << "\n\n=>Enter the ending position of x(row no): ";
	cin >> x1;
	cout << "\n=>Enter the ending position of y(clm no): ";
	cin >> y1;

	if ((x1 <1 or x1>page_length_in_rows) or (y1<1 or y1>page_width_in_clms))
	{
		cout << "\t\n Node positions out of bound, Plz remain within the limit\n\n";
		return;
	}

	bool flag = true;
	bool copy = false;
	


		while (temp)
		{


			if (temp->x == x && temp->y == y)
			{

				copy = true;

			}


			if (copy == true)
			{
				copy_String += temp->text;
				copy_String += " ";
				if (temp->y == y1 && temp->x == x1)
				{
					break;
				}

			}
			temp = temp->right;

		}

		cout << "\n\nThe copied string is: " << copy_String << endl << endl;
	

	paste_text();
}

void _4DLinkedList::paste_text()
{
	pushing_stack();
	cout << "\nNow Paste Operation Enter Positions";
	int x2, y2;
	cout << "\n\n=>Enter initial position of x(row no): "; cin >> x2;
	cout << "\n\n=>Enter initial position of y(clm no): "; cin >> y2;
	node* temp1 = first;
	if ((x2 <1 or x2>page_length_in_rows) or (y2<1 or y2>page_width_in_clms))
	{
		cout << "\t\n Node positions out of bound, Plz remain within the limit\n\n";
		return;
	}
	int total_words = 0;
	string word = "";
	for (auto x : copy_String)	//calculates the total number of given words
	{
		if (x == ' ')
			++total_words;
	}

	string* words = new string[total_words];

	int i = 0;

	for (auto x : copy_String)	//stores the words into dynamic string
	{
		if (x == ' ')
		{
			words[i++] = word;
			word = "";
		}
		else
			word += x;
	}

	while (temp1)
	{
		if (temp1->x == x2 && temp1->y == y2)
		{
			break;
		}
		temp1 = temp1->right;

	}

	char choice = 'y';

	if (choice == 'y')
	{
		for (int i = 0; i < total_words; i++)
		{
			if (temp1 == NULL)
				break;
			temp1->text = words[i];
			temp1 = temp1->right;

		}
	}
	else
	{
		cout << "\n=>Aborting paste operation\n";
	}
}

void _4DLinkedList::replace() {
	pushing_stack();
	string word;
	string new_word;
	cout << "\n=>Enter the word to be replaced: ";
	cin >> word;
	node* temp = first;
	char choice;
	cout << "\n=>Do you want to replace whole document y/n: ";
	cin >> choice;
	cout << "\n=>Enter new word: ";
	cin >> new_word;

	while (temp) {
		if (temp->text == word)
		{
			if (choice == 'n')
			{
				cout << "\n=>Replacing 1st word only\n";
				temp->text = new_word;
				break;
			}
			else
			{
				temp->text = new_word;
			}
		}
		temp = temp->right;
	}

}

void _4DLinkedList::count_words() {
	int count = 0;
	node* temp = first;
	while (temp) {
		if (temp->text != "" and temp->text != "[    ]")
			count++;
		temp = temp->right;
	}

	cout << "\n\n=>Total Words: " << count << endl;
}

void _4DLinkedList::save_text() {
	int count = 0;
	node* temp = first;
	

	ofstream file("abc.txt"); // Open the file for writing
	if (file.is_open()) {
		node* temp = first;
		while (temp != nullptr) {
			file << temp->text << endl;
			temp = temp->right;
		}
		file.close(); // Close the file
		std::cout << "Data written to file." << std::endl;
	}
	else {
		std::cout << "Unable to open file." << std::endl;
	}
}

void _4DLinkedList::similarity() {
	ifstream file1("dummy.txt");
	ifstream file2("abc.txt");

	if (!file1.is_open() || !file2.is_open()) {
		std::cout << "Unable to open one or both files." << std::endl;
		return ;
	}

	char ch1, ch2;
	int matchingChars = 0;
	int totalChars = 0;
	
	while (file1.get(ch1) && file2.get(ch2)) {
		totalChars++;
		if (ch1 == ch2) {
			matchingChars++;
		}
	}

	if (file1.eof() && file2.eof()) {
		matchingChars++; 
	}

	file1.close();
	file2.close();

	

	double Similarity = (static_cast<double>(matchingChars) / totalChars) * 100.0;

	std::cout << "Similarity percentage: " << Similarity << "%" << std::endl;




}

void main_menu_of_notepad()
{

	cout << "\n\n\n\t\t     +==========================================+" << endl;
	cout << "\t\t     |           MAIN MENU OF NOTEPAD           |" << endl;
	cout << "\t\t     +==========================================+" << endl;



	cout << "\n\t\t\t\t[e] => for exit\n";
	cout << "\t\t\t\t[i] => for page creation\n";
	cout << "\t\t\t\t[l] => for loading output text\n";
	cout << "\t\t\t\t[a] => for add text\n";
	cout << "\t\t\t\t[d] => for delete text\n";
	cout << "\t\t\t\t[c] => for copying and pasting text\n";
	cout << "\t\t\t\t[r] => for replacing text\n";
	cout << "\t\t\t\t[s] => for search text\n";
	cout << "\t\t\t\t[ss] => for save text\n";
	cout << "\t\t\t\t[u] => for undo text\n";
	cout << "\t\t\t\t[w] => for count words\n";
	cout << "\t\t\t\t[sc] => for similarity check files\n";
	cout << "\t\t\t\t[t] => for traversal\n";
	cout << "\t\t\t\t[x] => for clear screen\n\n";


}
int main()
{
	bool check_page = false;
	string choice;
	_4DLinkedList obj1;
	while (true)
	{
		//system("color f0");
		main_menu_of_notepad();
		cout << "\t\t\t\tInput -> : "; cin >> choice;

		if (choice == "i")
		{
			//system("color 4f");
			if (check_page) {
				obj1.~_4DLinkedList();
			}
			check_page = true;
			cout << "\n\n\t\t------------------------< CREATING Page >--------------------------\n\n";

			cout << "\n\n=> Enter the length of the page(in rows): "; cin >> page_length_in_rows;
			int total_insertions = page_length_in_rows * page_width_in_clms;
			for (int i = 1; i <= total_insertions; i++)
			{
				obj1.create();
			}
			cout << endl;
			obj1.traversal_from_first_node();
		}
		else if (choice == "l" and check_page)
		{
			//system("color 5f");
			cout << "\n\n\t\t-----------------------< LOADING DUMMY TEXT >-----------------------\n\n";
			obj1.load_text();
			cout << endl << endl;
			obj1.traversal_from_first_node();

		}
		else if (choice == "ss" and check_page)
		{
			//system("color 5f");
			cout << "\n\n\t\t-----------------------< SAVING TEXT >-----------------------\n\n";
			obj1.save_text();
			cout << endl << endl;
			obj1.traversal_from_first_node();

		}
		else if (choice == "a" and check_page)
		{
			//system("color 8f");
			cout << "\n\n\t\t---------------------------< ADD TEXT >-----------------------------\n\n";
			obj1.add_text();
			cout << endl << endl;
		}
		else if (choice == "d" and check_page)
		{
			//system("color 1f");
			cout << "\n\n\t\t--------------------------< DELETE TEXT >----------------------------\n\n";
			obj1.delete_text();
			cout << endl << endl;
		}
		else if (choice == "c" and check_page)
		{
			//system("color 5f");
			cout << "\n\n\t\t-------------------------< COPY AND PASTE >--------------------------\n\n";
			obj1.copy_text();
			cout << endl << endl;
		}
		else if (choice == "sc" and check_page)
		{
			//system("color 5f");
			cout << "\n\n\t\t-----------------------< CHECKING Similarity TEXT >-----------------------\n\n";
			obj1.similarity();
			cout << endl << endl;
			//obj1.traversal_from_first_node();

		}
		else if (choice == "r" and check_page)
		{
			//system("color 4f");
			cout << "\n\n\t\t--------------------------< REPLACE TEXT >---------------------------\n\n";
			obj1.replace();
			cout << endl << endl;
		}
		else if (choice == "s" and check_page)
		{
			//system("color 5f");
			cout << "\n\n\t\t--------------------------< SEARCH TEXT >---------------------------\n\n";
			obj1.search_text();
			cout << endl << endl;
		}
		else if (choice == "u" and check_page)
		{
			//system("color 8f");
			cout << "\n\n\t\t---------------------------< UNDO TEXT >----------------------------\n\n";
			obj1.undo_text();
			cout << endl << endl;
		}
		else if (choice == "w" and check_page)
		{
			//system("color 8f");
			cout << "\n\n\t\t--------------------------< COUNT WORDS >---------------------------\n\n";
			obj1.count_words();
			cout << endl << endl;
		}
		else if (choice == "t" and check_page)

		{
			//system("color 4f");
			cout << "\n\n\t\t---------------------------< TRAVERSAL >----------------------------\n\n";
			obj1.traversal_from_first_node();
			cout << endl << endl;
			system("pause");
		}
		else if (choice == "e")
		{
			cout << "\n\n\t\t--------------------------< EXIT PROGRAM >--------------------------\n\n";
			cout << "\n\n=> The program is terminating\n\n";
			break;
		}
		else if (choice == "x")
		{
			system("CLS");
		}
		else
		{
			cout << "\t\t\t\tInvalid Input ";
			if (!check_page)
				cout << " or page not created yet";

		}
	}

	system("pause");
	return 0;
}
