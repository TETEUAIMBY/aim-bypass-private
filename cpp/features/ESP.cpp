#include "ESP.h"
#include "../render/Render.h"
#include <cmath>
#include <cstdio>

ESP::ESP()
{
}

ESP::~ESP()
{
}

void ESP::Update()
{
    if (!m_bEnabled)
        return;

    ScanPlayers();

    for (const auto& player : m_Players)
    {
        if (m_bOnlyVisible && !player.visible)
            continue;

        if (m_bShowBox)
            DrawPlayerBox(player);

        if (m_bShowSkeleton)
            DrawPlayerSkeleton(player);

        DrawPlayerInfo(player);
    }
}

void ESP::ScanPlayers()
{
    m_Players.clear();
    // TODO: Implementar lógica de scanning de jogadores FiveM
    // Usar memoria read para encontrar handles dos players
    // Calcular distância, health, visibilidade
}

bool ESP::WorldToScreen(float worldX, float worldY, float worldZ, float& screenX, float& screenY)
{
    // TODO: Implementar world to screen conversion
    // Usar matriz de view/projection do jogo
    return true;
}

void ESP::DrawPlayerBox(const ESPPlayer& player)
{
    if (!WorldToScreen(player.x, player.y, player.z, (float&)player.screenX, (float&)player.screenY))
        return;

    float boxWidth = 30.0f * (500.0f / player.distance);
    float boxHeight = 60.0f * (500.0f / player.distance);
    float boxX = player.screenX - (boxWidth / 2.0f);
    float boxY = player.screenY - (boxHeight / 2.0f);

    // Desenhar box com cor baseada em saúde
    float healthPercent = player.health / 100.0f;
    float r = 1.0f - healthPercent;
    float g = healthPercent;
    float b = 0.0f;

    Render::DrawRect(boxX, boxY, boxWidth, boxHeight, {r, g, b, 1.0f}, 2.0f);
}

void ESP::DrawPlayerSkeleton(const ESPPlayer& player)
{
    if (!WorldToScreen(player.x, player.y, player.z, (float&)player.screenX, (float&)player.screenY))
        return;

    // Desenhar skeleton
    // Conectar ossos com linhas
    
    // Definição simplificada de ossos (em produção, usar offsets reais do jogo)
    std::vector<std::pair<int, int>> bones = {
        {0, 1},   // Head to Neck
        {1, 2},   // Neck to Chest
        {2, 3},   // Chest to Spine
        {1, 4},   // Neck to Left Shoulder
        {1, 5},   // Neck to Right Shoulder
        {4, 6},   // Left Shoulder to Left Elbow
        {5, 7},   // Right Shoulder to Right Elbow
        {6, 8},   // Left Elbow to Left Wrist
        {7, 9},   // Right Elbow to Right Wrist
        {2, 10},  // Chest to Left Hip
        {2, 11},  // Chest to Right Hip
        {10, 12}, // Left Hip to Left Knee
        {11, 13}, // Right Hip to Right Knee
        {12, 14}, // Left Knee to Left Ankle
        {13, 15}, // Right Knee to Right Ankle
    };

    for (const auto& bone : bones)
    {
        // TODO: Calcular posições dos ossos no mundo
        // Desenhar linhas conectando os ossos
    }
}

void ESP::DrawPlayerInfo(const ESPPlayer& player)
{
    if (!WorldToScreen(player.x, player.y, player.z, (float&)player.screenX, (float&)player.screenY))
        return;

    float infoX = player.screenX + 40.0f;
    float infoY = player.screenY - 30.0f;
    float lineHeight = 15.0f;

    // Desenhar nome
    if (m_bShowNames)
    {
        Render::DrawText(infoX, infoY, player.name, {1.0f, 1.0f, 1.0f, 1.0f}, 11);
        infoY += lineHeight;
    }

    // Desenhar distância
    if (m_bShowDistance)
    {
        char distanceText[32];
        sprintf_s(distanceText, sizeof(distanceText), "%.1f m", player.distance);
        Render::DrawText(infoX, infoY, distanceText, {0.7f, 0.7f, 0.7f, 1.0f}, 10);
        infoY += lineHeight;
    }

    // Desenhar health
    if (m_bShowHealth)
    {
        DrawHealthBar(infoX, infoY, 50.0f, 8.0f, player.health);
        char healthText[32];
        sprintf_s(healthText, sizeof(healthText), "HP: %.0f", player.health);
        Render::DrawText(infoX, infoY + 12.0f, healthText, {1.0f, 1.0f, 1.0f, 1.0f}, 9);
    }
}

void ESP::DrawHealthBar(float x, float y, float width, float height, float health)
{
    // Background (vermelho)
    Render::DrawRectFilled(x, y, width, height, {0.2f, 0.0f, 0.0f, 0.8f});

    // Health bar (verde até 50%, amarelo depois, vermelho no final)
    float healthPercent = health / 100.0f;
    float barWidth = width * healthPercent;

    float r = 1.0f - healthPercent;
    float g = healthPercent;
    float b = 0.0f;

    if (health > 50.0f)
    {
        r = 0.0f;
        g = 1.0f;
    }
    else if (health > 25.0f)
    {
        r = 1.0f;
        g = 1.0f;
        b = 0.0f;
    }
    else
    {
        r = 1.0f;
        g = 0.0f;
        b = 0.0f;
    }

    Render::DrawRectFilled(x, y, barWidth, height, {r, g, b, 1.0f});
}