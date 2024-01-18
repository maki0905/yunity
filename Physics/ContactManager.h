#pragma once

#include <cstdint>

class Contact;
class ContactFilter;
class ContactListener;
class BlockAllocator;

class ContactManager
{
public:
	ContactManager();

	void AddPair(void* proxyUserDataA, void* proxyUserDataB);

	void FindNewContacts();

	void Destroy(Contact* c);

	void Collide();

public:
	BroadPhase m_broadPhase;
	Contact* m_contactList;
	uint32_t m_contactCount;
	ContactFilter* m_contactFilter;
	ContactListener* m_contactListener;
	BlockAllocator* m_allocator;


};

