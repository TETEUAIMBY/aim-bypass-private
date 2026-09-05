#pragma once
#include <windows.h>
#include <vector>
#include <string>

struct ESPSkeleton
{
    float x, y;  // Screen position
    float x2, y2;
};

struct ESPPlayer
{
    int entityHandle;
    float x, y, z;           // World position
    float screenX, screenY;  // Screen position
    float health;
    char name[64];
    float distance;
    bool visible;
    std::vector<ESPSkeleton> skeleton;  // Bones
};

class ESP
{
public:
    ESP();
    ~ESP();

    void Update();
    void Enable(bool enable) { m_bEnabled = enable; }
    bool IsEnabled() const { return m_bEnabled; }

    void EnableBox(bool enable) { m_bShowBox = enable; }
    void EnableSkeleton(bool enable) { m_bShowSkeleton = enable; }
    void EnableDistance(bool enable) { m_bShowDistance = enable; }
    void EnableHealth(bool enable) { m_bShowHealth = enable; }
    void EnableNames(bool enable) { m_bShowNames = enable; }
    void EnableOnlyVisible(bool enable) { m_bOnlyVisible = enable; }

    bool IsBoxEnabled() const { return m_bShowBox; }
    bool IsSkeletonEnabled() const { return m_bShowSkeleton; }
    bool IsDistanceEnabled() const { return m_bShowDistance; }
    bool IsHealthEnabled() const { return m_bShowHealth; }
    bool IsNamesEnabled() const { return m_bShowNames; }
    bool IsOnlyVisibleEnabled() const { return m_bOnlyVisible; }

private:
    bool m_bEnabled = false;
    bool m_bShowBox = true;
    bool m_bShowSkeleton = true;
    bool m_bShowDistance = true;
    bool m_bShowHealth = true;
    bool m_bShowNames = true;
    bool m_bOnlyVisible = true;

    std::vector<ESPPlayer> m_Players;

    void ScanPlayers();
    void DrawPlayerBox(const ESPPlayer& player);
    void DrawPlayerSkeleton(const ESPPlayer& player);
    void DrawPlayerInfo(const ESPPlayer& player);
    void DrawHealthBar(float x, float y, float width, float height, float health);
    bool WorldToScreen(float worldX, float worldY, float worldZ, float& screenX, float& screenY);
};