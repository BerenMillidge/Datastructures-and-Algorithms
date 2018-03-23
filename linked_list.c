// okay, linked list implementation


//define linked node. A linked list is just one where each item contains
// a pointer to the next.
typedef struct node {
	int val;
	struct node* next;
} node;

void print_list(node* head){
	// initialise it
	node* current = head;
	//and iterate through the list
	while(current!=NULL){
		printf("%d\n", current->val);
		current = current->next;
	}
}
// add a new item to the list, requires iterating through the whole list

void push(node* head, int val){
	node* current = head;
	while(current->next!=NULL){
		current = current->next;
	}
	//now at the end so can add next variable
	current->next = malloc(sizeof(node));
	//assign new value
	current->next->val = val;
	//and set next next to null
	current->next->next = NULL;
}

void add_first(node** head, int val){
	node* new_node;
	new_node = malloc(sizeof(node));

	new_node->val = val;
	new_node->next = *head;
	*head = new_node;
}
//remove first item - pop
int pop(node** head){
	if(*head==NULL){
		return -1;
	}
	next_node = *head->next;
	val = *head->val;
	//now free head
	free(*head);
	//and set new head
	*head = next_node;
	return val;
}

int remove_by_index(node** head, int n){
	// need to look two ahead since change value depending where the node opints
	int i = 0;
	int retval = -1;
	node* current = *head;
	node* temp_node = NULL;

	if(n==0){
		return pop(head);
	}
	//iterate through the list to get to the end
	for (i=0; i<n-1; i++){
		if(current->next==NULL){
			// so index out of bounds
			return retval;
		}
		current = current->next;
	}
	// have reached the item, now need toremove and relink the list
	temp_node = current->next;
	retval = temp_node->val;
	current->next = temp_node->next;
	free(temp_node);
	return retval;
}

int remove_last(node* head){
	if(head->next==NULL){
		int retval = head->val;
		free(head);
		return retval;
	}
	//get second toe last node in the list
	node* current = head;
	while(current->next->next !=NULL){
		current = current->next;
	}
	// now current points to second to last item, so remove current ->next
	retval = current->next->val;
	free(current->next);
	current->next = NULL;
	return retval;
}

int remove_by_value(node** head, int val){
	int i = 0;
	node* current = *head;
	node* temp_node = NULL;
	
	// it needs to look two ahead
	if(current->val ==val){
		return pop(head);
	}
	// it's not the first so look two ahead
	while(current->next->next!=NULL){
		//check if match
		if(val==current->next->val){
			temp_node = current->next;
			//get val
			int retval = temp_node->val;
			//relink the pointer
			current->next = temp_node->next;
			//free
			free(temp_node);
			return retval;
		}
		//iterate one forward
		current = current->next;
	}
	//if this does not work, returns -1
	return -1;
}