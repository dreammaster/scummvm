//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
//
//
//
//=============================================================================
#ifndef AGS_ENGINE_AC__ROOMSTATUS_H
#define AGS_ENGINE_AC__ROOMSTATUS_H

#include "ac/roomobject.h"
#include "game/roomstruct.h"
#include "game/interactions.h"
#include "util/string_types.h"

// Forward declaration
namespace AGS { namespace Shared { class Stream; } }
using AGS::Shared::Stream;
using AGS::Shared::Interaction;

// This struct is saved in the save games - it contains everything about
// a room that could change
struct RoomStatus {
    int   beenhere;
    int   numobj;
    RoomObject obj[MAX_ROOM_OBJECTS];
    short flagstates[MAX_FLAGS];
    int   tsdatasize;
    char* tsdata;
    Interaction intrHotspot[MAX_ROOM_HOTSPOTS];
    Interaction intrObject [MAX_ROOM_OBJECTS];
    Interaction intrRegion [MAX_ROOM_REGIONS];
    Interaction intrRoom;

    Shared::StringIMap roomProps;
    Shared::StringIMap hsProps[MAX_ROOM_HOTSPOTS];
    Shared::StringIMap objProps[MAX_ROOM_OBJECTS];
    // [IKM] 2012-06-22: not used anywhere
#ifdef UNUSED_CODE
    EventBlock hscond[MAX_ROOM_HOTSPOTS];
    EventBlock objcond[MAX_ROOM_OBJECTS];
    EventBlock misccond;
#endif
    char  hotspot_enabled[MAX_ROOM_HOTSPOTS];
    char  region_enabled[MAX_ROOM_REGIONS];
    short walkbehind_base[MAX_WALK_BEHINDS];
    int   interactionVariableValues[MAX_GLOBAL_VARIABLES];

    RoomStatus();
    ~RoomStatus();

    void FreeScriptData();
    void FreeProperties();

    void ReadFromFile_v321(Shared::Stream *in);
    void ReadRoomObjects_Aligned(Shared::Stream *in);
    void ReadFromSavegame(Shared::Stream *in);
    void WriteToSavegame(Shared::Stream *out) const;
};

// Replaces all accesses to the roomstats array
RoomStatus* getRoomStatus(int room);
// Used in places where it is only important to know whether the player
// had previously entered the room. In this case it is not necessary
// to initialise the status because a player can only have been in
// a room if the status is already initialised.
bool isRoomStatusValid(int room);
void resetRoomStatuses();

#endif // AGS_ENGINE_AC__ROOMSTATUS_H
