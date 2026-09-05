#include "Menu.h"
#include <d3d11.h>
#include "../render/Render.h"

extern void SelfDelete();  // Declaração externa

Menu::Menu()
{
    m_bInitialized = true;
}

Menu::~Menu()
{
}

void Menu::Render()
{
    if (!m_bInitialized)
        return;

    // Fundo do menu (Dark background com transparência)
    Render::DrawRectFilled(m_MenuX, m_MenuY, m_MenuWidth, m_MenuHeight, 
                           {0.05f, 0.05f, 0.05f, 0.9f});

    // Border magenta
    Render::DrawRect(m_MenuX, m_MenuY, m_MenuWidth, m_MenuHeight, 
                     {1.0f, 0.0f, 1.0f, 1.0f}, 2.0f);

    RenderHeader();
    RenderTabs();

    // Renderizar conteúdo da aba atual
    switch (m_CurrentTab)
    {
    case MenuTab::AIMBOT:
        RenderAimbotTab();
        break;
    case MenuTab::ESP:
        RenderESPTab();
        break;
    case MenuTab::VISUALS:
        RenderVisualsTab();
        break;
    case MenuTab::SETTINGS:
        RenderSettingsTab();
        break;
    case MenuTab::INFO:
        RenderInfoTab();
        break;
    }
}

void Menu::RenderHeader()
{
    // Título
    Render::DrawText(m_MenuX + 15, m_MenuY + 10, 
                     "AIM BYPASS PRIVATE", 
                     {1.0f, 0.0f, 1.0f, 1.0f}, 14);
    
    Render::DrawText(m_MenuX + 15, m_MenuY + 30,
                     "v2.0 | FiveM Cheat",
                     {0.7f, 0.7f, 0.7f, 1.0f}, 10);

    // Linha de separação
    Render::DrawLine(m_MenuX, m_MenuY + 50, m_MenuX + m_MenuWidth, m_MenuY + 50,
                     {1.0f, 0.0f, 1.0f, 0.5f}, 1.0f);
}

void Menu::RenderTabs()
{
    float tabX = m_MenuX + 10;
    float tabY = m_MenuY + 60;
    float tabWidth = 90;
    float tabHeight = 25;

    const char* tabs[] = { "Aimbot", "ESP", "Visuals", "Settings", "Info" };

    for (int i = 0; i < 5; i++)
    {
        MenuTab tab = static_cast<MenuTab>(i);
        {1.0f, 0.0f, 1.0f, 1.0f} color = (m_CurrentTab == tab) 
                      ? {1.0f, 0.0f, 1.0f, 1.0f}  // Magenta
                      : {0.5f, 0.5f, 0.5f, 1.0f}; // Gray

        // Tab background
        if (m_CurrentTab == tab)
            Render::DrawRectFilled(tabX, tabY, tabWidth, tabHeight, 
                                   {1.0f, 0.0f, 1.0f, 0.2f});
        
        // Tab border
        Render::DrawRect(tabX, tabY, tabWidth, tabHeight, color, 1.5f);
        
        // Tab text
        Render::DrawText(tabX + 5, tabY + 5, tabs[i], color, 11);

        tabX += tabWidth + 5;
    }
}

void Menu::RenderAimbotTab()
{
    float contentX = m_MenuX + 15;
    float contentY = m_MenuY + 100;
    float lineHeight = 25;

    // Aimbot Options
    Render::DrawText(contentX, contentY, "✓ Enabled", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Show FOV", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Sticky Aimbot", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Only Visible", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Aimbot Humanizer", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Ignore NPCs", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight + 10;

    // Sliders
    Render::DrawText(contentX, contentY, "Smooth Horizontal (X): 100", 
                     {1.0f, 0.0f, 1.0f, 1.0f}, 11);
    Render::DrawRectFilled(contentX, contentY + 18, 200, 8, 
                           {1.0f, 0.0f, 1.0f, 0.3f});
    Render::DrawRectFilled(contentX, contentY + 18, 100, 8, 
                           {1.0f, 0.0f, 1.0f, 1.0f});
    contentY += 35;

    Render::DrawText(contentX, contentY, "Smooth Vertical (Y): 100", 
                     {1.0f, 0.0f, 1.0f, 1.0f}, 11);
    Render::DrawRectFilled(contentX, contentY + 18, 200, 8, 
                           {1.0f, 0.0f, 1.0f, 0.3f});
    Render::DrawRectFilled(contentX, contentY + 18, 100, 8, 
                           {1.0f, 0.0f, 1.0f, 1.0f});
    contentY += 35;

    Render::DrawText(contentX, contentY, "Max Distance: 500.000", 
                     {1.0f, 0.0f, 1.0f, 1.0f}, 11);
    Render::DrawRectFilled(contentX, contentY + 18, 200, 8, 
                           {1.0f, 0.0f, 1.0f, 0.3f});
    Render::DrawRectFilled(contentX, contentY + 18, 150, 8, 
                           {1.0f, 0.0f, 1.0f, 1.0f});
}

void Menu::RenderESPTab()
{
    float contentX = m_MenuX + 15;
    float contentY = m_MenuY + 100;
    float lineHeight = 25;

    Render::DrawText(contentX, contentY, "✓ Enabled", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Skeleton", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Show Names", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Show FOV", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Only Visible", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Box", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Distance Display", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "✓ Health Bar", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 11);
}

void Menu::RenderVisualsTab()
{
    float contentX = m_MenuX + 15;
    float contentY = m_MenuY + 100;

    Render::DrawText(contentX, contentY, "Visual Settings", 
                     {1.0f, 1.0f, 1.0f, 1.0f}, 12);
    contentY += 30;

    Render::DrawText(contentX, contentY, "Coming Soon...", 
                     {1.0f, 1.0f, 0.0f, 1.0f}, 11);
}

void Menu::RenderSettingsTab()
{
    float contentX = m_MenuX + 15;
    float contentY = m_MenuY + 100;
    float lineHeight = 30;

    Render::DrawText(contentX, contentY, "Settings", 
                     {1.0f, 1.0f, 1.0f, 1.0f}, 12);
    contentY += lineHeight + 10;

    // Self Delete Button
    Render::DrawText(contentX, contentY, "[ SELF DELETE ]", 
                     {1.0f, 0.0f, 0.0f, 1.0f}, 12);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "Clique para fechar o cheat", 
                     {0.7f, 0.7f, 0.7f, 1.0f}, 9);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "Limpa todos os logs automaticamente", 
                     {0.7f, 0.7f, 0.7f, 1.0f}, 9);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "Mantém o FiveM aberto", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 9);
}

void Menu::RenderInfoTab()
{
    float contentX = m_MenuX + 15;
    float contentY = m_MenuY + 100;
    float lineHeight = 25;

    Render::DrawText(contentX, contentY, "AIM BYPASS PRIVATE v2.0", 
                     {1.0f, 0.0f, 1.0f, 1.0f}, 12);
    contentY += lineHeight + 10;

    Render::DrawText(contentX, contentY, "Developer: TETEUAIMBY", 
                     {0.7f, 0.7f, 0.7f, 1.0f}, 10);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "Windows 10 Pro | FiveM", 
                     {0.7f, 0.7f, 0.7f, 1.0f}, 10);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "DirectX 11 | C++ 17", 
                     {0.7f, 0.7f, 0.7f, 1.0f}, 10);
    contentY += lineHeight + 10;

    Render::DrawText(contentX, contentY, "INSERT - Toggle Menu", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 10);
    contentY += lineHeight;

    Render::DrawText(contentX, contentY, "ESC - Close Menu", 
                     {0.0f, 1.0f, 0.0f, 1.0f}, 10);
}

void Menu::Update()
{
    // Update menu state
}
