#include <bits/stdc++.h>

using namespace std;

 map<char, int> mp;
 map<int ,char> mp1;
  int e = 4 , r = 10  ;


int todecimal(string b)
{
    int d=0;
    for (int i=0; i< (int)b.length(); i++)
    {
        if(b[i] == '1') d+= pow(2,(int)b.length()-1-i);
    }
    return d;
}

string toBinary(int n,int e)
{
    string r;
    while(n!=0)
    {
        r=(n%2==0 ?"0":"1")+r;
        n/=2;
    }
    while ((int)r.length() < e)
        r = "0"+r;
    return r;
}

class Node{
private:
public:
    char value;
    int w=0;
    Node* l= nullptr;
    Node* r = nullptr;
    Node* p = nullptr;
};

class Tree{
private:

public:
    Node* root;
    Node* NYT;
    Tree(){
        NYT = root = new Node;
        NYT->value = '$';
    }
    void insertNode(char value){
        Node* right = new Node;
        Node* newNYT = new Node;

        right->value = value;
        right->w = 1;
        right->p = NYT;

        newNYT->value = '$';
        newNYT->p = NYT;

        NYT->l = newNYT;
        NYT->r = right;
        NYT->value = '/';

        NYT = newNYT;
    }

    void updateTree() {
        Node* temp = NYT;
        while (temp->p) {
            temp->p->w = temp->p->r->w + temp->p->l->w;
            temp = temp->p;
        }
    }
    void swapTree() {
        Node* temp = NYT->p;
        while (temp) {
            if (temp->l->w > temp->r->w) {
            Node* temp2 = temp->l;
            temp->l = temp->r;
            temp->r = temp2;
            }
            temp = temp->p ;
        }
    }

    Node* findNode( char findit) {
        stack<Node*> st;
        st.push(root);
        while(!st.empty()){
            if(st.top()->value == findit)
                return st.top();
            Node* node = st.top();
            st.pop();
            if(node->l && node->r){
                st.push(node->r);
                st.push(node->l);
            }
        }
        return nullptr;
    }

    string findNodePath(char findit) {
        string path;
        stack<pair<Node*, string>> st;
        st.push({root, ""});
        while(!st.empty()){
            if(st.top().first->value == findit)
                return st.top().second;
            pair<Node*, string> pf = st.top();
            Node* node = pf.first;
            st.pop();
            if(node->l && node->r){
                st.push({node->r,pf.second+"1"});
                st.push({node->l,pf.second+"0"});
            }
            path = pf.second;
        }
        return path;
    }

};

string encoding (string s) {

    Tree tree;
    string enc="";

    for(int i = 0; i < (int)s.length(); ++i){
        Node* node = tree.findNode(s[i]);
            int fx;
            string bfx;

        if(!node){
            if (mp[s[i]] <= 2*r){
                fx = mp[s[i]] - 1;
                bfx = toBinary(fx,e+1);
            }
            else{
                fx = mp[s[i]]- r- 1;
                bfx = toBinary(fx,e);
            }
            enc += tree.findNodePath('$') + bfx;
            tree.insertNode(s[i]);
        }
        else {
            node->w++;
            enc += tree.findNodePath(s[i]);
        }
        tree.updateTree();
        tree.swapTree();
    }
    return enc;
}

string decoding (string s) {
    Tree tree ;

    string out = "" , temp = "";
    bool flag = 0 ;
    Node* curr = tree.root ;

    for (int i = 0 ; i < (int)s.length() ; i++)
    {
       temp = "";
       flag = 0 ;
       if (s[i] == '0') //left
       {
           if (!(curr -> l)) // external node
           {
               if (mp[curr-> value] > 0  && mp[curr-> value] < 27 ) // is letter
               {    out += curr-> value ;
                    flag = 1 ;
                    curr->w++ ;
                   // cout << "aho fa'l" <<endl ;
                }
               else //isNYT - read e bits
               {
                   for (int j = 0 ; j < e ; j++)
                    {temp += s[i+j] ;}
                    i += e-1 ;
                    if (todecimal(temp) < r)
                    {   i++;
                        temp += s[i];
                    //cout<<todecimal(temp) + 1<<endl;
                     out += mp1[todecimal(temp)+1];}
                    else
                    { out += mp1[todecimal(temp)+ 1 + r];}
                    tree.insertNode(out[(int)out.length()-1]);

               }
               tree.updateTree();
               tree.swapTree();
               curr = tree.root ;
           }
           else //internal
           { curr = curr->l ;
             if (i+1 == s.length())
              out += curr-> value ;}
       }
       else //right
       {
           if (!(curr -> l)) // external node
           {
               if (mp[curr-> value] > 0  && mp[curr-> value] < 27 ) // is letter
               {    out += curr-> value ;
                    flag = 1 ;
                    curr->w++ ;
                }
               else //isNYT - read e bits
               {
                   for (int j = 0 ; j < e ; j++)
                    {temp += s[i+j] ;}
                    i += e-1 ;
                    if (todecimal(temp) < r)
                    {i++;
                    temp += s[i];
                     out += mp1[todecimal(temp)+1];}
                    else
                    { out += mp1[todecimal(temp)+ 1 + r];}
                    tree.insertNode(out[(int)out.length()-1]);
               }
               tree.updateTree();
               tree.swapTree();
               curr = tree.root ;
           }
           else //internal
           { curr = curr->r ;
           if (i+1 == s.length())
                out += curr-> value ;
           }
       }
       i -= flag ;
    }

    return out ;
}


int main()
{
    for(int i = 'a'; i <= 'z'; ++i)
    {   mp[i] = i - 'a' + 1;
        mp1[i - 'a' + 1] = i ;}

    string s = "okpokda";
    cout << encoding(s)<<endl;
    cout << decoding(encoding(s)) ;


    return 0;
}
