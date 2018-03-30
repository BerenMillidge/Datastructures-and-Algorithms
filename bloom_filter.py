# simple bloom filter python implementation
#this is copying somebody's implementtion, but it is fairly straightforward

from __future__ import division
from bitarray import bitarray

#3rd party hash
import mmh3
import numpy as np

class BloomFilter(set):

	def __init__(self,size, hash_count):
		super(BloomFilter, self).__init__()
		self.bit_aray = bitarray(size) # init bit array
		self.bit_aray.setall(0) # set all to zero for start
		self.size = size
		self.hash_count = hash_count

	#override the default len and iter methods!
	def __len__(self):
		return self.size

	def __iter__(self):
		return iter(self.bit_array)

	def add(self, item):
		for ii in range(self.hash_count):
			index = mm3.hash(item,ii) %self.size # generate the index
			#and flop the bit at the index array
			self.bit_array[index] = 1

		return self

	#detect by seeing if any of the hash indices are zero
	def __contains__(self, item):
		out = True
		for ii in range(self.hash_count):
			index = mm3.hash(item,ii) %self.size
			if self.bit_array[index]==0:
				out=False
		return out

		# means of expansion/improvement- counting bloom filters,
		# and scalable bloom filters!


#my own simple implementatio nof this
class CountingBloomFilter(BloomFilter):

	def __init__(self, size, hash_count):
		super(CountingBloomFilter, self).__init__()
		self.bit_array = bitarray(size)
		self.bit_array.setall(0)
		self.size=size
		self.hash_count= hash_count


		#adding is simple - just incremenet the counts each time
	def add(self, item):
		for ii in range(self.hash_count):
			index = mm3.hash(item,ii) % self.size
			self.bit_array[index] +=1
		return self

	def __contains__(self,item):
		out = True
		for ii in range(self.hash_count):
			index = mm3.hash(item,ii) %self.size
			if self.bit_array[index]==0:
				out=False
				#dont' need to do the whole loop if false
				return out
		return out

	def remove(self,item):
		#this is the tricky part, remove only if it is in there, which means that all
		# the items are there, and this will result ni some false negatives
		# and decrement there
		#you can't get the item back, just a true/false
		indices = []
		for ii in range(self.hash_count):
			index = mm3.hash(item,ii) %self.size
			if self.bit_array[index] ==0:
				# no point removing, so retrun
				return False
			#greater than 1 so can be removed
			indices.append(index)

		#if succeeds now decrement
		for index in indices:
			self.bit_array[index] -=1
		return True


# the scalable bloom filter  http://gsd.di.uminho.pt/members/cbm/ps/dbloom.pdf
#just adds an additional filter every time you run out of space
# in the previous filter but makes it slightly larger
# so that the overall probability of error remains bounded. that's not that difficult
# probably to be honest, but I really don't know
# it uses the slice method to distribute the hash indices more uniformly also
# so I suppose I should implement that


class ScalableBloomFilter(set):

	def __init__(self, initial_size, hash_count, tightening_ratio=0.5, max_false_positive=0.1):
		super(ScalableBloomFilter, self).__init__()
		#initialise
		self.filters = []
		self.size = initial_size
		#save this for later
		self.initial_size = initial_size
		self.slice_size = initial_size//hash_count
		self.num_filters = 0
		self.initial_num_slices = hash_count
		self.tightening_ratio = tightening_ratio
		self.max_false_positive = max_false_positive
		self.hash_count = hash_count
		#count number of items added
		self.items_added = 0
		#initialise these here
		self.current_filter = None
		self.current_filter_size = None
		self.current_max_size = None
		# add the initial filter
		self._add_filter()

		# I also need to figure out when to add a new slice
		# so that is taken care of automatically

	def _get_max_size(self):
		# this is the equation to calculate this
		return self.current_filter_size * (np.log(2)**2/np.abs(np.log(self.max_false_positive)))
		
	def add(self, item):
		#first check if create new filter?
		if self.items_added >= self.current_max_size:
			self._add_filter()
		#then continue
		#for each hash check
		for i in range(self.hash_count):
			#the partition number and then the actual increment within partition address
			index = (i * self.slice_size) + (mm3.hash(item, i) % self.slice_size)
			self.current_filter[index] = 1
		return self

	def contains(self, item):
		#check all filters, stopping as soon as match foudn
		#can't stop as soon as zero found
		# as that's problematic
		#stop as soon as 1s in all are found in a filter
		# that's just the false positive rate having to deal with tbh
		#hopfully largely unlikely!
		for filt in filters:
			ones_found = 0
			for i in range(self.hash_count):
				index = (i*self.slice_size) + (mm3.hash(item,i) %self.slice_size)
				if filt[index]==1:
					ones_found +=1
			if ones_found >=self.hash_count:
				#success found in a filter
				return True

		#no successes found!
		return False

	def __len__(self):
		return len(self.filters)

	def __iter__(self):
		return __iter__(__iter__(self.filters))


	def _add_filter(self):
		new_num_slices = self.initial_num_slices + (self.num_filters * np.log((1/self.tightening_ratio),2)
		new_size = new_num_slices * self.slice_size
		new_filter = bitarray(new_size)
		new_filter.setall(0)
		self.filters.append(new_filter)
		self.current_filter = new_filter
		self.current_filter_size = new_size
		self.current_max_size = self._get_max_size()
		return self





