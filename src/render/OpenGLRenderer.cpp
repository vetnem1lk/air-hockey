#include "OpenGLRenderer.h"
#include "world/World.h"
#include <glad/glad.h>

OpenGLRenderer::OpenGLRenderer(const IWindow& window)
    : m_window(window)
{
    setupResources();
}

OpenGLRenderer::~OpenGLRenderer()
{
    glDeleteBuffers(1, &m_circleVBO);
    glDeleteBuffers(1, &m_rinkLinesVBO);
    glDeleteBuffers(1, &m_iceVBO);
}

void OpenGLRenderer::setupResources() {
    // --- 1. Круги ---
    m_circleShader = std::make_unique<Shader>("assets/shaders/gl_circle.vert", "assets/shaders/gl_circle.frag");

    const int segments = 64;
    std::vector<Vec2> circleVerts;
    circleVerts.push_back({0.0f, 0.0f});
    for (int i = 0; i <= segments; ++i) {
        float angle = i / float(segments) * 2.0f * glm::pi<float>();
        circleVerts.push_back({std::cos(angle), std::sin(angle)});
    }
    m_circleVertexCount = static_cast<uint32_t>(circleVerts.size());

    glGenBuffers(1, &m_circleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVerts.size() * sizeof(Vec2), circleVerts.data(), GL_STATIC_DRAW);

    m_circleVAO = std::make_unique<VertexArray>();
    VertexBufferLayout layout;
    layout.push<float>(2);
    m_circleVAO->addBuffer(m_circleVBO, layout);
    m_circleVAO->unbind();

    // --- 2. Линии поля ---
    m_lineShader = std::make_unique<Shader>("assets/shaders/gl_line.vert", "assets/shaders/gl_line.frag");

    std::vector<Vec2> rinkLineVertices;
    m_worldRink = Rink{}; // default rink

    // границы
    rinkLineVertices.push_back({m_worldRink.left, m_worldRink.bottom});
    rinkLineVertices.push_back({m_worldRink.right, m_worldRink.bottom});
    rinkLineVertices.push_back({m_worldRink.right, m_worldRink.top});
    rinkLineVertices.push_back({m_worldRink.left, m_worldRink.top});

    // центр
    rinkLineVertices.push_back({0.0f, m_worldRink.bottom});
    rinkLineVertices.push_back({0.0f, m_worldRink.top});

    // ворота
    float goalWidth = m_goalLineRadius;
    rinkLineVertices.push_back({m_worldRink.left, goalWidth});
    rinkLineVertices.push_back({m_worldRink.left, -goalWidth});
    rinkLineVertices.push_back({m_worldRink.right, goalWidth});
    rinkLineVertices.push_back({m_worldRink.right, -goalWidth});

    m_rinkLinesVertexCount = static_cast<uint32_t>(rinkLineVertices.size());

    glGenBuffers(1, &m_rinkLinesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_rinkLinesVBO);
    glBufferData(GL_ARRAY_BUFFER, rinkLineVertices.size() * sizeof(Vec2), rinkLineVertices.data(), GL_STATIC_DRAW);

    m_rinkLinesVAO = std::make_unique<VertexArray>();
    VertexBufferLayout lineLayout;
    lineLayout.push<float>(2);
    m_rinkLinesVAO->addBuffer(m_rinkLinesVBO, lineLayout);
    m_rinkLinesVAO->unbind();

    // --- 3. Лед ---
    m_iceShader = std::make_unique<Shader>("assets/shaders/gl_ice.vert", "assets/shaders/gl_ice.frag");
    m_iceTexture = std::make_unique<Texture>("assets/textures/ice.png");

    float iceVerts[] = {
        m_worldRink.left,  m_worldRink.bottom,  0.0f, 0.0f,
        m_worldRink.right, m_worldRink.bottom,  1.0f, 0.0f,
        m_worldRink.right, m_worldRink.top,     1.0f, 1.0f,
        m_worldRink.left,  m_worldRink.top,     0.0f, 1.0f
    };

    glGenBuffers(1, &m_iceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_iceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(iceVerts), iceVerts, GL_STATIC_DRAW);

    m_iceVAO = std::make_unique<VertexArray>();
    VertexBufferLayout iceLayout;
    iceLayout.push<float>(2); // pos
    iceLayout.push<float>(2); // uv
    m_iceVAO->addBuffer(m_iceVBO, iceLayout);
    m_iceVAO->unbind();
}

void OpenGLRenderer::beginFrame() {
    glViewport(0, 0, m_window.width(), m_window.height());
    glClearColor(0.05f, 0.05f, 0.07f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_projection = calculateProjection();
}

void OpenGLRenderer::render(const World& world) {
    // Лед
    m_iceShader->use();
    m_iceShader->setMat4("uProjection", m_projection);
    m_iceTexture->bind(0);
    m_iceVAO->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_iceVAO->unbind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Линии
    m_lineShader->use();
    m_lineShader->setMat4("uProjection", m_projection);
    m_lineShader->setVec3("uColor", {0.8f, 0.2f, 0.2f});
    m_rinkLinesVAO->bind();
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINES, 4, 2);
    glDrawArrays(GL_LINES, 6, 2);
    glDrawArrays(GL_LINES, 8, 2);
    m_rinkLinesVAO->unbind();

    // Шайба и палки
    m_circleShader->use();
    m_circleShader->setMat4("uProjection", m_projection);
    m_circleVAO->bind();

    drawCircle(world.puck.position, world.puck.radius, {1.0f, 1.0f, 1.0f});
    drawCircle(world.leftPaddle.position, world.leftPaddle.radius, {0.2f, 0.4f, 1.0f});
    drawCircle(world.rightPaddle.position, world.rightPaddle.radius, {1.0f, 0.3f, 0.3f});

    m_circleVAO->unbind();
    glDisable(GL_BLEND);
}

void OpenGLRenderer::drawCircle(const Vec2& position, float radius, const glm::vec3& color) {
    m_circleShader->setVec2("uInstancePos", position);
    m_circleShader->setFloat("uInstanceRadius", radius);
    m_circleShader->setVec3("uColor", color);

    glDrawArrays(GL_TRIANGLE_FAN, 0, m_circleVertexCount);
}

glm::mat4 OpenGLRenderer::calculateProjection() const
{
    int w = m_window.width();
    int h = m_window.height();
    float windowAspect = float(w) / float(h);

    float worldWidth  = m_worldRink.width();
    float worldHeight = m_worldRink.height();
    float worldAspect = worldWidth / worldHeight;

    float left, right, bottom, top;

    if (windowAspect > worldAspect) {
        float scale = windowAspect / worldAspect;
        left   = m_worldRink.left * scale;
        right  = m_worldRink.right * scale;
        bottom = m_worldRink.bottom;
        top    = m_worldRink.top;
    } else {
        float scale = worldAspect / windowAspect;
        left   = m_worldRink.left;
        right  = m_worldRink.right;
        bottom = m_worldRink.bottom * scale;
        top    = m_worldRink.top * scale;
    }

    return glm::ortho(left, right, bottom, top);
}

void OpenGLRenderer::endFrame() {
    // Здесь можно добавить SwapBuffers, если бы мы не делали это в Application
}
