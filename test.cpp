#include<iostream>
#include<ctime>
#include<cstdlib>
#include<unordered_map>
#include <string>
#include<list>
#include<ctime>
#include <bits/stdc++.h>
#include <utility>

using namespace std;

static int userCounter = 1;

class User{
    public:
        int user_id;
        string username,password;
        User(string name,string pass){
            user_id = userCounter;
            username = name;
            password = pass;
            userCounter++;
        }
		User(int id,string name,string pass){
			user_id = id;
            username = name;
            password = pass;
		}
};


unordered_map<string,list<pair<string,User*>>>* mainBucket[4];
unordered_map<string,list<pair<string,User*>>> map0 , map1 , map2 , map3 , map4;  

string getTimestampFromToken(string token){
    string res;
    int i=0;
    while(token[i]!='.'){
        res = res + token[i];
        i++;
    }
    return res;
}

User* findInList(list<pair<string,User*>> list,string token){
    string timeStamp = getTimestampFromToken(token);
    for(auto &itr:list){
        if(itr.first==timeStamp){
            return itr.second;
        }
    }
    cout<<"Not a valid token"<<endl;
    return nullptr;
}

string getBucketNumberFromToken(string token){
    int i=0;
    while(token[i]!='.'){
        i++;
    }
    i++;
    while(token[i]!='.'){
        i++;
    }
    i++;
    string res;
    while(i<=(token.length())){
        res = res + token[i];
        i++;
    }
    return res;
}


string getMainHashFromToken(string token){
    string res;
    int i = 0;
    while(token[i]!='.'){
        i++;
    }
    i++;
    while(token[i]!='.'){
        res = res + token[i];
        i++;
    }
    return res;
}

void insertToken(string token,User* user){
    string bucketNumber = getBucketNumberFromToken(token);
    int number = stoi(bucketNumber);
    string mainHash = getMainHashFromToken(token);
    if(mainBucket[number]->find(mainHash)==mainBucket[number]->end()){
        list<pair<string,User*>> temp;
        temp.push_front(make_pair(getTimestampFromToken(token),user));
        mainBucket[number]->insert({mainHash,temp});
    }
    else{
        mainBucket[number]->find(mainHash)->second.push_back(make_pair(getTimestampFromToken(token),user));
    }
}

User* getUserFromToken(string token){
    string bucketNumber = getBucketNumberFromToken(token);
    int number = stoi(bucketNumber);
    string mainHash = getMainHashFromToken(token);
    if(mainBucket[number]->find(mainHash)==mainBucket[number]->end()){
        cout<<"Cannot get user : Invalid Token\n";
        return nullptr;
    }
    else{
        return findInList(mainBucket[number]->find(mainHash)->second,token);
    }
}
class expiryListNode{
    public :
        string token;
        expiryListNode* next;
        expiryListNode(string tk){
            token = tk;
            next = nullptr;
        }        
};

class expiryList {
    public : 
        expiryListNode* head;
        expiryListNode* tail;
        expiryList(string token){
            expiryListNode* temp = new expiryListNode(token);
            head = tail = temp;
        }
        void insert(string token){
            expiryListNode* temp = new expiryListNode(token);
            tail->next = temp;
            tail = temp;
        }
        void checkTokenExpiryAndRemove(){
            string token = head->token;
            int bucketNumber = stoi(getBucketNumberFromToken(token));
            string mainHash = getMainHashFromToken(token);
            string timeStamp = getTimestampFromToken(token);
            string curr_time = to_string(time(nullptr)).substr(5);
            if(stoi(timeStamp)<=stoi(curr_time)){
                (*mainBucket)[bucketNumber].find(mainHash)->second.remove_if([timeStamp](auto it){
                    return it.first == timeStamp;
                });
                expiryListNode*  temp = head;
                head = head->next;
                free(temp);
                checkTokenExpiryAndRemove();
            }
        }
        void print(){
            expiryListNode* temp = head;
            while(temp!=nullptr){
                cout<<temp->token<<" -> ";
                temp = temp->next;
            }
        }
};

int main (){  

    mainBucket[0] = &map0;
    mainBucket[1] = &map1;
    mainBucket[2] = &map2;
    mainBucket[3] = &map3;
    mainBucket[4] = &map4;

    User* tempUser = new User("Sahil","0000");
    string token = "12345.q2084bowrawea.1";
    User* tempUser2 = new User("Sahil2","02000");
    string token2 = "99999.sfsefsfrsgarg.1";

    insertToken(token,tempUser);
    insertToken(token2,tempUser2);

    expiryList* exp = new expiryList(token);

    exp->insert(token2);
    cout<<"\n";
    exp->checkTokenExpiryAndRemove();
    exp->checkTokenExpiryAndRemove();

    cout<<mainBucket[1]->find(getMainHashFromToken(token))->second.front().second->username;

    cout<<"\n";

    // string timeStamp = getTimestampFromToken(token);
    // (*mainBucket[1]).find(getMainHashFromToken(token))->second.remove_if([timeStamp](auto it){
    //                 cout<<"Here";
    //                 return it.first == timeStamp;
    //             });

    
    // string timeStamp = getTimestampFromToken(token);

	// mainBucket[0] = &map0;
	// map0[token].push_front(make_pair(timeStamp,temp));
    // User* res = findInList(map0[token],"Iabauidwo");
    // cout<<res->username;


    // cout<<map0[token].front().second->username;
    return 0;
}







// #include<iostream>
// #include<ctime>
// #include<cstdlib>
// #include<unordered_map>
// using namespace std;

// static int userCounter = 1;

// class User{
//     public:
//         int user_id;
//         string username,password;
//         User(string name,string pass){
//             user_id = userCounter;
//             username = name;
//             password = pass;
//             userCounter++;
//         }
// 		User(int id,string name,string pass){
// 			user_id = id;
//             username = name;
//             password = pass;
// 		}
// };


// int main (){

//     User* temp = new User("Sahil","0000");
//     string token = "q2084bowrawea";
//     unordered_map<string,User*>* mainBucket[4];
// 	unordered_map<string,User*> map0 , map1 , map2 , map3 , map4;
// 	mainBucket[0] = &map0;
// 	map0[token] = temp;
//     cout<<mainBucket[0]->find(token)->second->username;
//     return 0;
// }