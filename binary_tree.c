//okay, very simple binary tree implementation

typedef struct node {
	int key_value;
	//pointers to left and right node
	node* left;
	node* right;
} node;

//initialise root
node* root = 0;

void destroy_tree(node* leaf){
	//if not root
	if(leaf!=0){
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		free(leaf);
	}
}

void insert(int key, node** leaf){
	//if root, allocate and begin tree
	if(*leaf = 0){
		*leaf = malloc(sizeof(node));
		*leaf->key_value = key;
		//initialise children to null
		*leaf->left = 0;
		*leaf->right=0;
	}
	//choose to insert recursively
	else if (key < *leaf->key_value){
		insert(key, &(*leaf)->left);
	}
	else if(key>= *leaf->key_value){
		insert(key, &(*leaf)->right);
	}
}

//and search
node* search(int key, node* leaf){
	if(leaf!=0){
		if(key==leaf->key_value){
			//success!
			return leaf;
		}
		if(key<leaf->key_value){
			return search(key, leaf->left);
		}
		if(key>leaf->key_value){
			return search(key, leaf->right);
		}
	}
	return 0;
}