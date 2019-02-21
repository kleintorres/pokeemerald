#include "global.h"
#include "follower.h"
#include "event_object_movement.h"
#include "field_specials.h"
#include "event_scripts.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement_constants.h"
#include "constants/species.h"

static u8 TransformFollowerMovementId(u8 movementId, u8 nextMovementId);
static u8 GetFollowerPartyIndex();
static bool8 IsRunningMovementId(u8 movementId);
static bool8 IsWalkingMovementId(u8 movementId);
static u8 TransformMovementIdSpeed(u8 movementId);
static u8 SanatizeMovementId(u8 movementId);

EWRAM_DATA struct FollowerAvatar gFollower = {};

void SpawnFollower(s16 x, s16 y, u8 nextDirection)
{
    u8 followerObjectId;
    struct EventObject *followerEvent;
    struct EventObjectTemplate eventObjectTemplate;

    eventObjectTemplate.localId = EVENT_OBJ_ID_FOLLOWER;
    eventObjectTemplate.graphicsId = 0;
    eventObjectTemplate.unk2 = 0;
    eventObjectTemplate.x = x - 7;
    eventObjectTemplate.y = y - 7;
    eventObjectTemplate.elevation = 0;
    eventObjectTemplate.movementType = MOVEMENT_TYPE_NONE;
    eventObjectTemplate.movementRangeX = 0;
    eventObjectTemplate.movementRangeY = 0;
    eventObjectTemplate.trainerType = 0;
    eventObjectTemplate.trainerRange_berryTreeId = 0;
    eventObjectTemplate.script = NULL;
    followerObjectId = SpawnSpecialEventObject(&eventObjectTemplate);
    followerEvent = &gEventObjects[followerObjectId];
    followerEvent->facingDirectionLocked = 0;
    gFollower.eventObjectId = followerObjectId;
    gFollower.nextMovementId = GetWalkNormalMovementAction(nextDirection);
}

void MoveFollower(u8 nextMovementId)
{
    struct EventObject *followerEventObj = &gEventObjects[gFollower.eventObjectId];
    struct EventObject *playerEventObj = &gEventObjects[gPlayerAvatar.eventObjectId];
    u8 dir;

    if (followerEventObj->localId == EVENT_OBJ_ID_FOLLOWER)
    {
        if (EventObjectClearHeldMovementIfFinished(followerEventObj))
        {
            if (followerEventObj->currentCoords.x != playerEventObj->currentCoords.x || followerEventObj->currentCoords.y != playerEventObj->currentCoords.y)
            {
                nextMovementId = SanatizeMovementId(nextMovementId);
                dir = TransformFollowerMovementId(gFollower.nextMovementId, nextMovementId);
                EventObjectSetHeldMovement(followerEventObj, dir);
                gFollower.nextMovementId = nextMovementId;
            }
        }
    }
}

bool8 CanSpawnFollower()
{
    struct Pokemon *pokemon = &gPlayerParty[GetFollowerPartyIndex()];
    u8 i;

    // Comprueba que el Pokémon exista
    if (GetMonData(pokemon, MON_DATA_SPECIES2) == SPECIES_NONE)
    {
        return FALSE;
    }

    // Comprueba si se ha creado ya el evento del follower
    for (i = 0; i < EVENT_OBJECTS_COUNT; i++)
    {
        if (gEventObjects[i].active && gEventObjects[i].localId == EVENT_OBJ_ID_FOLLOWER)
        {
            return FALSE;
        }
    }

    return TRUE;
}

static u8 GetFollowerPartyIndex()
{
    u8 i;
    u16 species;
    for (i = 0; i < CalculatePlayerPartyCount(); i++)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);

        // Recupera el primer Pokémon que no sea un huevo y no esté debilitado
        if (species != SPECIES_EGG && species != SPECIES_NONE && GetMonData(&gPlayerParty[i], MON_DATA_HP) > 0)
        {
            return i;
        }
    }

    return 0;
}

static u8 TransformFollowerMovementId(u8 movementId, u8 nextMovementId)
{
    if ((IsRunningMovementId(movementId) && IsWalkingMovementId(nextMovementId)) ||
        (IsWalkingMovementId(movementId) && IsRunningMovementId(nextMovementId)))
        return TransformMovementIdSpeed(movementId);

    return movementId;
}

static bool8 IsRunningMovementId(u8 movementId)
{
    if (movementId == MOVEMENT_ACTION_WALK_FAST_RIGHT || movementId == MOVEMENT_ACTION_WALK_FAST_LEFT || movementId == MOVEMENT_ACTION_WALK_FAST_UP || movementId == MOVEMENT_ACTION_WALK_FAST_DOWN)
        return TRUE;
    return FALSE;
}

static bool8 IsWalkingMovementId(u8 movementId)
{
    if (movementId == MOVEMENT_ACTION_WALK_NORMAL_RIGHT || movementId == MOVEMENT_ACTION_WALK_NORMAL_LEFT || movementId == MOVEMENT_ACTION_WALK_NORMAL_UP || movementId == MOVEMENT_ACTION_WALK_NORMAL_DOWN)
        return TRUE;
    return FALSE;
}

static u8 TransformMovementIdSpeed(u8 movementId)
{
    switch (movementId)
    {
    case MOVEMENT_ACTION_WALK_NORMAL_RIGHT:
        return MOVEMENT_ACTION_WALK_FAST_RIGHT;
    case MOVEMENT_ACTION_WALK_NORMAL_LEFT:
        return MOVEMENT_ACTION_WALK_FAST_LEFT;
    case MOVEMENT_ACTION_WALK_NORMAL_UP:
        return MOVEMENT_ACTION_WALK_FAST_UP;
    case MOVEMENT_ACTION_WALK_NORMAL_DOWN:
        return MOVEMENT_ACTION_WALK_FAST_DOWN;
    case MOVEMENT_ACTION_WALK_FAST_RIGHT:
        return MOVEMENT_ACTION_WALK_NORMAL_RIGHT;
    case MOVEMENT_ACTION_WALK_FAST_LEFT:
        return MOVEMENT_ACTION_WALK_NORMAL_LEFT;
    case MOVEMENT_ACTION_WALK_FAST_UP:
        return MOVEMENT_ACTION_WALK_NORMAL_UP;
    case MOVEMENT_ACTION_WALK_FAST_DOWN:
        return MOVEMENT_ACTION_WALK_NORMAL_DOWN;
    }

    return movementId;
}

static u8 SanatizeMovementId(u8 movementId)
{
    switch (movementId)
    {
    case MOVEMENT_ACTION_PLAYER_RUN_RIGHT:
        return MOVEMENT_ACTION_WALK_FAST_RIGHT;
    case MOVEMENT_ACTION_PLAYER_RUN_LEFT:
        return MOVEMENT_ACTION_WALK_FAST_LEFT;
    case MOVEMENT_ACTION_PLAYER_RUN_UP:
        return MOVEMENT_ACTION_WALK_FAST_UP;
    case MOVEMENT_ACTION_PLAYER_RUN_DOWN:
        return MOVEMENT_ACTION_WALK_FAST_DOWN;
    }

    return movementId;
}