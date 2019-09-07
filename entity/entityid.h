#ifndef __ENTITY_ENTITYID_H__
#define __ENTITY_ENTITYID_H__

#include "strongid.h"

class EntityIdDiscriminant;
typedef StrongId<EntityIdDiscriminant, size_t> EntityId;
typedef StrongIdGenerator<EntityId> EntityIdGenerator;

#endif
