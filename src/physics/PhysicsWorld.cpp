#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() = default;

void PhysicsWorld::simulate(World& world, double dt)
{
    moveObjects(world, dt);
    handleCollisions(world);
}

void PhysicsWorld::moveObjects(World& world, double dt)
{
    // Плейн физика: позиция += скорость * dt
    world.puck.position.x += world.puck.velocity.x * static_cast<float>(dt);
    world.puck.position.y += world.puck.velocity.y * static_cast<float>(dt);

    world.leftPaddle.position.x += world.leftPaddle.velocity.x * static_cast<float>(dt);
    world.leftPaddle.position.y += world.leftPaddle.velocity.y * static_cast<float>(dt);

    world.rightPaddle.position.x += world.rightPaddle.velocity.x * static_cast<float>(dt);
    world.rightPaddle.position.y += world.rightPaddle.velocity.y * static_cast<float>(dt);

    // применяем трение
    world.puck.velocity.x *= (1.0f - m_friction);
    world.puck.velocity.y *= (1.0f - m_friction);

    world.leftPaddle.velocity.x *= (1.0f - m_friction);
    world.leftPaddle.velocity.y *= (1.0f - m_friction);

    world.rightPaddle.velocity.x *= (1.0f - m_friction);
    world.rightPaddle.velocity.y *= (1.0f - m_friction);
}

void PhysicsWorld::handleCollisions(World& world)
{
    // шайба и борта
    if (world.puck.position.x - world.puck.radius < world.rink.left ||
        world.puck.position.x + world.puck.radius > world.rink.right) {
        world.puck.velocity.x = -world.puck.velocity.x * m_restitution;
        // корректируем позицию
        if (world.puck.position.x - world.puck.radius < world.rink.left)
            world.puck.position.x = world.rink.left + world.puck.radius;
        else
            world.puck.position.x = world.rink.right - world.puck.radius;
    }

    if (world.puck.position.y - world.puck.radius < world.rink.bottom ||
        world.puck.position.y + world.puck.radius > world.rink.top) {
        world.puck.velocity.y = -world.puck.velocity.y * m_restitution;
        if (world.puck.position.y - world.puck.radius < world.rink.bottom)
            world.puck.position.y = world.rink.bottom + world.puck.radius;
        else
            world.puck.position.y = world.rink.top - world.puck.radius;
    }

    // коллизии с битами
    auto handleCircleCollision = [this](Vec2& pos1, Vec2& vel1, const Vec2& pos2, float r1, float r2){
        Vec2 delta{pos1.x - pos2.x, pos1.y - pos2.y};
        float dist2 = delta.x*delta.x + delta.y*delta.y;
        float r = r1 + r2;
        if (dist2 < r*r) {
            float dist = std::sqrt(dist2);
            if (dist == 0.f) dist = 0.0001f;
            float overlap = r - dist;

            // корректировка позиции
            pos1.x += (delta.x / dist) * overlap;
            pos1.y += (delta.y / dist) * overlap;

            // отражение скорости
            float dot = vel1.x*delta.x + vel1.y*delta.y;
            vel1.x -= 2 * dot / dist2 * delta.x * m_restitution;
            vel1.y -= 2 * dot / dist2 * delta.y * m_restitution;
        }
    };

    handleCircleCollision(world.puck.position, world.puck.velocity, world.leftPaddle.position, world.puck.radius, world.leftPaddle.radius);
    handleCircleCollision(world.puck.position, world.puck.velocity, world.rightPaddle.position, world.puck.radius, world.rightPaddle.radius);
}

void PhysicsWorld::applyPlayerInput(World &world, const InputState &input, double dt)
{
    float speed = 1.0f;

    Vec2 dir{0.f, 0.f};
    if(input.up)    dir.y += 1.f;
    if(input.down)  dir.y -= 1.f;
    if(input.left)  dir.x -= 1.f;
    if(input.right) dir.x += 1.f;

    world.leftPaddle.velocity = dir.normalized() * speed;
    world.leftPaddle.position += world.leftPaddle.velocity * static_cast<float>(dt);

    // ограничиваем движение внутри rink
    if(world.leftPaddle.position.x - world.leftPaddle.radius < world.rink.left)
        world.leftPaddle.position.x = world.rink.left + world.leftPaddle.radius;
    if(world.leftPaddle.position.x + world.leftPaddle.radius > 0.f)
        world.leftPaddle.position.x = 0.f - world.leftPaddle.radius;

    if(world.leftPaddle.position.y - world.leftPaddle.radius < world.rink.bottom)
        world.leftPaddle.position.y = world.rink.bottom + world.leftPaddle.radius;
    if(world.leftPaddle.position.y + world.leftPaddle.radius > world.rink.top)
        world.leftPaddle.position.y = world.rink.top - world.leftPaddle.radius;
}

