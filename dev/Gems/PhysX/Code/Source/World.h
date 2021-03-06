/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <PxPhysicsAPI.h>
#include <AzFramework/Physics/SystemBus.h>
#include <AzFramework/Physics/World.h>

namespace PhysX
{
    /// PhysX specific implementation of generic physics API World class.
    class World
        : public Physics::WorldRequestBus::Handler
        , public physx::PxSimulationFilterCallback
        , public physx::PxSimulationEventCallback
    {
    public:

        AZ_CLASS_ALLOCATOR(World, AZ::SystemAllocator, 0);
        AZ_RTTI(World, "{C116A4D3-8843-45CA-9F32-F7B5CCB7F3AB}", Physics::World);

        World(AZ::Crc32 id, const Physics::WorldConfiguration& settings);
        ~World() override;

        physx::PxScene* GetNativeWorld() const { return m_world; }
        AZ::Crc32 GetWorldId() const { return m_worldId; }

        // Physics::World
        Physics::RayCastHit RayCast(const Physics::RayCastRequest& request) override;
        Physics::RayCastHit ShapeCast(const Physics::ShapeCastRequest& request) override;
        AZStd::vector<Physics::RayCastHit> RayCastMultiple(const Physics::RayCastRequest& request);
        AZStd::vector<Physics::RayCastHit> ShapeCastMultiple(const Physics::ShapeCastRequest& request) override;
        AZStd::vector<Physics::OverlapHit> Overlap(const Physics::OverlapRequest& request) override;
        void RegisterSuppressedCollision(const AZStd::shared_ptr<Physics::WorldBody>& body0,
            const AZStd::shared_ptr<Physics::WorldBody>& body1) override;
        void UnregisterSuppressedCollision(const AZStd::shared_ptr<Physics::WorldBody>& body0,
            const AZStd::shared_ptr<Physics::WorldBody>& body1) override;
        void AddBody(Physics::WorldBody& body) override;
        void RemoveBody(const Physics::WorldBody& body) override;
        void Update(float deltaTime) override;
        AZ::Crc32 GetNativeType() const override;
        void* GetNativePointer() const override;
        void SetEventHandler(Physics::WorldEventHandler* eventHandler) override;

        // physx::PxSimulationFilterCallback
        physx::PxFilterFlags pairFound(physx::PxU32 pairId, physx::PxFilterObjectAttributes attributes0,
            physx::PxFilterData filterData0, const physx::PxActor* actor0, const physx::PxShape* shape0,
            physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, const physx::PxActor* actor1,
            const physx::PxShape* shape1, physx::PxPairFlags& pairFlags);
        void pairLost(physx::PxU32 pairId, physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
            physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, bool objectRemoved);
        bool statusChange(physx::PxU32& pairId, physx::PxPairFlags& pairFlags, physx::PxFilterFlags& filterFlags);

        // physx::PxSimulationEventCallback
        void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
        void onWake(physx::PxActor** actors, physx::PxU32 count) override;
        void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
        void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
        void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
        void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

        void SetSimFunc(std::function<void(void*)> func) override;

        void AddBody(physx::PxActor* body);

        AZ::Vector3 GetGravity() override;
        void SetGravity(const AZ::Vector3& gravity) override;

    private:
        using ActorPair = AZStd::pair<const physx::PxActor*, const physx::PxActor*>;
        AZStd::unordered_set<ActorPair>::iterator FindSuppressedPair(const physx::PxActor* actor0, const physx::PxActor* actor1);

        physx::PxScene* m_world = nullptr;
        AZ::Crc32 m_worldId;

        AZStd::vector<physx::PxRaycastHit> m_raycastBuffer; ///< Maximum number of hits that can be stored from a single raycast.
        AZStd::vector<physx::PxSweepHit> m_sweepBuffer; ///< Maximum number of hits that can be stored from a single shapecast.
        AZStd::vector<physx::PxOverlapHit> m_overlapBuffer; ///< Maximum number of hits that can be stored from a single overlap query.

        AZStd::unordered_set<ActorPair> m_suppressedCollisionPairs; ///< Actor pairs with collision suppressed.

        float m_maxDeltaTime = 0.0f;
        float m_fixedDeltaTime = 0.0f;
        float m_accumulatedTime = 0.0f;

        //function pointer for simulating
        std::function<void(void *)> m_simFunc = nullptr;
        Physics::WorldEventHandler* m_eventHandler = nullptr;
    };
} // namespace PhysX