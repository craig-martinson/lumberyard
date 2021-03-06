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
#include "EMotionFXConfig.h"
#include "AnimGraphNode.h"


namespace EMotionFX
{


    /**
    *
    */
    class EMFX_API BlendTreeRotationMath2Node
        : public AnimGraphNode
    {
    public:
        AZ_RTTI(BlendTreeRotationMath2Node, "{7DDDBAA4-1FD5-47B0-8E34-BA27C1F52210}", AnimGraphNode)
        AZ_CLASS_ALLOCATOR_DECL

        enum
        {
            INPUTPORT_X = 0,
            INPUTPORT_Y = 1,
            OUTPUTPORT_RESULT_QUATERNION = 0
        };

        enum
        {
            PORTID_INPUT_X = 0,
            PORTID_INPUT_Y = 1,
            PORTID_OUTPUT_QUATERNION = 0
        };

        enum EMathFunction : AZ::u8
        {
            MATHFUNCTION_MULTIPLY = 0,
            MATHFUNCTION_INVERSE_MULTIPLY = 1,
            MATHFUNCTION_NUMFUNCTIONS
        };

        BlendTreeRotationMath2Node();
        ~BlendTreeRotationMath2Node();

        void Reinit() override;
        bool InitAfterLoading(AnimGraph* animGraph) override;

        void SetMathFunction(EMathFunction func);

        uint32 GetVisualColor() const override { return MCore::RGBA(0, 123, 167); }

        const char* GetPaletteName() const override;
        AnimGraphObject::ECategory GetPaletteCategory() const override;

        void SetDefaultValue(const MCore::Quaternion& value);

        static void Reflect(AZ::ReflectContext* context);

    private:
        typedef void (MCORE_CDECL * BlendTreeRotationMath2Function)(const MCore::Quaternion& inputX, const MCore::Quaternion& inputY, MCore::Quaternion* quaternionOutput);

        AZ::Quaternion                  m_defaultValue;
        EMathFunction                   m_mathFunction;
        BlendTreeRotationMath2Function  m_calculateFunc;

        void Output(AnimGraphInstance* animGraphInstance) override;

        void ExecuteMathLogic(EMotionFX::AnimGraphInstance * animGraphInstance);

        static void MCORE_CDECL CalculateMultiply(const MCore::Quaternion& inputA, const MCore::Quaternion& inputB, MCore::Quaternion* quaternionOutput);
        static void MCORE_CDECL CalculateInverseMultiply(const MCore::Quaternion& inputA, const MCore::Quaternion& inputB, MCore::Quaternion* quaternionOutput);
    };
}   // namespace EMotionFX
