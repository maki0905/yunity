#include "BlockAllocator.h"

#include <cassert>
#include <limits.h>
#include <string.h>
#include <stddef.h>

static const uint32_t chunkSize = 16 * 1024;
static const uint32_t maxBlockSize = 640;
static const uint32_t chunkArrayIncrement = 120;

// �T�|�[�g�����I�u�W�F�N�g�̃T�C�Y
static const int32_t blockSizes[blockSizeCount] = {
	16,	  // 0
	32,	  // 1
	64,	  // 2
	96,	  // 3
	128,  // 4
	160,  // 5
	192,  // 6
	224,  // 7
	256,  // 8
	320,  // 9
	384,  // 10
	448,  // 11
	512,  // 12
	640,  // 13
};

// �C�ӂ̊��蓖�ăT�C�Y��K�؂ȃX���b�g�Ƀ}�b�v����
struct SizeMap {
	SizeMap() {
		int32_t j = 0;
		values[0] = 0;
		for (int32_t i = 1; i <= maxBlockSize; ++i) {
			assert(j < blockSizeCount);
			if (i <= blockSizes[j]) {
				values[i] = (uint8_t)j;
			}
			else {
				++j;
				values[i] = (uint8_t)j;
			}
		}
	}
	uint8_t values[maxBlockSize + 1];
};

static const SizeMap sizeMap;

struct Chunk {
	int32_t blockSize;
	Block* blocks;
};

struct Block {
	Block* next;
};

BlockAllocator::BlockAllocator()
{
	assert(blockSizeCount < UCHAR_MAX);

	m_chunkSpace = chunkArrayIncrement;
	m_chunkCount = 0;
	m_chunks = (Chunk*)myAlloc(m_chunkSpace * sizeof(Chunk));

	memset(m_chunks, 0, m_chunkSpace * sizeof(Chunk));
	memset(m_freeLists, 0, sizeof(m_freeLists));

}

BlockAllocator::~BlockAllocator()
{
	for (int32_t i = 0; i < m_chunkCount; ++i) {
		myFree(m_chunks[i].blocks);
	}
	myFree(m_chunks);
}

void* BlockAllocator::Allocate(uint32_t size)
{
	if (size == 0) {
		return nullptr;
	}

	assert(0 < size);

	if (size > maxBlockSize) {
		return myAlloc(size);
	}

	int32_t index = sizeMap.values[size];
	assert(0 <= index && index < blockSizeCount);

	if (m_freeLists[index]) {
		// �t���[���X�g�̐擪�̃u���b�N���擾
		Block* block = m_freeLists[index];

		// �t���[���X�g�̐擪���擾�����u���b�N�̎��̃u���b�N�ɍX�V�B
		m_freeLists[index] = block->next;

		return block;
	}
	else {

		if (m_chunkCount == m_chunkSpace) {
			// �Â��`�����N�̃|�C���^��ۑ�
			Chunk* oldChunks = m_chunks;

			// chunkSpace���g��
			m_chunkSpace += chunkArrayIncrement;

			// �V�����T�C�Y�Ń��������m��
			m_chunks = (Chunk*)myAlloc(m_chunkSpace * sizeof(Chunk));

			// �Â��`�����N�̃f�[�^��V�����`�����N�ɃR�s�[
			memcpy(m_chunks, oldChunks, m_chunkCount * sizeof(Chunk));

			// �V�����̈��������
			memset(m_chunks + m_chunkCount, 0, chunkArrayIncrement * sizeof(Chunk));

			// �Â��̈�̃����������
			myFree(oldChunks);
		}

		// �V�����`�����N�̃|�C���^�����擾
		Chunk* chunk = m_chunks + m_chunkCount;

		// �V�����`�����N���Ƀu���b�N�̃������̈���m��
		chunk->blocks = (Block*)myAlloc(chunkSize);

		/// �u���b�N�̃����N�t��

		// �Ή�����u���b�N�T�C�Y���擾
		int32_t blockSize = blockSizes[index];

		// �`�����N�Ƀu���b�N�T�C�Y��ݒ�
		chunk->blockSize = blockSize;

		// �`�����N���Ɋ܂܂��u���b�N�����v�Z
		int32_t blockCount = chunkSize / blockSize;

		assert(blockCount * blockSize <= chunkSize);
		for (int32_t i = 0; i < blockCount - 1; ++i) {
			// ���݂̃C���f�b�N�X�ɑΉ�����u���b�N�̃|�C���^���擾
			Block* block = (Block*)((int8_t*)chunk->blocks + blockSize * i);

			// ���̃C���f�b�N�X�ɑΉ�����u���b�N�̃|�C���^���擾
			Block* next = (Block*)((int8_t*)chunk->blocks + blockSize * (i + 1));

			// ���E�̃u���b�N���玟�̃u���b�N�ւ̃|�C���^�������N�t��
			block->next = next;
		}
		// �Ō�̃u���b�N�ɑΉ�����|�C���^���擾
		Block* last = (Block*)((int8_t*)chunk->blocks + blockSize * (blockCount - 1));

		last->next = nullptr;

		// �t���[���X�g�ɐV�����`�����N�̍ŏ��̃u���b�N��ǉ�
		m_freeLists[index] = chunk->blocks->next;

		// �`�����N���𑝂₷
		++m_chunkCount;

		return chunk->blocks;

	}
}

void BlockAllocator::Free(void* p, uint32_t size)
{
	if (size == 0) {
		return;
	}

	assert(0 < size);

	if (size > maxBlockSize) {
		myFree(p);
		return;
	}

	int32_t index = sizeMap.values[size];
	assert(0 <= index && index < blockSizeCount);

	// ������郁�����̃|�C���^���L���X�g
	Block* block = (Block*)p;

	// ������ꂽ�u���b�N���t���[���X�g�̐擪�ɑ}��
	block->next = m_freeLists[index];

	// �t���[���X�g�̐擪���X�V
	m_freeLists[index] = block;

}

void BlockAllocator::Clear()
{
	for (int32_t i = 0; i < m_chunkCount; ++i) {
		myFree(m_chunks[i].blocks);
	}

	m_chunkCount = 0;
	memset(m_chunks, 0, m_chunkSpace * sizeof(Chunk));
	memset(m_freeLists, 0, sizeof(m_freeLists));
}
