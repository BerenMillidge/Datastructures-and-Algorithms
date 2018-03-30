# simple bloom filter python implementation
#this is copying somebody's implementtion, but it is fairly straightforward


from bitarray import bitarray

#3rd party hash
import mmh3

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
			index = mm3.hash(item,ii) %self.size)
			if self.bit_array[index]==0:
				out=False
		return out

		# means of expansion/improvement- counting bloom filters,
		# and scalable bloom filters!