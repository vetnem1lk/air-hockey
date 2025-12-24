#include "Game.h"

Game::Game() : m_physics()
{
    m_world.puck.position = {0.f, 0.f};
    m_world.puck.velocity = {0.5f, 0.3f};

    m_world.leftPaddle.position  = {-0.8f, 0.f};
    m_world.rightPaddle.position = { 0.8f, 0.f};
}

void Game::update(double dt)
{
    m_accumulator += dt;

    while (m_accumulator >= FIXED_DT) {
        fixedUpdate(FIXED_DT);
        m_accumulator -= FIXED_DT;
    }
}

void Game::fixedUpdate(double fixedDt)
{
    m_physics.simulate(m_world, fixedDt);
}
