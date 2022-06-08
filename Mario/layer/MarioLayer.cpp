//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "MarioLayer.hpp"

using namespace Mario;

/* --------------------------------------------------------------------------------------
 Scene String. Each char represnet a tile described below
 0 : END OF LINE TO GET NUMBER OF ELEMENT IN A ROW
 G : Ground
 | : Castel Brick
 o : Castel Gate
 u : castel Gate Domb
 . : Castel Domb
 l : Castel Windlow Left
 r : Castel Window Right
 S : Steps
 - : Bridge
 ! : Pipe Base
 Y : Pipe Head
 X : Bricks
 B : Bonus
 ( : Cloud (
 ^ : Cloud ^
 ) : Cloud )
 < : Grass <
 v : Grass v
 > : Grass >
 { : Forest {
 } : Forest }
 * : Forest *
 1 : Forest 1
 2 : Forest 2
 3 : Forest 3
/ -------------------------------------------------------------------------------------- */
// TODO: For now making number of row even to make position of y integral
static std::string s_MapTiles =
"                                                                                                                                                                                                                                                                                                                            0"
"                                                                                                                                                                                                                                                                                                                            0"
"                                  (^)                                                                  (^)                                                                                           (^^^)                                                                         (^^)                                     0"
"       (^)                                              (^^^)                                                                                  (^^)                                                                                           (^)                                                                  .        0"
"                                                                                                                                                                                                                                                                                                                  ...       0"
"                                                                                                                                                                                                                                                                                                                  |u|       0"
"                                                                                                                                                                                                                                                                                                                  |o|       0"
"                                                                                                                                                                                                                                                                                                                .......     0"
"                                               B                  XXXX                             B            XBX                                                                 B                                                                 B                                                         |u|r|u|     0"
"                                                                                                                                                                                                                                                                                                                |o|||o|     0"
"                                                                                                                                                                                                                                                                                                              ...........   0"
"                                                                                                                                                                                                                                                                                                              |||||||||||   0"
"                                                                                                                                                                                                                                                                                                              |l|u|r|u|l|   0"
"      XBXBX                                  XBXBX              XBXXXXBX                           S           XXXXX               B                                           B    B    B                                                          XBXBX                                                     |||o|||o|||   0"
"                                                                                Y                 SS                                                                                                                          S  S                                                       S                  ............... 0"
"                                                                                !                SSS                                                   S                                                 Y                   SS  SS                                                     SS                  ||||||||||||||| 0"
"                                                          Y                     !               SSSS                               Y                  SS        *                                        !                  SSS  SSS       *          Y                                SSS                  |u||u||u||u||u| 0"
"  <v>          S                           <v>      S     !       S  S          !     <vv>     SSSSS                    <vvv>      !          <v>    SSS       {1}                                       !                 SSSS  SSSS     {1}         !         <v>                   SSSS         <v>      |o||o||o||o||0| 0"
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG--------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGG--------------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG0"
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG                    GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGG                          GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG0"
;

/// Get entity name from Char code
/// @param type Char of time map
static std::string GetEntityNameFromChar(char type) {
    switch(type) {
        case 'G' : return "Ground"; break;
            
        case '|' : return "Castel Brick";        break;
        case 'o' : return "Castel Gate";         break;
        case 'u' : return "castel Gate Domb";    break;
        case '.' : return "Castel Domb";         break;
        case 'l' : return "Castel Windlow Left"; break;
        case 'r' : return "Castel Window Right"; break;
            
        case 'S' : return "Steps";     break;
        case '-' : return "Bridge";    break;
        case '!' : return "Pipe Base"; break;
        case 'Y' : return "Pipe";      break;
        case 'X' : return "Bricks";    break;
        case 'B' : return "Bonus";     break;
            
        case '<' : return "Grass <";   break;
        case 'v' : return "Grass v";   break;
        case '>' : return "Grass >>";  break;
            
        case '(' : return "Cloud (";    break;
        case '^' : return "Cloud ^";    break;
        case ')' : return "Cloud )>";   break;
            
        case '}' : return "Grass }";    break;
        case '{' : return "Grass {";    break;
        case '*' : return "Grass *";    break;
        case '1' : return "Grass 1";    break;
        case '2' : return "Grass 2";    break;
        case '3' : return "Grass 3";    break;
    }
    IK_ASSERT(false, "Invalid Type");
    return "";
}

/// Theme color mario tile
enum ThemeColor {
    Brown = 0, Blue = 1, Grey = 2, Green = 3, MAX_COLOR = 4
};

/// Background data storage
struct BgData {
    /// Stores the data of single Tile
    struct Tile {
        bool IsRigid = true;
        std::shared_ptr<SubTexture> SubTexture;
        
        Tile(const std::shared_ptr<iKan::SubTexture>& subTexture, bool isRigid) : IsRigid(isRigid),  SubTexture(subTexture) {}
        
        static std::shared_ptr<Tile> Create(std::shared_ptr<iKan::SubTexture> subTexture, bool isRigid = true) {
            return std::make_shared<Tile>(subTexture, isRigid);
        }
    };
    
    ThemeColor CurrentTheme = ThemeColor::Brown;
    
    // Texture to store tile sprite sheet
    std::shared_ptr<Texture> Sprite;
    
    // Stores the map of each tile with Entity
    std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Tile>> m_TileEntityMap;
    
    // Map of Array of subtextures to the Char (defined in s_MapTiles) of their corresponding tile.
    // Sotring Subtextures of same tile with different color Array
    std::unordered_map<char, std::array<std::shared_ptr<SubTexture>, MAX_COLOR>> TilesColorMapWithChar;
    
    // Map of tile (property of tile that stores the subtexture also)
    // to the Char of their corresponding. This map stores the Tile information
    // that will be rendered in background
    // NOTE: s_BgData->TilesColorMapWithChar is just storing all the colors. These are not
    // getting rendered at all
    std::unordered_map<char, std::shared_ptr<Tile>> TileMap;
};
static BgData* s_BgData;

/// Chess Renderer Layer Constructor
MarioLayer::MarioLayer()
: Layer("Mario Renderer Layer") {
    IK_INFO("Creating '{0}' ...", m_Name);
    
    s_BgData = new BgData();
}

/// Chess Renderer Layer Destructor
MarioLayer::~MarioLayer() {
    IK_WARN("Destroying '{0}' !!!", m_Name);
    
    if (s_BgData)
        delete s_BgData;
}

/// Attach the Renderer Layer to the application
void MarioLayer::Attach() {
    IK_INFO("Attaching '{0}'", m_Name);
    
    // Change the Imgui Font
    auto& imguiLayer = Application::Get().GetImGuiLayer();
    imguiLayer.SetFont(AssetManager::GetClientAsset("/fonts/Mario.ttf"), AssetManager::GetClientAsset("fonts/Mario.ttf"));
    
    // Setup the Theme
    ImguiAPI::SetGreyThemeColors();
    
    // Viewport Data setup
    FrameBuffer::Specification spec;
    spec.Width = 2100;
    spec.Height = 900;
    spec.Color = { 0.1, 0.1, 0.1, 1.0 };
    spec.Attachments = {
        FrameBuffer::Attachment::TextureFormat::RGBA8,
        FrameBuffer::Attachment::TextureFormat::R32I,
        FrameBuffer::Attachment::TextureFormat::Depth24Stencil,
    };
    m_ViewportData.FrameBuffer = FrameBuffer::Create(spec);
    
    // Create the Scene for rendering
    m_Scene = Scene::Create();
    m_Scene->PlayScene();

    // Setup the scene Camera Entity
    m_CameraEntity = m_Scene->CreateEntity("Camera");
    auto& cameraComponent = m_CameraEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Orthographic);
    cameraComponent.Camera->SetOrthographicSize(18.0f);
    
    // Initialize tha Mario Data
    InitBackgroundData();
}

/// Update the renderer Layer each frame
void MarioLayer::Update(Timestep ts) {
    // If resize the window call the update the Scene View port and Frame buffer should be resized
    if (const FrameBuffer::Specification& spec = m_ViewportData.FrameBuffer->GetSpecification();
        (uint32_t)m_ViewportData.Size.x > 0 && (uint32_t)m_ViewportData.Size.y > 0 && // zero sized framebuffer is invalid
        (spec.Width != (uint32_t)m_ViewportData.Size.x || spec.Height != (uint32_t)m_ViewportData.Size.y)) {
        m_ViewportData.FrameBuffer->Resize((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);
        m_Scene->SetViewport((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);    }
    
    Renderer::ResetStatsEachFrame();

    m_ViewportData.FrameBuffer->Bind();

    Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
    m_Scene->Update(ts);
    m_ViewportData.UpdateMousePos();
    UpdateHoveredEntity();

    m_ViewportData.FrameBuffer->Unbind();
}

/// Render ; for Renderer Layer
void MarioLayer::RenderGui() {
    ImguiAPI::StartDcocking();
    
    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Chess Viewport");
    ImGui::PushID("Chess Viewport");
    
    m_ViewportData.Focused = ImGui::IsWindowFocused();
    m_ViewportData.Hovered = ImGui::IsWindowHovered();
    
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportData.Size = { viewportPanelSize.x, viewportPanelSize.y };
    
    size_t textureID = m_ViewportData.FrameBuffer->GetColorAttachmentIds()[0];
    PropertyGrid::Image((void*)textureID, { m_ViewportData.Size.x, m_ViewportData.Size.y }, { 0, 1 }, { 1, 0 });
    ImGui::PopStyleVar();

    m_ViewportData.UpdateBound();

    ImGui::PopID();
    ImGui::End(); // Viewport
    
    // TODO: Debug
    {
        ImguiAPI::FrameRate();
        Renderer::ImguiRendererStats();
        m_ViewportData.RenderImgui();
        
        ImGui::Begin("Debug");
        PropertyGrid::Float3("Camera Position", m_CameraEntity->GetComponent<TransformComponent>().Translation);
        ImGui::End();
    }
        
    ImguiAPI::EndDcocking();
}

/// Detach the Renderer Lyer
void MarioLayer::Detach() {
    IK_WARN("Detaching '{0}'", m_Name);
}

/// Handle the Events of Renderer Layer
void MarioLayer::EventHandler(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(MarioLayer::OnKeyPressed));
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(MarioLayer::OnWindowResize));
}

/// Kwy Press Event
/// @param event Key Press event handler
bool MarioLayer::OnKeyPressed(KeyPressedEvent& event) {
    return false;
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool MarioLayer::OnWindowResize(WindowResizeEvent& e) {
    Renderer::SetViewportSize(e.GetWidth(), e.GetHeight());
    return false;
}

/// Update Hovered Entity
void MarioLayer::UpdateHoveredEntity() {
    if (m_ViewportData.Hovered) {
        Renderer::GetEntityIdFromPixels(m_ViewportData.MousePosX, m_ViewportData.MousePosY, m_ViewportData.HoveredEntityID);
        m_ViewportData.HoveredEntity = (m_ViewportData.HoveredEntityID > m_Scene->GetMaxEntityId()) ? nullptr : m_Scene->GetEnitityFromId(m_ViewportData.HoveredEntityID);
    }
}

/// Initialize the Mario Background
void MarioLayer::InitBackgroundData() {
    IK_CLIENT_LOG_SEPARATOR();
    IK_INFO("    Initializing the background data");

    s_BgData->Sprite = Renderer::GetTexture(AssetManager::GetClientAsset("textures/MarioTile.png"));
    
    // Initialize all Subtextures for all theme
    StoreSubtextureOfEachTile();
    
    // Extract the map width. MAP Width should be same for each New line string
    size_t mapWidth    = s_MapTiles.find_first_of('0') + 1;
    uint32_t mapHeight = static_cast<uint32_t>(strlen(s_MapTiles.c_str())) / mapWidth;

    IK_INFO("    Creating Entity for each tile ");
    for (uint32_t y = 0; y < mapHeight; y++) {
        for (uint32_t x = 0; x < mapWidth; x++) {
            // Create entity if we have sub texture for the character we found in map
            if (char tileType = s_MapTiles[x + y * mapWidth]; s_BgData->TileMap.find(tileType) != s_BgData->TileMap.end()) {
                auto entity = m_Scene->CreateEntity(GetEntityNameFromChar(tileType));
                const auto& spriteEntity = entity->AddComponent<SpriteComponent>(s_BgData->TileMap[tileType]->SubTexture);
                const auto& spriteSize   = spriteEntity.SubTexture->GetSpriteSize();

                auto& tc = entity->GetComponent<TransformComponent>();
                
                tc.Translation = { x, (mapHeight / 2.0f) - y, 0.0f };
                tc.Scale       = { spriteSize.x, spriteSize.y , 0.0f};
            }
            else {
                IK_WARN("        Char {0} at position Row {1} and Column {2} is not found in Tile Map", tileType, y, x);
            }
        }
    }
    
}

/// Stores the subtexture from Sprite for each tile in init time
void MarioLayer::StoreSubtextureOfEachTile() {
    IK_INFO("        Storing the Subtextures for each Tile");

    // Ground SubTexutres
    {
        s_BgData->TilesColorMapWithChar['G'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 27.0f })); // Brown
        s_BgData->TilesColorMapWithChar['G'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 25.0f })); // Blue
        s_BgData->TilesColorMapWithChar['G'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 23.0f })); // Grey
        s_BgData->TilesColorMapWithChar['G'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 21.0f })); // Green
        
        s_BgData->TileMap['G'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['G'][s_BgData->CurrentTheme]);
    }
    
    // Bricks SubTextures
    {
        s_BgData->TilesColorMapWithChar['X'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 17.0f, 27.0f })); // Brown;
        s_BgData->TilesColorMapWithChar['X'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 17.0f, 25.0f })); // Blue;
        s_BgData->TilesColorMapWithChar['X'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 17.0f, 23.0f })); // Grey;
        s_BgData->TilesColorMapWithChar['X'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 17.0f, 21.0f })); // green;
        
        s_BgData->TileMap['X'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['X'][s_BgData->CurrentTheme]);
    }
    
    // Bonus SubTextures
    {
        // Unused bonus (from 0 -3)
        s_BgData->TilesColorMapWithChar['B'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 24.0f, 27.0f })); // Brown
        s_BgData->TilesColorMapWithChar['B'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 24.0f, 25.0f })); // Blue
        s_BgData->TilesColorMapWithChar['B'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 24.0f, 23.0f })); // Grey
        s_BgData->TilesColorMapWithChar['B'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 24.0f, 21.0f })); // Green
        
        // USed bonus (from 4 -7)
        s_BgData->TilesColorMapWithChar['B'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 27.0f, 27.0f })); // Brown
        s_BgData->TilesColorMapWithChar['B'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 27.0f, 25.0f })); // Blue
        s_BgData->TilesColorMapWithChar['B'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 27.0f, 23.0f })); // Grey
        s_BgData->TilesColorMapWithChar['B'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 27.0f, 21.0f })); // Green
        
        s_BgData->TileMap['B'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['B'][s_BgData->CurrentTheme]);
    }
    
    // Steps SubTextures
    {
        s_BgData->TilesColorMapWithChar['S'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 26.0f })); // Brown
        s_BgData->TilesColorMapWithChar['S'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 24.0f })); // Blue
        s_BgData->TilesColorMapWithChar['S'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 22.0f })); // Grey
        s_BgData->TilesColorMapWithChar['S'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 20.0f })); // Green
        
        s_BgData->TileMap['S'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['S'][s_BgData->CurrentTheme]);
    }
    
    // Bridge SubTexture
    {
        s_BgData->TilesColorMapWithChar['-'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 15.0f, 18.5f })); // Green
        s_BgData->TilesColorMapWithChar['-'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 15.0f, 14.5f })); // Orange
        s_BgData->TilesColorMapWithChar['-'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 15.0f, 10.5f })); // Grey
        s_BgData->TilesColorMapWithChar['-'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 15.0f,  8.5f })); // Pink
        
        s_BgData->TileMap['-'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['-'][s_BgData->CurrentTheme]);
    }
    
    // Pipes
    {
        // Heads
        s_BgData->TilesColorMapWithChar['Y'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 19.0f }, { 2.0f, 1.0f })); // Green
        s_BgData->TilesColorMapWithChar['Y'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 15.0f }, { 2.0f, 1.0f })); // Orange
        s_BgData->TilesColorMapWithChar['Y'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 13.0f }, { 2.0f, 1.0f })); // Grey
        s_BgData->TilesColorMapWithChar['Y'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f,  9.0f }, { 2.0f, 1.0f })); // Pink
        
        s_BgData->TileMap['Y'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['Y'][s_BgData->CurrentTheme]);
        
        // Base
        s_BgData->TilesColorMapWithChar['!'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 18.0f }, { 2.0f, 1.0f })); // Green
        s_BgData->TilesColorMapWithChar['!'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 14.0f }, { 2.0f, 1.0f })); // Orange
        s_BgData->TilesColorMapWithChar['!'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 12.0f }, { 2.0f, 1.0f })); // Grey
        s_BgData->TilesColorMapWithChar['!'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f,  8.0f }, { 2.0f, 1.0f })); // Pink
        
        s_BgData->TileMap['!'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['!'][s_BgData->CurrentTheme]);
    }
    
    // Grass
    {
        s_BgData->TilesColorMapWithChar['<'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 11.0f, 18.0f })); // Green
        s_BgData->TilesColorMapWithChar['<'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 11.0f, 14.0f })); // Orange
        s_BgData->TilesColorMapWithChar['<'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 11.0f, 12.0f })); // Grey
        s_BgData->TilesColorMapWithChar['<'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 11.0f,  8.0f })); // Pink
        
        s_BgData->TileMap['<'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['<'][s_BgData->CurrentTheme], false);
        
        s_BgData->TilesColorMapWithChar['v'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 12.0f, 18.0f })); // Green
        s_BgData->TilesColorMapWithChar['v'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 12.0f, 14.0f })); // Orange
        s_BgData->TilesColorMapWithChar['v'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 12.0f, 12.0f })); // Grey
        s_BgData->TilesColorMapWithChar['v'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 12.0f,  8.0f })); // Pink
        
        s_BgData->TileMap['v'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['v'][s_BgData->CurrentTheme], false);
        
        s_BgData->TilesColorMapWithChar['>'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 13.0f, 18.0f })); // Green
        s_BgData->TilesColorMapWithChar['>'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 13.0f, 14.0f })); // Orange
        s_BgData->TilesColorMapWithChar['>'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 13.0f, 12.0f })); // Grey
        s_BgData->TilesColorMapWithChar['>'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 13.0f, 8.0f  })); // Pink
        
        s_BgData->TileMap['>'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['>'][s_BgData->CurrentTheme], false);
    }
    
    // Forest Grass
    {
        s_BgData->TilesColorMapWithChar['{'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 8.0f, 19.0f })); // Green
        s_BgData->TilesColorMapWithChar['{'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 8.0f, 15.0f })); // Orange
        s_BgData->TilesColorMapWithChar['{'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 8.0f, 13.0f })); // Grey
        s_BgData->TilesColorMapWithChar['{'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 8.0f,  9.0f })); // Pink
        
        s_BgData->TileMap['{'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['{'][s_BgData->CurrentTheme], false);
        
        s_BgData->TilesColorMapWithChar['}'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 10.0f, 19.0f })); // Green
        s_BgData->TilesColorMapWithChar['}'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 10.0f, 15.0f })); // Orange
        s_BgData->TilesColorMapWithChar['}'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 10.0f, 13.0f })); // Grey
        s_BgData->TilesColorMapWithChar['}'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 10.0f,  9.0f })); // Pink
        
        s_BgData->TileMap['}'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['}'][s_BgData->CurrentTheme], false);
        
        
        s_BgData->TilesColorMapWithChar['*'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 9.0f, 19.0f })); // Green
        s_BgData->TilesColorMapWithChar['*'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 9.0f, 15.0f })); // Orange
        s_BgData->TilesColorMapWithChar['*'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 9.0f, 13.0f })); // Grey
        s_BgData->TilesColorMapWithChar['*'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 9.0f,  9.0f })); // Pink
        
        s_BgData->TileMap['*'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['*'][s_BgData->CurrentTheme], false);
        
        s_BgData->TilesColorMapWithChar['1'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 8.0f, 18.0f })); // Green
        s_BgData->TilesColorMapWithChar['1'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 8.0f, 14.0f })); // Orange
        s_BgData->TilesColorMapWithChar['1'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 8.0f, 12.0f })); // Grey
        s_BgData->TilesColorMapWithChar['1'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 8.0f,  8.0f })); // Pink
        
        s_BgData->TileMap['1'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['1'][s_BgData->CurrentTheme], false);
        
        s_BgData->TilesColorMapWithChar['2'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 9.0f, 18.0f })); // Green
        s_BgData->TilesColorMapWithChar['2'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 9.0f, 14.0f })); // Orange
        s_BgData->TilesColorMapWithChar['2'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 9.0f, 12.0f })); // Grey
        s_BgData->TilesColorMapWithChar['2'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 9.0f,  8.0f })); // Pink
        
        s_BgData->TileMap['2'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['2'][s_BgData->CurrentTheme], false);
        
        s_BgData->TilesColorMapWithChar['3'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 10.0f, 18.0f })); // Green
        s_BgData->TilesColorMapWithChar['3'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 10.0f, 14.0f })); // Orange
        s_BgData->TilesColorMapWithChar['3'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 10.0f, 12.0f })); // Grey
        s_BgData->TilesColorMapWithChar['3'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 10.0f,  8.0f })); // Pink
        
        s_BgData->TileMap['3'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['3'][s_BgData->CurrentTheme], false);
    }
    
    // Cloud
    {
        s_BgData->TilesColorMapWithChar['('][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        s_BgData->TilesColorMapWithChar['('][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        s_BgData->TilesColorMapWithChar['('][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        s_BgData->TilesColorMapWithChar['('][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 0.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        s_BgData->TileMap['('] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['('][s_BgData->CurrentTheme], false);
        
        s_BgData->TilesColorMapWithChar['^'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 1.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        s_BgData->TilesColorMapWithChar['^'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 1.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        s_BgData->TilesColorMapWithChar['^'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 1.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        s_BgData->TilesColorMapWithChar['^'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 1.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        s_BgData->TileMap['^'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar['^'][s_BgData->CurrentTheme], false);
        
        s_BgData->TilesColorMapWithChar[')'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 2.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        s_BgData->TilesColorMapWithChar[')'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 2.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        s_BgData->TilesColorMapWithChar[')'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 2.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        s_BgData->TilesColorMapWithChar[')'][ThemeColor::Green] = (SubTexture::CreateFromCoords(s_BgData->Sprite, { 2.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        s_BgData->TileMap[')'] = BgData::Tile::Create(s_BgData->TilesColorMapWithChar[')'][s_BgData->CurrentTheme], false);
    }
    
    // Castel
    {
        s_BgData->TileMap['.'] = BgData::Tile::Create(SubTexture::CreateFromCoords(s_BgData->Sprite, { 19.0f, 25.0f }), false);
        s_BgData->TileMap['u'] = BgData::Tile::Create(SubTexture::CreateFromCoords(s_BgData->Sprite, { 20.0f, 24.0f }), false);
        s_BgData->TileMap['o'] = BgData::Tile::Create(SubTexture::CreateFromCoords(s_BgData->Sprite, { 21.0f, 24.0f }), false);
        s_BgData->TileMap['|'] = BgData::Tile::Create(SubTexture::CreateFromCoords(s_BgData->Sprite, { 21.0f, 25.0f }), false);
        s_BgData->TileMap['l'] = BgData::Tile::Create(SubTexture::CreateFromCoords(s_BgData->Sprite, { 20.0f, 25.0f }), false);
        s_BgData->TileMap['r'] = BgData::Tile::Create(SubTexture::CreateFromCoords(s_BgData->Sprite, { 22.0f, 25.0f }), false);
    }
}
