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