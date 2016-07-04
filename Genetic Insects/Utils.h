#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define stdmax(a,b) (a>b?a:b)


inline float RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}


inline float RandomInt(int min, int max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return (int)((float)min + r * float(max - min));
}

class BlockIndexBitField
{
public:

	BlockIndexBitField(int numBits)
	{
		dataSize = 32;
		if (numBits == 0)
			numBits = dataSize;

		m_numBits = numBits;
		m_sizeInInts = (numBits / dataSize) + 1; // need to add one int

		m_data = new int[(m_sizeInInts)];

		//memset( (void*)m_data, 0, sizeof(m_data) );
		for (int i = 0; i < m_sizeInInts; i++)
		{
			m_data[i] = 0;
		}
	}

	~BlockIndexBitField()
	{
		if (m_data)
			delete[] m_data;
	}

	void SetBit(int idx)
	{
		if (idx > m_numBits) return;

		if (idx < 0) return;

		int blockIndex = (int)((float)(idx) / dataSize);

		int remainder = idx - blockIndex*dataSize;

		m_data[blockIndex] |= (1 << remainder);
	}

	bool CheckBit(int idx)
	{
		if (idx > m_numBits) return false;

		if (idx < 0) return false;

		int blockIndex = (int)((float)(idx) / dataSize);

		int remainder = idx - blockIndex*dataSize;

		return m_data[blockIndex] & (1 << remainder);
	}

	void ResetBit(int idx)
	{
		if (idx > m_numBits) return;

		if (idx < 0) return;

		int blockIndex = (int)((float)(idx) / dataSize);

		int remainder = idx - blockIndex*dataSize;

		m_data[blockIndex] &= ~(1 << remainder);
	}

	void Clear()
	{
		for (int i = 0; i < m_sizeInInts; i++)
		{
			m_data[i] = 0;
		}
	}

	bool Binary_AND(BlockIndexBitField &B, BlockIndexBitField &C)
	{
		if (B.m_numBits != C.m_numBits)
		{
			return false;
		}

		if (this->m_numBits != B.m_numBits)
		{
			return false;
		}

		for (int i = 0; i < B.m_sizeInInts; i++)
		{
			this->m_data[i] = B.m_data[i] & C.m_data[i];
		}

		return true;
	}

	bool Binary_OR(BlockIndexBitField &B, BlockIndexBitField &C)
	{
		if (B.m_numBits != C.m_numBits)
		{
			return false;
		}

		if (this->m_numBits != B.m_numBits)
		{
			return false;
		}

		for (int i = 0; i < B.m_sizeInInts; i++)
		{
			this->m_data[i] = B.m_data[i] | C.m_data[i];
		}

		return true;
	}

	bool Binary_NAND(BlockIndexBitField &B, BlockIndexBitField &C)
	{
		if (B.m_numBits != C.m_numBits)
		{
			return false;
		}

		if (this->m_numBits != B.m_numBits)
		{
			return false;
		}

		for (int i = 0; i < B.m_sizeInInts; i++)
		{
			this->m_data[i] = B.m_data[i] & ~C.m_data[i];
		}

		return true;
	}

	bool Binary_XOR(BlockIndexBitField &B, BlockIndexBitField &C)
	{
		if (B.m_numBits != C.m_numBits)
		{
			return false;
		}

		if (this->m_numBits != B.m_numBits)
		{
			return false;
		}

		for (int i = 0; i < B.m_sizeInInts; i++)
		{
			this->m_data[i] = B.m_data[i] ^ C.m_data[i];
		}

		return true;
	}

	int m_sizeInInts;
	int m_numBits;
	int dataSize;
	int *m_data;
};

#endif
