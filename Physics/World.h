//#pragma once
//
//#include <cstdint>
//
//#include "Vector3.h"
//
//#include "ContactManager.h"
//#include "BlockAllocator.h"
//#include "StackAllocator.h"
//#include "WorldCallBacks.h"
//#include "TimeStep.h"
//
//struct BodyDef;
//struct Color;
//struct JointDef;
//
//class Body;
//class Fixture;
//class Joint;
//class Controller;
//
//class World
//{
//public:
//
//	World();
//	~World();
//
//	// 衝突フィルターの設定
//	void SetContactFilter();
//
//	// 衝突イベントの設定
//	void SetContactListener();
//
//	// ボディー生成
//	Body* CreateBody(const BodyDef* def);
//	// ボディー破棄
//	void DestroyBody(Body* body);
//
//	// ジョイント生成
//	Joint* CreateJoint(const JointDef* def);
//	// ジョイント破棄
//	void DestroyJoint(Joint* joint);
//
//
//	void Step(float timeStep,
//		uint32_t velocityIterations,
//		uint32_t positionIterations);
//
//
//	void ClearForces();
//
//	Body* GetBodyList();
//	const Body* GetBodyList() const;
//
//	Joint* GetJointList();
//	const Joint* GetJointList() const;
//
//	Contact* GetContactList();
//	const Contact* GetContactList() const;
//
//	void SetAllowSleeping(bool flag);
//	bool GetAllowSleeping() const { return m_allowSleep; }
//
//	void SetWarmStarting(bool flag) { m_warmStarting = flag; }
//	bool GetWarmStarting() const { return m_warmStarting; }
//
//	void SetContinuousPhysics(bool flag) { m_continuousPhysics = flag; }
//	bool GetContinuousPhysics() const { return m_continuousPhysics; }
//
//	void SetSubStepping(bool flag) { m_subStepping = flag; }
//	bool GetSubStepping() const { return m_subStepping; }
//
//	uint32_t GetProxyCount() const;
//
//	uint32_t GetBodyCount() const;
//
//	uint32_t GetJointCount() const;
//
//	uint32_t GetContactCount() const;
//
//	uint32_t GetTreeHeight() const;
//
//	uint32_t GetTreeBalance() const;
//
//	float GetTreeQuality() const;
//
//	void SetGravity(const Vector3& gravity);
//
//	Vector3 GetGravity() const;
//
//	bool IsLocked() const;
//
//	void SetAutoClearForces(bool flag);
//
//	bool GetAutoClearForces() const;
//
//	void ShiftOrigin(const Vector3& newOrigin);
//
//	const ContactManager& GetContactManager() const;
//
//private:
//
//	friend class Body;
//	friend class Fixture;
//	friend class ContactManager;
//	friend class Controller;
//
//	void Solve(const TimeStep& step);
//	void SolveTOI(const TimeStep& step);
//
//	void DrawShape(Fixture* shape, const Transform& xf, const Color& color);
//
//	BlockAllocator m_blockAllocator;
//	StackAllocator m_stackAllocator;
//
//	ContactManager m_contactManager;
//
//	Body* m_bodyList;
//	Joint* m_jointList;
//
//	uint32_t m_bodyCount;
//	uint32_t m_jointCount;
//
//	Vector3 m_gravity;
//	bool m_allowSleep;
//
//	DestructionListener* m_destructionListener;
//
//	float m_inv_dt0;
//
//	bool m_newContacts;
//	bool m_locked;
//	bool m_clearForces;
//
//	bool m_warmStarting;
//	bool m_continuousPhysics;
//	bool m_subStepping;
//
//	bool m_stepComplete;
//
//};
//
