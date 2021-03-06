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

#include <AzCore/Math/Vector3.h>
#include <AzCore/Math/Quaternion.h>
#include <AzCore/std/string/string.h>
#include <EMotionFX/Source/EMotionFXConfig.h>
#include <EMotionFX/Source/AnimGraphNode.h>
#include <EMotionFX/Source/TransformSpace.h>


namespace EMotionFX
{
    class EMFX_API BlendTreeGetTransformNode
        : public AnimGraphNode
    {
    public:
        AZ_RTTI(BlendTreeGetTransformNode, "{97E4FC12-7468-4F06-AF93-2FE2472E2B59}", AnimGraphNode)
        AZ_CLASS_ALLOCATOR_DECL

        // input ports
        enum
        {
            INPUTPORT_POSE                  = 0
        };

        enum
        {
            PORTID_INPUT_POSE               = 0
        };

        // output ports
        enum
        {
            OUTPUTPORT_TRANSLATION          = 0,
            OUTPUTPORT_ROTATION             = 1,
            OUTPUTPORT_SCALE                = 2
        };

        enum
        {
            PORTID_OUTPUT_TRANSLATION      = 0,
            PORTID_OUTPUT_ROTATION         = 1,
            PORTID_OUTPUT_SCALE            = 2
        };

        class EMFX_API UniqueData
            : public AnimGraphNodeData
        {
            EMFX_ANIMGRAPHOBJECTDATA_IMPLEMENT_LOADSAVE
        public:
            AZ_CLASS_ALLOCATOR_DECL

            UniqueData(AnimGraphNode* node, AnimGraphInstance* animGraphInstance)
            : AnimGraphNodeData(node, animGraphInstance)
            , m_nodeIndex(MCORE_INVALIDINDEX32)
            , m_mustUpdate(true) {}
            ~UniqueData() {}

        public:
            uint32          m_nodeIndex;
            bool            m_mustUpdate;
        };

        BlendTreeGetTransformNode();
        ~BlendTreeGetTransformNode();

        void Reinit() override;
        bool InitAfterLoading(AnimGraph* animGraph) override;

        void OnUpdateUniqueData(AnimGraphInstance* animGraphInstance) override;

        uint32 GetVisualColor() const override                  { return MCore::RGBA(255, 0, 0); }

        const char* GetPaletteName() const override;
        AnimGraphObject::ECategory GetPaletteCategory() const override;

        static void Reflect(AZ::ReflectContext* context);

    private:
        void UpdateUniqueData(AnimGraphInstance* animGraphInstance, UniqueData* uniqueData);
        void Output(AnimGraphInstance* animGraphInstance) override;

        using ActorNode = AZStd::pair<AZStd::string, int>;
        ActorNode           m_actorNode;
        ETransformSpace     m_transformSpace;
    };
}   // namespace EMotionFX
