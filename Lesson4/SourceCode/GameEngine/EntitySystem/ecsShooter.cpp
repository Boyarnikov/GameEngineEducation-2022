#include "ecsPhys.h"
#include "ecsMesh.h"
#include "ecsShooter.h"
#include "ecsSystems.h"
#include "../InputHandler.h"

static float rand_flt(float from, float to)
{
    return from + (float(rand()) / RAND_MAX) * (to - from);
}

void register_ecs_shooter_systems(flecs::world& ecs)
{
    //for handling inputs
    static auto inputQuery = ecs.query<InputHandlerPtr>();

    ecs.system<const Position, Gun>()
        .each([&](flecs::entity e, const Position& pos, Gun& g) {
        if (!g.reloaded) {
            g.reloadTimer -= e.delta_time();
            if (g.reloadTimer < 0.f) {
                g.reloadTimer = g.reloadTime;
                g.reloaded = true;
            }
        }


        if (g.reloaded && (g.capacity > 0)) {
            inputQuery.each([&](InputHandlerPtr input)
                {                   
                    if (input.ptr->GetInputState().test(eIC_Fire)) {
                        if (g.reloaded) {
                            g.capacity -= 1;
                            g.reloaded = false;
                        }

                        for (int i = 0; i < g.bulletsInOneShot; i++)
                            e.world().entity()
                            .set(Position{ pos.x, pos.y + 0.5f, pos.z })
                            .set(Velocity{ rand_flt(-6.0f, 6.0f), 0.f, rand_flt(-6.0f, 6.0f) + 40.f })
                            .set(Gravity{ 0.f, -9.8065f, 0.f })
                            .set(BouncePlane{ 0.f, 1.f, 0.f, 0.f })
                            .set(Bounciness{ 0.01f })
                            .set(FrictionAmount{ 0.9f })
                            .set(Bullet{ 1.5f })
                            .add<CubeMesh>()
                            .set(DespawnOnGround{ 5.f, 5.f });
                    }
                    
                });
        }
    });


    ecs.system<const Position, const BouncePlane, DespawnOnGround, RenderProxyPtr>()
        .each([&](flecs::entity e, const Position& pos, const BouncePlane& plane, DespawnOnGround& d, const RenderProxyPtr& render) {
        if (plane.w + 0.5f > pos.x * plane.x + pos.y * plane.y + pos.z * plane.z) {
            d.despawnTime -= e.delta_time();
        }
        else {
            d.despawnTime = d.despawnTimer;
        }
        if (d.despawnTime < 0.f) {
            e.destruct();
            delete render.ptr;
        }
    });

    ecs.system<const Position, Enemy, RenderProxyPtr>()
        .each([&](flecs::entity e, const Position& p, Enemy& e_, const RenderProxyPtr& render) {

        bool killed = false;

        e.world().filter<Position, Bullet>().each([&](const Position& bp, const Bullet& b)
            {
                if ((p.x - bp.x) * (p.x - bp.x) + (p.y - bp.y) * (p.y - bp.y) + (p.z - bp.z) * (p.z - bp.z) < (b.radius) * (b.radius)) {

                    killed = true;
                }
            });

        if (killed) {
            if ( e.has<Bonus>()) {
                e.world().filter<Gun>().each([&](Gun& g)
                    {
                        g.bulletsInOneShot += 3;
                    });
            }
            e.destruct();
            delete render.ptr;
        }
        });

};