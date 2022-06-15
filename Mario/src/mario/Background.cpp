//
//  Background.cpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#include "Background.hpp"

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
 b : Used Bonus
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
"                                             XBXBX              XBXXXXBX                           S           XXXXX               B                                           B    B    B                                                          XBXBX                                                     |||o|||o|||   0"
"                                                                                Y                 SS                                                                                                                          S  S                                                       S                  ............... 0"
"                                                                                !                SSS                                                   S                                                 Y                   SS  SS                                                     SS                  ||||||||||||||| 0"
"                                                          Y                     !               SSSS                               Y                  SS        *                                        !                  SSS  SSS       *          Y                                SSS                  |u||u||u||u||u| 0"
"                                           <v>      S     !       S  S          !     <vv>     SSSSS                    <vvv>      !          <v>    SSS       {1}                                       !                 SSSS  SSSS     {1}         !         <v>                   SSSS         <v>      |o||o||o||o||0| 0"
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
        case 'b' : return "UsedBonus"; break;
            
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

/// Store the tile information
struct Tile {
    // Store the subtexture of each tile
    std::shared_ptr<SubTexture> SubTexture;
    
    // Constructors
    Tile(const std::shared_ptr<iKan::SubTexture>& subTexture) : SubTexture(subTexture) {}
    static std::shared_ptr<Tile> Create(const std::shared_ptr<iKan::SubTexture>& subTexture) { return std::make_shared<Tile>(subTexture); }
};

/// Store the renderer data responsible for mario background
struct MarioRendererData {
    // Texture to store tile sprite sheet
    std::shared_ptr<Texture> Sprite;

    // Map of Array of subtextures to the Char (defined in s_MapTiles) of their corresponding tile.
    // Sotring Subtextures of same tile with different color Array
    std::unordered_map<char, std::array<std::shared_ptr<SubTexture>, MAX_COLOR>> TilesColorMapWithChar;
    
    // Map of tile (property of tile that stores the subtexture also)
    // to the Char of their corresponding. This map stores the Tile information
    // that will be rendered in background
    // NOTE: s_BgData->TilesColorMapWithChar is just storing all the colors. These are not
    // getting rendered at all
    std::unordered_map<char, std::shared_ptr<Tile>> TileMap;

    // Init Renderer Data
    void Init(ThemeColor color);
};
static MarioRendererData* s_RendererData;

/// Initialize the background data
/// @param scene Scene pointer
/// @param color theme color
void Background::Init(const std::shared_ptr<Scene>& scene, ThemeColor color) {
    IK_CLIENT_LOG_SEPARATOR();
    IK_INFO("Initializing the background data");
    
    // 1. Allocate the memory for renderer data
    s_RendererData = new MarioRendererData();

    // 2. Create Sprite Texture
    s_RendererData->Sprite = Renderer::GetTexture(AssetManager::GetClientAsset("textures/MarioTile.png"), false, false);
    
    // 3. Initialize all Subtextures for all theme
    s_RendererData->Init(color);

    // 4. Create entities for each tile
    // Extract the map width. MAP Width should be same for each New line string
    size_t mapWidth    = s_MapTiles.find_first_of('0') + 1;
    uint32_t mapHeight = static_cast<uint32_t>(strlen(s_MapTiles.c_str())) / mapWidth;

    IK_INFO("Creating Entity for each tile ");
    for (uint32_t y = 0; y < mapHeight; y++) {
        for (uint32_t x = 0; x < mapWidth; x++) {
            // Create entity if we have sub texture for the character we found in map
            if (char tileType = s_MapTiles[x + y * mapWidth]; s_RendererData->TileMap.find(tileType) != s_RendererData->TileMap.end()) {
                auto entity = scene->CreateEntity(GetEntityNameFromChar(tileType));
                const auto& spriteEntity = entity->AddComponent<SpriteComponent>(s_RendererData->TileMap[tileType]->SubTexture);
                const auto& spriteSize   = spriteEntity.SubTexture->GetSpriteSize();

                auto& tc = entity->GetComponent<TransformComponent>();
                
                tc.Translation = { x, (mapHeight / 2.0f) - y, 0.0f };
                tc.Scale       = { spriteSize.x, spriteSize.y , 0.0f};
            }
            else {
                if (tileType != ' ' || tileType != '0') // No need to validate Space
                    IK_WARN("    Char {0} at position Row {1} and Column {2} is not found in Tile Map", tileType, y, x);
            }
        }
    }

}

/// Shutdown the bg data
/// @param scene Scene pointer
void Background::Shutdown(const std::shared_ptr<Scene>& scene) {
    if (s_RendererData)
        delete s_RendererData;
}

/// Stores the subtexture from Sprite for each tile in init time
void MarioRendererData::Init(ThemeColor color) {
    IK_INFO("    Storing the Subtextures for each Tile");

    // Ground SubTexutres
    {
        TilesColorMapWithChar['G'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 27.0f })); // Brown
        TilesColorMapWithChar['G'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 25.0f })); // Blue
        TilesColorMapWithChar['G'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 23.0f })); // Grey
        TilesColorMapWithChar['G'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 21.0f })); // Green
        
        TileMap['G'] = Tile::Create(TilesColorMapWithChar['G'][color]);
    }
    
    // Bricks SubTextures
    {
        TilesColorMapWithChar['X'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 17.0f, 27.0f })); // Brown;
        TilesColorMapWithChar['X'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 17.0f, 25.0f })); // Blue;
        TilesColorMapWithChar['X'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 17.0f, 23.0f })); // Grey;
        TilesColorMapWithChar['X'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 17.0f, 21.0f })); // green;
        
        TileMap['X'] = Tile::Create(TilesColorMapWithChar['X'][color]);
    }
    
    // Bonus SubTextures
    {
        // Unused bonus (from 0 - 3)
        TilesColorMapWithChar['B'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 24.0f, 27.0f })); // Brown
        TilesColorMapWithChar['B'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 24.0f, 25.0f })); // Blue
        TilesColorMapWithChar['B'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 24.0f, 23.0f })); // Grey
        TilesColorMapWithChar['B'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 24.0f, 21.0f })); // Green
        
        TileMap['B'] = Tile::Create(TilesColorMapWithChar['B'][0]);
        
        // Used bonus (from 4 - 7)
        TilesColorMapWithChar['b'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 27.0f, 27.0f })); // Brown
        TilesColorMapWithChar['b'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 27.0f, 25.0f })); // Blue
        TilesColorMapWithChar['b'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 27.0f, 23.0f })); // Grey
        TilesColorMapWithChar['b'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 27.0f, 21.0f })); // Green
        
        TileMap['b'] = Tile::Create(TilesColorMapWithChar['b'][0]);
    }
    
    // Steps SubTextures
    {
        TilesColorMapWithChar['S'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 26.0f })); // Brown
        TilesColorMapWithChar['S'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 24.0f })); // Blue
        TilesColorMapWithChar['S'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 22.0f })); // Grey
        TilesColorMapWithChar['S'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 20.0f })); // Green
        
        TileMap['S'] = Tile::Create(TilesColorMapWithChar['S'][color]);
    }
    
    // Bridge SubTexture
    {
        TilesColorMapWithChar['-'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 15.0f, 18.5f })); // Green
        TilesColorMapWithChar['-'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 15.0f, 14.5f })); // Orange
        TilesColorMapWithChar['-'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 15.0f, 10.5f })); // Grey
        TilesColorMapWithChar['-'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 15.0f,  8.5f })); // Pink
        
        TileMap['-'] = Tile::Create(TilesColorMapWithChar['-'][color]);
    }
    
    // Pipes
    {
        // Heads
        TilesColorMapWithChar['Y'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 19.0f }, { 2.0f, 1.0f })); // Green
        TilesColorMapWithChar['Y'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 15.0f }, { 2.0f, 1.0f })); // Orange
        TilesColorMapWithChar['Y'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 13.0f }, { 2.0f, 1.0f })); // Grey
        TilesColorMapWithChar['Y'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f,  9.0f }, { 2.0f, 1.0f })); // Pink
        
        TileMap['Y'] = Tile::Create(TilesColorMapWithChar['Y'][color]);
        
        // Base
        TilesColorMapWithChar['!'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 18.0f }, { 2.0f, 1.0f })); // Green
        TilesColorMapWithChar['!'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 14.0f }, { 2.0f, 1.0f })); // Orange
        TilesColorMapWithChar['!'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 12.0f }, { 2.0f, 1.0f })); // Grey
        TilesColorMapWithChar['!'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f,  8.0f }, { 2.0f, 1.0f })); // Pink
        
        TileMap['!'] = Tile::Create(TilesColorMapWithChar['!'][color]);
    }
    
    // Grass
    {
        TilesColorMapWithChar['<'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 11.0f, 18.0f })); // Green
        TilesColorMapWithChar['<'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 11.0f, 14.0f })); // Orange
        TilesColorMapWithChar['<'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 11.0f, 12.0f })); // Grey
        TilesColorMapWithChar['<'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 11.0f,  8.0f })); // Pink
        
        TileMap['<'] = Tile::Create(TilesColorMapWithChar['<'][color]);
        
        TilesColorMapWithChar['v'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 12.0f, 18.0f })); // Green
        TilesColorMapWithChar['v'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 12.0f, 14.0f })); // Orange
        TilesColorMapWithChar['v'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 12.0f, 12.0f })); // Grey
        TilesColorMapWithChar['v'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 12.0f,  8.0f })); // Pink
        
        TileMap['v'] = Tile::Create(TilesColorMapWithChar['v'][color]);
        
        TilesColorMapWithChar['>'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 13.0f, 18.0f })); // Green
        TilesColorMapWithChar['>'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 13.0f, 14.0f })); // Orange
        TilesColorMapWithChar['>'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 13.0f, 12.0f })); // Grey
        TilesColorMapWithChar['>'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 13.0f, 8.0f  })); // Pink
        
        TileMap['>'] = Tile::Create(TilesColorMapWithChar['>'][color]);
    }
    
    // Forest Grass
    {
        TilesColorMapWithChar['{'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 19.0f })); // Green
        TilesColorMapWithChar['{'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 15.0f })); // Orange
        TilesColorMapWithChar['{'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 13.0f })); // Grey
        TilesColorMapWithChar['{'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 8.0f,  9.0f })); // Pink
        
        TileMap['{'] = Tile::Create(TilesColorMapWithChar['{'][color]);
        
        TilesColorMapWithChar['}'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 19.0f })); // Green
        TilesColorMapWithChar['}'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 15.0f })); // Orange
        TilesColorMapWithChar['}'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 13.0f })); // Grey
        TilesColorMapWithChar['}'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 10.0f,  9.0f })); // Pink
        
        TileMap['}'] = Tile::Create(TilesColorMapWithChar['}'][color]);
        
        
        TilesColorMapWithChar['*'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 19.0f })); // Green
        TilesColorMapWithChar['*'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 15.0f })); // Orange
        TilesColorMapWithChar['*'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 13.0f })); // Grey
        TilesColorMapWithChar['*'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 9.0f,  9.0f })); // Pink
        
        TileMap['*'] = Tile::Create(TilesColorMapWithChar['*'][color]);
        
        TilesColorMapWithChar['1'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 18.0f })); // Green
        TilesColorMapWithChar['1'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 14.0f })); // Orange
        TilesColorMapWithChar['1'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 12.0f })); // Grey
        TilesColorMapWithChar['1'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 8.0f,  8.0f })); // Pink
        
        TileMap['1'] = Tile::Create(TilesColorMapWithChar['1'][color]);
        
        TilesColorMapWithChar['2'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 18.0f })); // Green
        TilesColorMapWithChar['2'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 14.0f })); // Orange
        TilesColorMapWithChar['2'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 12.0f })); // Grey
        TilesColorMapWithChar['2'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 9.0f,  8.0f })); // Pink
        
        TileMap['2'] = Tile::Create(TilesColorMapWithChar['2'][color]);
        
        TilesColorMapWithChar['3'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 18.0f })); // Green
        TilesColorMapWithChar['3'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 14.0f })); // Orange
        TilesColorMapWithChar['3'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 12.0f })); // Grey
        TilesColorMapWithChar['3'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 10.0f,  8.0f })); // Pink
        
        TileMap['3'] = Tile::Create(TilesColorMapWithChar['3'][color]);
    }
    
    // Cloud
    {
        TilesColorMapWithChar['('][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar['('][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar['('][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        TilesColorMapWithChar['('][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        TileMap['('] = Tile::Create(TilesColorMapWithChar['('][color]);
        
        TilesColorMapWithChar['^'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 1.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar['^'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 1.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar['^'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 1.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        TilesColorMapWithChar['^'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 1.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        TileMap['^'] = Tile::Create(TilesColorMapWithChar['^'][color]);
        
        TilesColorMapWithChar[')'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 2.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar[')'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 2.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar[')'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 2.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        TilesColorMapWithChar[')'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 2.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        TileMap[')'] = Tile::Create(TilesColorMapWithChar[')'][color]);
    }
    
    // Castel
    {
        TileMap['.'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 19.0f, 25.0f }));
        TileMap['u'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 20.0f, 24.0f }));
        TileMap['o'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 21.0f, 24.0f }));
        TileMap['|'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 21.0f, 25.0f }));
        TileMap['l'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 20.0f, 25.0f }));
        TileMap['r'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 22.0f, 25.0f }));
    }
}
