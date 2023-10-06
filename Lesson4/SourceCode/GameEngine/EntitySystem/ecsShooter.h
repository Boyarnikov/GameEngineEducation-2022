#pragma once
#include "flecs.h"
#include "ecsPhys.h"


struct Gun
{
	float reloadTime;
	float reloadTimer;
	bool reloaded;
	unsigned int capacity;
	unsigned int bulletsInOneShot;
};

struct Bullet { 
	float radius;
};

struct Enemy { };

struct Bonus { };

struct DespawnOnGround
{
	float despawnTime;
	float despawnTimer;
};

void register_ecs_shooter_systems(flecs::world &ecs);

