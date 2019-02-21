#ifndef FOLLOWER_EVENT_H
#define FOLLOWER_EVENT_H

extern struct FollowerAvatar gFollower;
void SpawnFollower(s16 x, s16 y, u8 nextDirection);
void MoveFollower(u8 nextMovementId);
bool8 CanSpawnFollower();

struct FollowerAvatar
{
    bool8 active:1;
    u8 eventObjectId;
    u8 nextMovementId;
};

#define EVENT_OBJ_ID_FOLLOWER 0xFC

#endif // FOLLOWER_EVENT_H