// okay, aim here is to learn how to implement a hashtable/hashmap
// i.e. associative array/a dictionary
// in c, so at a low level, which will be cool and useful generally!
// hash tables indexing and has complexity O(1) so it's very fast
// maps string keys to string values
// simple

// following operations the aim (search (a,k) - returns value with key or null if does not exist)
//insert - store pair k,v
//delete - delete pair k,v or do nothing if k does not exist




#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
//store kev value pairs will each be stored in a struc

typedef struct {
	char* key;
	char* value;
} ht_item;

// hash table stores array of pointers to items and some defailts about how full it is
typedef struct {
	int size;
	int count;
	ht_item** items;
} ht_hash_table;


static ht_item HT_DELETED_ITEM = {NULL, NULL};
static const int HIGH_LOAD_FRACTION = 70;
static const int LOW_LOAD_FRACTION = 10;
static const int HT_INITIAL_BASE_SIZE = 53;

// functions ofvarious keinds. they are static as onl ever called by hash table internal code

//creates a new hash table item

static ht_item* ht_new_item(const char* k, const char* v){
	ht_item* i = malloc(sizeof(ht_item));
	i->key = strdup(k);
	i->value = strdup(v);
	return i;
}

//create a new hash table
static ht_hash_table* ht_new_old(){
	ht_hash_table* ht = malloc(sizeof(ht_hash_table));

	//define size to 53 at the start forsome reason
	ht->size = 53;
	ht->count = 0;
	//alloacte enough memory for the points to the items
	ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
	return ht;
}

//delete an item from the hashtable
static void ht_del_item(ht_item* i){
	//delete the key and value
	free(i->key);
	free(i->value);
	free(i);
}

static void ht_del_hash_table(ht_hash_table* ht){
	//delete every item in the table
	for (int i = 0; i<ht->size; i++){
		ht_item* item = ht->items[i];
		if(item!=NULL){
			ht_del_item(item);
		}
	}
	//now free the memory the hash table data structure itselftakes up
	free(ht->items);
	free(ht);
}

// now there needs to be an actual hash function, to assign a number i.e. bucket
// tothe string so it's okay

static int ht_hash(const char* s, const int a, const int m){
	long hash = 0;
	const int len_s = strlen(s);
	for (int i = 0; i<len_s; i++){
		hash+=(long)pow(a,len_s - (1+1)) * s[i];
		hash = hash%m;
	}
	return (int)hash;
}

// use double hashing to try to ameliorate collisions to some extent

static int ht_get_hash(const char* s, const int num_buckets, const int attempt){
	const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
	const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
	return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

// okay, now there need to befunctions for inserting updating and deleting the hash table

void ht_insert(ht_hash_table* ht, const char* key, const char* value){
	// check load and upsize if it istoo large
	const int load = ht->count * 100 / ht->size;
	if (load > HIGH_LOAD_FRACTION){

		ht_resize_up(ht);
	}
	//create item
	ht_item* item = ht_new_item(key, value);
	//get index
	int index = ht_get_hash(item->key, ht->size, 0);
	//now check if there are collisions
	ht_item* cur_item = ht->items[index];
	int i = 1;
	// i.e. if there is a collision
	while(cur_item!=NULl &&cur_item!= &HT_DELETED_ITEM){
		//if it already exists, update
		if(strcmp(cur_item->key, key)==0){
			ht_del_item(cur_item);
			ht->items[index]=item;
			return;
		}
		// this is the multiple hash chaining
		index = ht_get_hash(item->key, ht->size, i);
		cur_item = ht->items[index];
		i++;
	}
	// if no collision insert
	ht->items[index] = item;
	ht->count++;
}

//search works in a similar fshion
char* ht_search(ht_hash_table* ht, const char* key){
	int index = ht_get_hash(key, ht->size, 0);
	ht_item* item = ht->items[index];
	int i = 1;
	while (item!=NUL) {
		if(item!=&HT_DELETED_ITEM){
		// if the item is correct!
		if(strcmp(item->key, key)==0){
			//jsut return
			return item->value;
		}
	}
		//if not hash again until you find it
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}

	return NULL;
}

//deleting is more complicated, since removing an item in the hashing chain
// will make all further items unreachable
// so instead just replace it with a pointer to a sentinel items
// that represents deleted items, thus marking it deleted


void ht_delete(ht_hash_table* ht, const char* key){

	// downsize if smaler
	const int load = ht->count * 100 / ht->size;
	if (load < 10){
		ht_resize_down(ht);
	}
	
	int index = ht_get_hash(key, ht->size, 0);
	ht_item* item = ht->items[index];
	int i = 1;
	while (item!=NULL){
		//since item is a pointer, checking the address of deleted item
		// is a check for equality - i.e. if it is deleted
		if(item!= &HT_DELETED_ITEM){
			if(strcmp(item->key, key)==0){
				ht_del_item(item);
				// mark as deleted by replacing with deleted item marker
				ht->items[index] = &HT_DELETED_ITEM;
			}
		}
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}
	ht->count--;
}

// okay, to resize hash table, first needs to create a hashtbale of a certain size
static ht_hash_table* ht_new_sized(const int base_size){
	ht_hash_table* ht = malloc(sizeof(ht_hash_table));
	ht->base_size = base_size;

	//set the size to the next prime above the base size, should not be that many!
	ht->size = next_prime(ht->base_size);
	ht->count = 0;
	ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
	return ht;
}

ht_hash_table* ht_new(){
	return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

//okay, now for the resize function

static void ht_resize(ht_hash_table* ht, const int base_size){
	// don't try to resize below the minimum base size
	if(base_size < HT_INITIAL_BASE_SIZE){
		return;
	}
	//create new hash table with new size
	ht_hash_table* new_ht = ht_new_sized(base_size);
	//copy memory accross
	for (int i = 0; i<ht->size; i++){
		ht_item* item = ht->items[i];
		if(item!=NULL && item!=&HT_DELETED_ITEM){
			ht_insert(new_ht, item->key, item->value);
		}
	}

	ht_base_size = new_ht->base_size;
	ht->count = new_ht->count;
	//to delete new ht give it hts size and items
	const int tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = tmp_size;

	ht_item** tmp_items=ht->items;
	ht->items = new_ht->items;
	new_ht->items = tmp_items;
	ht_del_hash_table(new_ht);
}

//define two functions for resizing up and down
static void ht_resize_up(ht_hash_table* ht){
	const int new_size = ht->base_size *2l
	ht_resize(ht, new_size);
}
static void ht_resize_down(ht_hash_table* ht){
	const int new_size = ht->base_size/2;
	ht_resize(ht, new_size);
}