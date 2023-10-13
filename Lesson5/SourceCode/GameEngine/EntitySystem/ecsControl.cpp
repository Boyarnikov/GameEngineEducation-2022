#include "ecsControl.h"
#include "ecsSystems.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "../InputHandler.h"
#include "../ScriptSystem/IScriptProxy.h"

void register_ecs_control_systems(flecs::world &ecs)
{
  static auto scriptsQuery = ecs.query<ScriptPtr>();
  static auto inputQuery = ecs.query<InputHandlerPtr>();
  ecs.system<Velocity, const Speed, const Controllable>()
    .each([&](flecs::entity e, Velocity &vel, const Speed &spd, const Controllable &)
    {
      inputQuery.each([&](InputHandlerPtr input)
      {
        scriptsQuery.each([&](ScriptPtr script)
            {
                vel.x = spd * script.ptr->Update(input.ptr->GetInputState().test(eIC_GoLeft), input.ptr->GetInputState().test(eIC_GoRight), vel.x, e.delta_time());
            });
      });
    });

  ecs.system<const Position, Velocity, const Controllable, const BouncePlane, const JumpSpeed>()
    .each([&](const Position &pos, Velocity &vel, const Controllable &, const BouncePlane &plane, const JumpSpeed &jump)
    {
      inputQuery.each([&](InputHandlerPtr input)
      {
        constexpr float planeEpsilon = 0.1f;
        if (plane.x*pos.x + plane.y*pos.y + plane.z*pos.z < plane.w + planeEpsilon)
          if (input.ptr->GetInputState().test(eIC_Jump))
            vel.y = jump.val;
      });
    });
}

