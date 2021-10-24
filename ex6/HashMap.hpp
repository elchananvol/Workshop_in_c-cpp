//
// Created by Eli on 12/09/2020.
//

#ifndef EX5_HASHMAP_HPP
#define EX5_HASHMAP_HPP

#include <iterator>
#include <vector>

#define MIN_CAPACITY 16
#define TOP_BOUND 0.75
#define LOW_BOUND 0.25
#define START 0
#define POW 2
#define ERROR "invalid argument"


template<typename keyT, typename valueT>
/**
 * this class execute hash map by dynamic array and hash func to store elements
 */
class HashMap
{
private:
	size_t _size;
	size_t _capacity;
	std::vector<std::pair<keyT, valueT> > *buckets;

public:
	/**
	 * this is constructor that iterators to beginning and end of container of the keys and values
	 * and install the hash table accordingly
	 * @tparam KeysInputIterator  type of the keys input constIterator
	 * @tparam ValuesInputIterator type of the values input constIterator
	 * @param keysBegin constIterator to the beginning of the container of the keys
	 * @param keysEnd constIterator to the end of the container of the keys
	 * @param valuesBegin constIterator to the beginning of the container of the values
	 * @param valuesEnd constIterator to the end of the container of the values
	 */
	template<typename KeysInputIterator,
			typename ValuesInputIterator>
	HashMap(const KeysInputIterator keysBegin,
			const KeysInputIterator keysEnd,
			const ValuesInputIterator valuesBegin,
			const ValuesInputIterator valuesEnd) noexcept(false):
			_size(START), _capacity(MIN_CAPACITY), buckets(nullptr)
	{
		size_t a = std::distance(keysBegin, keysEnd);
		size_t b = std::distance(valuesBegin, valuesEnd);
		if (a != b)
		{
			throw std::invalid_argument(ERROR);
		}

		buckets = new std::vector<std::pair<keyT, valueT> >[_capacity];
		KeysInputIterator i = keysBegin;
		ValuesInputIterator j = valuesBegin;
		for (; i != keysEnd; i++, j++)
		{
			(*this)[*i] = (*j);
		}

	}

	/**
	 * default constructor. initialise empty ty table with capacity 16
	 */
	HashMap() noexcept(false):
			_size(START), _capacity(MIN_CAPACITY), buckets(nullptr)
	{
		buckets = new std::vector<std::pair<keyT, valueT> >[_capacity];
	}

	/**
	 * copy constructor
	 * @param other - other HashMap
 	*/
	HashMap(const HashMap &other) noexcept(false):
			_size(other.size()), _capacity(other.capacity()), buckets(nullptr)
	{
		buckets = new std::vector<std::pair<keyT, valueT> >[capacity()];
		for (size_t i = 0; i < capacity(); i++)
		{
			buckets[i] = other.buckets[i];
		}

	}

	/**
	 * destructor. just delete the allocation
	 */
	~HashMap()
	{
		delete[] buckets;
	}

	/**
	 * this mקthod insert key & value to the table in pair.
	 * if the key exist - return false
	 * @param key
	 * @param value
	 * @return true in the end of process, false if key already exist or resize up fail
	 */
	bool insert(const keyT key, const valueT value) noexcept
	{
		if (contains_key(key))
		{
			return false;
		}
		_size++;
		if (load_factor() > TOP_BOUND)
		{
			if (!resize(TOP_BOUND))
			{
				_size--;
				return false;
			}
		}
		buckets[hashIndex(key)].push_back(std::make_pair(key, value));
		return true;
	}

	/**
	 * bool func that check if the table contain some key
	 */
	bool contains_key(const keyT &key) const noexcept
	{
		for (auto i:buckets[hashIndex(key)])
		{
			if (key == i.first)
			{
				return true;
			}
		}
		return false;
	}


	/** getter method that return the load_factor (_size / _capacity)*/
	double load_factor() const
	{
		return (double) _size / _capacity;
	};

	/** getter method that return the table size*/
	size_t size() const
	{
		return _size;
	}

	/** getter method that return the capacity*/
	size_t capacity() const
	{
		return _capacity;
	}

	/** getter method that true if table empty*/

	bool empty() const
	{
		return (_size == START);
	}

	/** getter method that return reference to value if key exist, otherwise throw an exception*/
	valueT &at(keyT key) noexcept(false)
	{
		if (!contains_key(key))
		{
			throw std::out_of_range(ERROR);
		}
		return (*this)[key];
	}

	/** const getter method that return the value if key exist, otherwise throw an exception*/
	valueT at(keyT key) const noexcept(false)
	{
		if (!contains_key(key))
		{
			throw std::out_of_range(ERROR);
		}
		return (*this)[key];
	}

	/** const getter method that return the size of bucket of key if it exist, otherwise throw an
	 * exception*/
	size_t bucket_size(const keyT &key) const noexcept(false)
	{
		if (!contains_key(key))
		{
			throw std::invalid_argument(ERROR);
		}
		return buckets[bucket_index(key)].size();
	}

	/** const getter method that return the index of bucket of key if it exist, otherwise throw an
 * exception*/
	size_t bucket_index(const keyT &key) const noexcept(false)
	{
		if (!contains_key(key))
		{
			throw std::invalid_argument(ERROR);
		}
		return std::hash<keyT>{}(key) & (_capacity - 1);
	}

	/** const getter method that return the index of which bucket key Should be */
	size_t hashIndex(const keyT &key) const noexcept
	{
		return std::hash<keyT>{}(key) & (_capacity - 1);
	}

	/**
	 * mathod to resize the capacity while insert or erase
	 * @param direction: get TOP_BOUND (0.75) and resize up, or down otherwise
	 * @return true if process succeed, false if allocation fail
	 */
	bool resize(double direction) noexcept
	{
		int tmp_capacity;
		std::vector<std::pair<keyT, valueT> > *tmp_buckets;
		if (direction == TOP_BOUND)
		{

			tmp_capacity = POW * capacity();
		}
		else
		{
			tmp_capacity = _capacity / POW;
		}
		try
		{
			tmp_buckets = new std::vector<std::pair<keyT, valueT> >[tmp_capacity];
		}
		catch (const std::bad_alloc &e)
		{
			return false;
		}


		for (auto i: (*this))
		{
			size_t hash = std::hash<keyT>{}(i.first) & (tmp_capacity - 1);
			tmp_buckets[hash].push_back(std::make_pair(i.first, i.second));
		}
		_capacity = tmp_capacity;
		delete[] buckets;
		buckets = tmp_buckets;
		return true;
	}

	/**
	 *  this method erase the key she get and it's value (if key exist, otherwise return false)
	 * @return true in the end of process, or false if key doesnt exist or resize down fail
	 */
	bool erase(const keyT &key) noexcept
	{
		if (!contains_key(key))
		{
			return false;
		}

		if (((double) _size - 1) / _capacity < LOW_BOUND && _capacity > 1)
		{
			if (!resize(LOW_BOUND))
			{
				return false;
			}
		}
		auto &bucket = buckets[bucket_index(key)];

		for (auto i = bucket.begin(); i != bucket.end(); i++)
			//for (auto &index : bucket)
		{
			if (key == i->first)
			{
				bucket.erase(i);
				break;
			}
		}
		_size--;
		return true;
	}

	/** clean all the table to be empty */
	void clear()
	{
		for (size_t i = 0; i < _capacity; i++)
		{
			buckets[i].clear();
		}
		_size = START;
	}

	/**
 * bool function to check if two hash are not the same
* if they contain the same keys and values they are the same even there is another Hash func
 * @param ther the another hash
 * @return false if they are same, true otherwise
 */

	bool operator!=(const HashMap &other) const
	{
		return !((*this) == other);
	}

	/**
 * bool function to check if two HashMap are the same
	 * if they contain the same keys and values they are the same even there is another Hash func
 * @param other the another HashMap
 * @return true if they are same, false otherwise
 */

	bool operator==(const HashMap &other) const
	{
		if (this == &other)
		{
			return true;
		}
		if (_size == other.size() && _capacity == other.capacity())
		{
			for (auto i = other.begin(); i != other.end(); i++)
			{
				if (!contains_key(i->first))
				{
					return false;
				}
				if (i->second != (*this)[i->first])
				{
					return false;
				}
			}
			return true;
		}
		return false;

	}

	/*** assignment operator. if new allocation fail the method do nothing*/
	HashMap &operator=(const HashMap &other) noexcept
	{
		std::vector<std::pair<keyT, valueT> > *new_buckets;
		if (this == &other)
		{
			return (*this);
		}
		try
		{
			new_buckets = new std::vector<std::pair<keyT, valueT> >[other._capacity];
		}
		catch (const std::bad_alloc &e)
		{
			return (*this);
		}
		delete[] buckets;
		_size = other.size();
		_capacity = other._capacity;
		buckets = new_buckets;
		for (int i = 0; (size_t) i < _capacity; i++)
		{
			buckets[i] = other.buckets[i];
		}
		return *this;
	}

	/** getter method that return reference to value if key exist, otherwise its create one*/
	valueT &operator[](const keyT &key) noexcept
	{
		valueT val = valueT();
		if (!contains_key(key))
		{
			insert(key, val);
		}
		auto &bucket = buckets[bucket_index(key)];
		for (auto &i : bucket)
		{
			if (key == i.first)
			{
				return i.second;
			}
		}
		return buckets[START].begin()->second; //just to make compiler not yelling or if insert fail
	}

	/** const getter method that return a value if key exist, otherwise is undefined behavior*/
	valueT operator[](const keyT &key) const
	{
		valueT val = valueT();
		if (contains_key(key))
		{
			for (auto &i : buckets[bucket_index(key)])
			{
				if (key == i.first)
				{
					return i.second;
				}
			}
		}
		return val;
	}

	/**
	 * this class execute iterator on hash map
	 */
	class constIterator
	{
	public:
		/** all the typedef that needed*/
		typedef constIterator iterator;
		typedef std::pair<keyT, valueT> ValueType;
		typedef const std::pair<keyT, valueT> &Reference;
		typedef const std::pair<keyT, valueT> *Pointer;
		typedef std::forward_iterator_tag IteratorCategory;
		typedef std::ptrdiff_t differenceType;


		/** const dereference operator
		 *
		 */
		Reference operator*() const
		{
			return *cur;
		}

		/**
		 * Advances the iterator in one, until the valuess are finished //prefix
		 */
		constIterator &operator++()
		{
			if (cur != hashMap->buckets[hashMap->capacity() - 1].cend())
			{
				if (cur != hashMap->buckets[index].cend())
				{
					cur++;
				}
				if (cur == hashMap->buckets[index].cend() &&
					(size_t) index < hashMap->capacity() - 1)
				{

					index++;
					while (!hashMap->buckets[index].size())
					{
						index++;
						if ((size_t) index >= hashMap->_capacity)
						{
							cur = hashMap->buckets[hashMap->capacity() - 1].cend();
							return *this;
						}
					}
					cur = hashMap->buckets[index].cbegin();
				}
			}
			return *this;
		}

		/**
		 * Advances the iterator in one, until the values are finished  /postfix
		 */
		constIterator operator++(int)
		{
			auto temp(*this);
			++(*this);
			return temp;
		}

		/**
		* bool function to check if two iterators are Pointer to the same */
		bool operator==(const constIterator &other) const
		{
			return cur == other.cur;
		}

		/** bool function to check if two iterators are not Pointer to the same */
		bool operator!=(const constIterator &other) const
		{
			return cur != other.cur;
		}

		Pointer operator->() const
		{
			return &(*cur);
		}

		/**
		 * constructor/ do nothing
		 * @param parent the hash map for iterator to work on
		 * @param cur - iterator to the beginning of the hash.(and the class work with this
		 * iterator all the time
		 * @param i - the bucket index in buckets that cur still there
		 */
		constIterator(const HashMap *parent,
					  typename std::vector<std::pair<keyT, valueT> >::const_iterator
					  cur, int i) : hashMap(parent), cur(cur), index(i)
		{}

		/**
		 * copy constructor
		 */
		constIterator(const constIterator &other) : hashMap(other.hashMap), cur(other.cur),
													index(other.index)
		{}

		/** default constructor*/
		constIterator() : hashMap(HashMap()), cur(nullptr), index(START)
		{}

		/** assignment operator. */
		constIterator &operator=(const constIterator &other)
		{
			if (this == &other)
			{
				return *this;
			}
			hashMap = other.hashMap;
			cur = other.cur;
			index = other.index;
			return *this;
		}

	private:
		const HashMap *hashMap;
		typename std::vector<std::pair<keyT, valueT> >::const_iterator cur;
		int index;
	};

	typedef constIterator iterator;

	/**
	* bring iterator to the start of hash map
 	* @return const iterator to the start of hashmap
 	*/
	HashMap::constIterator begin() const
	{
		return cbegin();

	}

	/**
 	* the method return const iterator to the end of hashMap
 	* @return const iterator to the end of hashMap
 	*/
	HashMap::constIterator end() const
	{
		return cend();
	}

	/**
	 * bring iterator to the start of hash map
	 * @return const iterator to the start of hashmap
	 * */
	HashMap::constIterator cbegin() const
	{
		if (_size > 0)
		{
			for (size_t i = 0; i < _capacity; i++)
			{
				if (buckets[i].size() > 0)
				{
					HashMap::constIterator it(this, buckets[i].cbegin(), i);
					return it;
				}
			}
		}
		return end();
	}

	/**
	 * the method return const iterator to the end of hashMap
	 * @return const iterator to the end of hashMap
	 */
	HashMap::constIterator cend() const
	{
		HashMap::constIterator it(this, buckets[_capacity - 1].cend(), _capacity - 1);
		return it;
	}


};

#endif //EX5_HASHMAP_HPP
