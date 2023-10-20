#include "EntitySystem.h"
#include "ecsSystems.h"
#include "ecsMesh.h"
#include "ecsControl.h"
#include "../tinyxml2.h"
#include "ecsPhys.h"


void EntitySystem::LoadXMLObject(const char* file) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(file);

    tinyxml2::XMLElement* ptr = doc.RootElement();

    if (ptr == nullptr) {
        auto entity = ecs.entity();

        entity.add<CubeMesh>();
        entity.set(Position{ 0.f, 0.f, 0.f });
        return;
    }
        

    auto entity = ecs.entity();
    
    if (tinyxml2::XMLElement* cube = ptr->FirstChildElement("cube")) {
        entity.add<CubeMesh>();
    }

    
    if (tinyxml2::XMLElement* pos = ptr->FirstChildElement("position")) {
        entity.set(Position{ pos->FirstChildElement("x")->FloatText(),
                             pos->FirstChildElement("y")->FloatText(),
                             pos->FirstChildElement("z")->FloatText() 
            });
    }
    

    if (tinyxml2::XMLElement* grav = ptr->FirstChildElement("gravity")) {
        entity.set(Gravity{ grav->FirstChildElement("x")->FloatText(),
                            grav->FirstChildElement("y")->FloatText(),
                            grav->FirstChildElement("z")->FloatText()
            });
    }

    if (tinyxml2::XMLElement* b = ptr->FirstChildElement("bounciness")) {
        entity.set(Bounciness{ b->FirstChildElement("v")->FloatText() });
    }
    
    if (tinyxml2::XMLElement* b_plane = ptr->FirstChildElement("bounce_plane")) {
        entity.set(BouncePlane{ b_plane->FirstChildElement("x")->FloatText(),
                                b_plane->FirstChildElement("y")->FloatText(),
                                b_plane->FirstChildElement("z")->FloatText()
            });
    }

    if (tinyxml2::XMLElement* vel = ptr->FirstChildElement("velocity")) {
        entity.set(Velocity{ vel->FirstChildElement("x")->FloatText(),
                             vel->FirstChildElement("y")->FloatText(),
                             vel->FirstChildElement("z")->FloatText()
            });
    }

    if (tinyxml2::XMLElement* spd = ptr->FirstChildElement("speed")) {
        entity.set(Speed{ spd->FirstChildElement("v")->FloatText() });
    }
    
}


EntitySystem::EntitySystem(RenderEngine* renderEngine, InputHandler* inputHandler)
{
    ecs.entity("inputHandler")
        .set(InputHandlerPtr{ inputHandler });
    ecs.entity("renderEngine")
        .set(RenderEnginePtr{ renderEngine });

    register_ecs_mesh_systems(ecs);
    register_ecs_control_systems(ecs);
    register_ecs_phys_systems(ecs);

    LoadXMLObject("../../../Assets/XmlObjects/CustomXMLCube.xml");

    /*
    auto cubeControl = ecs.entity()
        .set(Position{ 0.f, 0.f, 0.f })
        .set(Velocity{ 0.f, 0.f, 0.f })
        .set(Speed{ 10.f })
        .set(FrictionAmount{ 0.9f })
        .set(JumpSpeed{ 10.f })
        .set(Gravity{ 0.f, -9.8065f, 0.f })
        .set(BouncePlane{ 0.f, 1.f, 0.f, 0.f })
        .set(Bounciness{ 0.3f })
        .add<Controllable>()
        .add<CubeMesh>();

    auto cubeMoving = ecs.entity()
        .set(Position{ 0.f, 0.f, 0.f })
        .set(Velocity{ 0.f, 3.f, 0.f })
        .set(Gravity{ 0.f, -9.8065f, 0.f })
        .set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
        .set(Bounciness{ 1.f })
        .add<CubeMesh>();
    */
}

void EntitySystem::Update()
{
    ecs.progress();
}