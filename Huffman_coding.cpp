///------------------------------------------------
///Creator: RC at 19/6/2016
///------------------------------------------------

#include<iostream>
#include<fstream>
#include<string>
#include<queue>
#include<stack>
#define MAX_NODE 256
using namespace std;

class node{
public:
	int order,weight;
	node* left,*right,*parent;
	char input;
	bool is_char;
	
	node(int order_input){            /////ctor with order
	left = right = parent = NULL;	
	this->order = order_input;
	this->weight = 0;
	this->is_char = false;
	}
	
    node(int order_input,char ch){   /////ctor with order and char
        left = right = parent = NULL;
        this->order = order_input;
        this->weight = 0;
        this->is_char = true;
        this->input  =ch;
    }
    
    void increase_weight(){
        this->weight++;
    }
    
};

class dynamic_huffman_tree{
public:
    int size;
    node* root, *NYT;
    node* check[MAX_NODE];
    bool in_or_not[MAX_NODE];
    
    
    dynamic_huffman_tree(){         /////ctor
        root = new node(100);
        NYT = root;
        
        for(int i = 0;i < MAX_NODE; i++){   /////reset to false
            check[i]=NULL;
            in_or_not[i] = false;
        }
        check[0] = root;
                size = 1;
        
    }
    void insert_node(node *n,char ch){
        
        node* node_with_ch , * node_without_ch;
        
        node_with_ch = new node(100 - size , ch);
        check[size] = node_with_ch;
        in_or_not[ch] = true;             ////第(int)ch = true, 代表有這個字
        size++;
        
        node_without_ch = new node(100-size);
        check[size] = node_without_ch;
        size++;
        
        NYT = node_without_ch;
        node_without_ch->parent = n;
        node_with_ch->parent = n;
        
        n->right = node_with_ch;
        n->left = node_without_ch;
        
        run_modify(node_with_ch);
        
    }
    
    void run_modify(node* n){  ////increase weight and refresh the tree
    
        node * tmp = n;
        
        while(tmp!= root){
        int w = tmp->weight;
        for(int i = 1 ; i < size; i++){
            
            if(check[i] == tmp|| check[i]->order <= tmp->order)break;
            
            if(check[i]->weight <= w && !is_parent(tmp, check[i])){
                swap(tmp,check[i]);
            }
        }
            tmp->increase_weight();
            tmp = tmp-> parent;
        }
    }
    
    void  swap(node* a, node* b){
        
        node* a_parent = a->parent;
        bool is_right;
        a->parent->right == a ? is_right = true : is_right = false;
        
        if(b->parent->left == b){
            b->parent->left = a;
            a->parent = b->parent;
        }
        else{
            b->parent->right = a;
            a->parent = b->parent;
        }
        if(is_right){
            b->parent = a_parent;
            a_parent->right = b;
        }
        else{
            b->parent = a_parent;
            a_parent -> left = b;
        }
        
         ///////BFS using Queue
        
        queue<node*> Q;
        Q.push(root);
        int size = 100;
        
        while(!Q.empty()){
            
            node* r = Q.front();
            Q.pop();
            check[100-size] = r;
            r->order = size;
            size--;
            
            if(r->right){
                Q.push(r->right);
            }
            if(r->left){
                Q.push(r->left);
            }
        }
        
    }
    
    bool is_parent(node*a , node *b){   ////check if b is a's parent
        node* tmp = a->parent;
        while(tmp != root){
            if(b == tmp) return true;
            tmp = tmp -> parent;
        }
        return false;
    }
    
    void output(node* n , FILE* out){          /////use stack to output
                                                ///from bottom node to the root and output from root to bottom
        stack<bool> S;
        node* tmp=n;
        
        while(tmp!=root){
            if(tmp->parent->left == tmp) S.push(false);    ////left
            else S.push(true);                              ///right
            tmp = tmp->parent;
        }
        
        while(!S.empty()){
            bool temp = S.top();
            if(temp) fprintf(out , "1");
            else fprintf(out, "0");
            S.pop();
        }
        
    }
};



int main(){
    char E;
    char input_file_name[50], output_file_name[50];
    char buffer[10000];
    FILE* input_file , *output_file;
    
    cin >> E >> input_file_name >> output_file_name;
    
    dynamic_huffman_tree * dht = new dynamic_huffman_tree();
    
    input_file = fopen(input_file_name , "r");
    output_file = fopen(output_file_name , "w");
    fscanf(input_file, "%s" , buffer);
    
    int length = strlen(buffer);
    
    if(E=='E'){                                             //////endcoding
        
        for(int i = 0; i<length; i++){
            char input_char = buffer[i];
            
            if(dht-> in_or_not[input_char] == true){     /////search tree if ch exist
                for(int j=0 ; j< dht->size ; j++){
                    if(dht->check[j]-> is_char && dht->check[j]->input == input_char){
                        dht->output(dht->check[j], output_file);
                        dht->run_modify(dht->check[j]);
                        break;
                    }
                }
            }
            else{
              /////not exist in the tree
                dht->output (dht->NYT, output_file);
                fprintf(output_file , "%c" , input_char);
                dht->insert_node(dht->NYT,input_char);
            }
        }
    }
    else if(E == 'D'){                                      ////////decoding

        node* tmp = dht->root;
        
        for(int i= 0;i<length;i++){
            char input_char = buffer[i];
            
            
            if(dht->root == dht->NYT){
                dht->insert_node(dht->root, input_char);
                fprintf(output_file , "%c" , input_char);
            }
            else{
                if(input_char =='1')
                    tmp = tmp->right;
                else if(input_char == '0')
                    tmp = tmp->left;
                
                if(tmp->is_char){
                    fprintf(output_file,"%c" , tmp->input);
                    dht->run_modify(tmp);
                    tmp = dht->root;
                    continue;
                }
                    if(tmp == dht->NYT && input_char!='1' && input_char !='0'){
                        dht->insert_node(tmp , input_char);
                        fprintf(output_file , "%c" ,input_char);
                        tmp = dht->root;
                    }
                }
        }
    
    }
    return 0;
}


