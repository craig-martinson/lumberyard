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

// include required files
#include "EMotionFXConfig.h"
#include "BaseObject.h"
#include <MCore/Source/Array.h>
#include <MCore/Source/StringIdPool.h>
#include <MCore/Source/Endian.h>

// MCore forward declarations
MCORE_FORWARD_DECLARE(DiskFile);


namespace EMotionFX
{
    // forward declarations
    class Actor;

    /**
     * A node map maps a set of nodes from one given node name to another.
     * This can create a linkage/mapping between two different hierarchies. For example the node "Bip01 L Head" in one Actor could be mapped to
     * a node named "Head" in another Actor. A node map is for example used during retargeting of motions who's hierarchies are different.
     * The first name is always the key, and the second name is the name it is mapped to.
     * You should not have any duplicated firstName entries. The multiple entries can have the same second name though.
     */
    class EMFX_API NodeMap
        : public BaseObject
    {
        AZ_CLASS_ALLOCATOR_DECL

    public:
        struct MapEntry
        {
            uint32  mFirstNameID;   /**< The first name ID, which is the primary key in the map. */
            uint32  mSecondNameID;  /**< The second name ID. */

            MapEntry()
                : mFirstNameID(MCORE_INVALIDINDEX32)
                , mSecondNameID(MCORE_INVALIDINDEX32) {}
        };

        static NodeMap* Create();

        // prealloc space in the map
        void Reserve(uint32 numEntries);
        void Resize(uint32 numEntries);

        // get data
        uint32 GetNumEntries() const;
        const char* GetFirstName(uint32 entryIndex) const;
        const char* GetSecondName(uint32 entryIndex) const;
        const AZStd::string& GetFirstNameString(uint32 entryIndex) const;
        const AZStd::string& GetSecondNameString(uint32 entryIndex) const;
        bool GetHasEntry(const char* firstName) const;
        uint32 FindEntryIndexByName(const char* firstName) const;
        uint32 FindEntryIndexByNameID(uint32 firstNameID) const;
        const char* FindSecondName(const char* firstName) const;
        void FindSecondName(const char* firstName, AZStd::string* outString);

        // set/modify
        void SetFirstName(uint32 entryIndex, const char* name);
        void SetSecondName(uint32 entryIndex, const char* name);
        void SetEntry(uint32 entryIndex, const char* firstName, const char* secondName);
        void AddEntry(const char* firstName, const char* secondName);
        void SetEntry(const char* firstName, const char* secondName, bool addIfNotExists);
        void RemoveEntryByIndex(uint32 entryIndex);
        void RemoveEntryByName(const char* firstName);
        void RemoveEntryByNameID(uint32 firstNameID);

        // filename
        void SetFileName(const char* fileName);
        const char* GetFileName() const;
        const AZStd::string& GetFileNameString() const;

        // source actor
        void SetSourceActorFileName(const char* fileName);
        void SetSourceActor(Actor* actor, bool deleteExisting = true);
        Actor* GetSourceActor() const;
        void SetAutoDeleteSourceActor(bool autoDelete);
        bool GetAutoDeleteSourceActor() const;
        const char* GetSourceActorFileName() const;
        const AZStd::string& GetSourceActorFileNameString() const;

        // auto load source actor, based on the set source actor filename, and store the pointer also inside this class
        bool LoadSourceActor();

        // saving
        bool Save(const char* fileName, MCore::Endian::EEndianType targetEndianType) const;

    private:
        MCore::Array<MapEntry>  mEntries;                   /**< The array of entries. */
        AZStd::string           mFileName;                  /**< The filename. */
        AZStd::string           mSourceActorFileName;       /**< The source actor filename. */
        Actor*                  mSourceActor;               /**< The source actor. */
        bool                    mAutoDeleteSourceActor;     /**< Automatically delete the source actor object on destruction? */

        // constructor and destructor
        NodeMap();
        ~NodeMap();

        // file saving
        bool WriteFileString(MCore::DiskFile* f, const AZStd::string& textToSave, MCore::Endian::EEndianType targetEndianType) const;
        uint32 CalcFileChunkSize() const;
        uint32 CalcFileStringSize(const AZStd::string& text) const;
    };
}   // namespace EMotionFX
