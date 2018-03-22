//okay, prime functions for the hash table
// hash tables shold be a prime number in size
// as for some weird reason, this has beneficial mathematical properties
// which is really nice and cool.Primes are around everything
// and it is weird

#include <math.h>
#include "prime.h"

//returns 1 if x is prime, 0 if not, -1 if undefined

int is_prime(const int x){
	if(x<2){
		return -1;
	}
	if(x<4){
		return 1;
	}
	if((x%2)==0){
		return 0;
	}
	//now check possible factors
	for (int i = 3; i<floor(sqrt((double) x)); i+=2){
		if((x%i)==0){
			return 0;
		}
	}
	return 1;
}

//returns next prime after x or x if it is prime - does by brute force search
int next_prime(int x){
	while(is_prime(x)!=1){
		x++;
	}
	return x;
}