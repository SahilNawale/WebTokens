#include <iostream>
#include <ctime>
#include <bits/stdc++.h>
#include <unordered_map>
#include <string>
#include <list>
#include <utility>
#include <cstdlib>

using namespace std;

static int userCounter = 1;

class User
{
public:
	int user_id;
	string username, password;
	User(string name, string pass)
	{
		user_id = userCounter;
		username = name;
		password = pass;
		userCounter++;
	}
	User(int id, string name, string pass)
	{
		user_id = id;
		username = name;
		password = pass;
	}
};

//main data structure of hashmap for storing tokens
unordered_map<string, list<pair<string, User *>>> *  mainBucket[4];
unordered_map<string, list<pair<string, User *>>> map0, map1, map2, map3, map4;

class Node
{
public:
	User *userPointer;
	Node *left;
	Node *right;
	int height;
};

Node *userTreeRoot = NULL;

int max(int a, int b)
{
	return (a > b) ? a : b;
}
int height(Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

Node *newNode(User *user)
{
	Node *node = new Node();
	node->userPointer = user;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;

	return (node);
}

Node *rightRotate(Node *y)
{
	Node *x = y->left;
	Node *T2 = x->right;

	x->right = y;
	y->left = T2;

	y->height = max(height(y->left),
					height(y->right)) +
				1;
	x->height = max(height(x->left),
					height(x->right)) +
				1;

	return x;
}

Node *leftRotate(Node *x)
{
	Node *y = x->right;
	Node *T2 = y->left;

	y->left = x;
	x->right = T2;

	x->height = max(height(x->left),
					height(x->right)) +
				1;
	y->height = max(height(y->left),
					height(y->right)) +
				1;

	return y;
}

int getBalance(Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

Node *insert(Node *node, User *user)
{
	if (node == NULL)
		return (newNode(user));

	if (user->user_id < node->userPointer->user_id)
		node->left = insert(node->left, user);
	else if (user->user_id > node->userPointer->user_id)
		node->right = insert(node->right, user);
	else
		return node;

	node->height = 1 + max(height(node->left),
						   height(node->right));

	int balance = getBalance(node);

	if (balance > 1 && user->user_id < node->left->userPointer->user_id)
		return rightRotate(node);

	if (balance < -1 && user->user_id > node->right->userPointer->user_id)
		return leftRotate(node);

	if (balance > 1 && user->user_id > node->left->userPointer->user_id)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (balance < -1 && user->user_id < node->right->userPointer->user_id)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

Node * minValueNode(Node* node)
{
    Node* current = node;
 
    while (current->left != NULL)
        current = current->left;
 
    return current;
}

Node* deleteNode(Node* root, User* user)
{
     
    if (root == NULL)
        return root;
 
    if ( user->user_id < root->userPointer->user_id )
        root->left = deleteNode(root->left, user);
 
    else if( user->user_id > root->userPointer->user_id )
        root->right = deleteNode(root->right, user);

    else
    {
        if( (root->left == NULL) ||
            (root->right == NULL) )
        {
            Node *temp = root->left ? root->left : root->right ;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else 
            *root = *temp; 
            free(temp);
        }
        else
        {
            Node* temp = minValueNode(root->right);
 
            root->userPointer->user_id = temp->userPointer->user_id;
 
            root->right = deleteNode(root->right,
                                     temp->userPointer);
        }
    }
 
    if (root == NULL)
    return root;
 
    root->height = 1 + max(height(root->left),
                           height(root->right));
 
    int balance = getBalance(root);
 
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);
 
    if (balance > 1 &&
        getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
 
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return leftRotate(root);
 
    if (balance < -1 &&
        getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
 
    return root;
}
// Part Yash
bool is_number(const string& s)
{
    return !s.empty() && find_if(s.begin(), 
        s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

void preOrder(Node *root)
{
	if (root != NULL)
	{
		cout << root->userPointer->user_id << " ";
		preOrder(root->left);
		preOrder(root->right);
	}
}

Node *searchUserTree(Node *root, int user_id)
{
	if (root == NULL || root->userPointer->user_id == user_id)
		return root;

	if (root->userPointer->user_id < user_id)
		return searchUserTree(root->right, user_id);

	return searchUserTree(root->left, user_id);
}

bool isValidUser(User *user)
{
	Node *result = searchUserTree(userTreeRoot, user->user_id);
	if (result->userPointer->username == user->username && result->userPointer->password == user->password)
	{
		return true;
	}
	return false;
}

int getAsciiSum(string str)
{
	int i, sum = 0;
	for (i = 0; str[i] != '\0'; i++)
		sum = sum + str[i];
	return sum;
}

string generateToken(User *user)
{

	//timestamp.userDataHash.indexNumber
	int tempUserId = user->user_id;
	string tempUsername = user->username;
	string tempPass = user->password;
	// Get expirydate = current timestamp  + 3600 (1hr expiry token )
	string expiryTimeStamp = to_string(time(nullptr) + 3600).substr(5);
	// Get bucket sequence
	srand((unsigned)time(NULL)); // initialize random generator
	int bucket = rand() % 5;
	// generate main hash
	string mainHash = to_string(tempUserId) + to_string((tempUserId % 3) * getAsciiSum(tempUsername)) + to_string(getAsciiSum(tempPass));
	// Final token
	string token = (expiryTimeStamp) + "." + mainHash + "." + to_string(bucket);
	return token;

}

string getTimestampFromToken(string token)
{

	//to handle invalid input
	token.append(".x.x");
	string res;
	int i = 0;
	while (token[i] != '.')
	{
		res = res + token[i];
		i++;
	}
	return res;
}

User *findInList(list<pair<string, User *>> list, string token)
{
	string timeStamp = getTimestampFromToken(token);
	for (auto &itr : list)
	{
		if (itr.first == timeStamp)
		{
			return itr.second;
		}
	}
	cout << "Not a valid token" << endl;
	return nullptr;
}

string getBucketNumberFromToken(string token)
{
	token.append(".x.x");
	int i = 0;
	while (token[i] != '.')
	{
		i++;
	}
	i++;
	while (token[i] != '.')
	{
		i++;
	}
	i++;
	string res;
	while (token[i]!='.')
	{
		res = res + token[i];
		i++;
	}
	return res;
}

string getMainHashFromToken(string token)
{
	token.append(".x.x");
	string res;
	int i = 0;
	while (token[i] != '.')
	{
		i++;
	}
	i++;
	while (token[i] != '.')
	{
		res = res + token[i];
		i++;
	}
	return res;
}

void insertToken(string token, User *user)
{
	string bucketNumber = getBucketNumberFromToken(token);
	int number = stoi(bucketNumber);
	string mainHash = getMainHashFromToken(token);

	if (mainBucket[number]->find(mainHash) == mainBucket[number]->end())
	{
		list<pair<string, User *>> temp;
		temp.push_front(make_pair(getTimestampFromToken(token), user));
		mainBucket[number]->insert({mainHash, temp});
	}
	else
	{
		mainBucket[number]->find(mainHash)->second.push_back(make_pair(getTimestampFromToken(token), user));
	}
}
//Part Anushka

class expiryListNode
{
public:
	string token;
	expiryListNode *next;
	expiryListNode(string tk)
	{
		token = tk;
		next = nullptr;
	}
};

class expiryList
{
public:
	expiryListNode *head;
	expiryListNode *tail;
	expiryList()
	{
		head = tail = nullptr;
	}
	void insert(string token)
	{
		if (head == tail && head == nullptr)
		{
			expiryListNode *temp = new expiryListNode(token);
			head = tail = temp;
		}
		else
		{
			expiryListNode *temp = new expiryListNode(token);
			tail->next = temp;
			tail = temp;
		}
	}
	void checkTokenExpiryAndRemove()
	{
		string token = head->token;
		int bucketNumber = stoi(getBucketNumberFromToken(token));
		string mainHash = getMainHashFromToken(token);
		string timeStamp = getTimestampFromToken(token);
		string curr_time = to_string(time(nullptr)).substr(5);
		if (stoi(timeStamp) <= stoi(curr_time))
		{
			(*mainBucket)[bucketNumber].find(mainHash)->second.remove_if([timeStamp](auto it)
																		 { return it.first == timeStamp; });
			expiryListNode *temp = head;
			head = head->next;
			free(temp);
		}
	}
	void print()
	{
		expiryListNode *temp = head;
		while (temp != nullptr)
		{
			cout << temp->token << " -> ";
			temp = temp->next;
		}
		cout << "\n";
	}
};

bool isValidToken(string token)
{
	if(!is_number(getBucketNumberFromToken(token))){
		cout<<"Invalid input\n";
		return false;
	}
	int bucketNumber = stoi(getBucketNumberFromToken(token));
	if (bucketNumber >= 5)
	{
		return false;
	}
	string mainHash = getMainHashFromToken(token);
	string timeStamp = getTimestampFromToken(token);
	if (findInList(mainBucket[bucketNumber]->find(mainHash)->second, token) == nullptr)
	{
		return false;
	};
	return true;
}

bool getTopSecretData(string token)
{
	if (isValidToken(token))
	{
		cout << " DS MINI PROJECT" << endl;
		return true;
	}
	else
	{
		cout << "You are not Authorised\n";
		return false;
	}
}

User* deleteToken(string token){
	int bucketNumber = stoi(getBucketNumberFromToken(token));
	string mainHash = getMainHashFromToken(token);
	string timeStamp = getTimestampFromToken(token);
	User* tempUser = findInList((*mainBucket)[bucketNumber].find(mainHash)->second,token);
	(*mainBucket)[bucketNumber].find(mainHash)->second.remove_if([timeStamp](auto it)
																		 { return it.first == timeStamp; });
	return tempUser;
}

int main()
{
	cout << " ----- Welcome to Token Authentication system ----- \n";
	//initalizing map
	mainBucket[0] = &map0;
	mainBucket[1] = &map1;
	mainBucket[2] = &map2;
	mainBucket[3] = &map3;
	mainBucket[4] = &map4;

	//creating expiryList
	expiryList *expList = new expiryList();

	//Populating User Tree
	User *user1 = new User("Sahil", "1234");
	User *user2 = new User("Yash", "2652");
	User *user3 = new User("Anushka", "2890");
	User *user4 = new User("Rahul", "1224");
	User *user5 = new User("Soham", "4568");
	User *user6 = new User("Aditya", "3960");
	userTreeRoot = insert(userTreeRoot, user1);
	userTreeRoot = insert(userTreeRoot, user2);
	userTreeRoot = insert(userTreeRoot, user3);
	userTreeRoot = insert(userTreeRoot, user4);
	userTreeRoot = insert(userTreeRoot, user5);
	userTreeRoot = insert(userTreeRoot, user6);
	// preOrder(userTreeRoot);

int runner =1;

while(runner==1)
{
	int mainMenu = 0;
	cout << " ---- Options -----\n";
	cout << "   1 : Login\n";
	cout << "   2 : Get top secret data \n";
	cout << "   3 : Signup\n";
	cout << "   4 : Get all active tokens\n";
	cout << "   5 : Remove expired tokens\n";
	cout << "   6 : Log Out\n";
	cout << "   7 : Exit\n";
	cin >> mainMenu;

	if (mainMenu == 1)
	{
		string id;
		cout << " -- Enter your User ID -- \n";
		cin >> id;
		if(!is_number(id)){
			cout<<"invalid Input\n";
			continue;
		}

		int user_id = stoi(id);
		string username;
		cout << " -- Enter username -- \n";
		cin >> username;

		string password;
		cout << " -- Enter Password --\n";
		cin >> password;

		User *tempUser = new User(user_id, username, password);
		if (!isValidUser(tempUser))
		{
			cout << " --! Login failed !--" << endl;
			return 0; //Exit program if not valid user
		}else{
			cout<<" -- Login successfull --\n";
		};

		string token = generateToken(tempUser);
		cout<<" -- Your Token is : "<<token<<endl;

		//insert token into main data structure
		insertToken(token, tempUser);

		//insert token into expiryList
		expList->insert(token);
		expList->checkTokenExpiryAndRemove();
	}
	else if (mainMenu == 2)
	{
		string token;
		cout<<" -- Enter your token --\n";
		cin>>token;
		getTopSecretData(token);
	}
	else if (mainMenu==3){

		string username;
		cout << " -- Enter username -- \n";
		cin >> username;

		string password;
		cout << " -- Enter Password --\n";
		cin >> password;

		User* newUser = new User(username,password);
    	cout<<" (IMP|Note Down) => Your user id is "<<newUser->user_id<<endl;
    	userTreeRoot = insert(userTreeRoot, newUser);
		cout<<" --Now you can login with new credentials\n";

	}
	else if(mainMenu==4){
		cout<<" -- All Active Tokens -- \n";
		expList->print();
	}
	else if(mainMenu==5){
		expList->checkTokenExpiryAndRemove();
		cout<<"-- Expired Tokens Removed Successfully\n";
	}
	else if(mainMenu==6){
		string token;
		cout << " -- Enter your token -- \n";
		cin >> token;
		try{
			if(!isValidToken(token)){
				continue;
			}
			User* tempUser = deleteToken(token);

			deleteNode(userTreeRoot,tempUser);
			expList->checkTokenExpiryAndRemove();
		}
		catch(string err){
			cout<<"invalid input\n";
			continue;
		}
	}
	else if(mainMenu==7){
		runner = 0;
	}
}
	return 0;
}