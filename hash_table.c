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


// functions ofvarious keinds. they are static as onl ever called by hash table internal code

//creates a new hash table item

static ht_item* ht_new_item(const char* k, const char* v){
	ht_item* i = malloc(sizeof(ht_item));
	i->key = strdup(k);
	i->value = strdup(v);
	return i;
}

//create a new hash table
static ht_hash_table* ht_new(){
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
	//create item
	ht_item* item = ht_new_item(key, value);
	//get index
	int index = ht_get_hash(item->key, ht->size, 0);
	//now check if there are collisions
	ht_item* cur_item = ht->items[index];
	int i = 1;
	// i.e. if there is a collision
	while(cur_item!=NULl){
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
	while (item!=NUL){
		// if the item is correct!
		if(strcmp(item->key, key)==0){
			//jsut return
			return item->value;
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

static ht_item HT_DELETED_ITEM = {NULL, NULL};

void ht_delete(ht_hash_table* ht, const char* key){
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