//
//  Background.hpp
//  Mario
//
//  Created by iKan on 08/06/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Mario {
    
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
    "      XBXBX                                  XBXBX              XBXXXXBX                           S           XXXXX               B                                           B    B    B                                                          XBXBX                                                     |||o|||o|||   0"
    "                                                                                Y                 SS                                                                                                                          S  S                                                       S                  ............... 0"
    "                                                                                !                SSS                                                   S                                                 Y                   SS  SS                                                     SS                  ||||||||||||||| 0"
    "                                                          Y                     !               SSSS                               Y                  SS        *                                        !                  SSS  SSS       *          Y                                SSS                  |u||u||u||u||u| 0"
    "  <v>          S                           <v>      S     !       S  S          !     <vv>     SSSSS                    <vvv>      !          <v>    SSS       {1}                                       !                 SSSS  SSSS     {1}         !         <v>                   SSSS         <v>      |o||o||o||o||0| 0"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG--------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGG--------------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG0"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG                    GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGG                          GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG0"
    ;

    /// Theme color mario tile
    enum ThemeColor {
        Brown = 0, Blue = 1, Grey = 2, Green = 3, MAX_COLOR = 4
    };
    
    class Background {
    public:
        static void Init(const std::shared_ptr<Scene>& scene);
        static void Shutdown();
        
    private:
        Background() = default;
        MAKE_SINGLETON(Background);

        static void StoreSubtextureOfEachTile();
        
        /// Stores the data of single Tile
        struct Tile {
            bool IsRigid = true;
            std::shared_ptr<SubTexture> SubTexture;
            
            Tile(const std::shared_ptr<iKan::SubTexture>& subTexture, bool isRigid) : IsRigid(isRigid),  SubTexture(subTexture) {}
            
            static std::shared_ptr<Tile> Create(const std::shared_ptr<iKan::SubTexture>& subTexture, bool isRigid = true) {
                return std::make_shared<Tile>(subTexture, isRigid);
            }
            
            ~Tile() {
                
            }
        };
        
        static ThemeColor CurrentTheme;
        
        // Texture to store tile sprite sheet
        static std::shared_ptr<Texture> Sprite;
        
        // Stores the map of each tile with Entity
        static std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Tile>> TileEntityMap;
        
        // Map of Array of subtextures to the Char (defined in s_MapTiles) of their corresponding tile.
        // Sotring Subtextures of same tile with different color Array
        static std::unordered_map<char, std::array<std::shared_ptr<SubTexture>, MAX_COLOR>> TilesColorMapWithChar;
        
        // Map of tile (property of tile that stores the subtexture also)
        // to the Char of their corresponding. This map stores the Tile information
        // that will be rendered in background
        // NOTE: s_BgData->TilesColorMapWithChar is just storing all the colors. These are not
        // getting rendered at all
        static std::unordered_map<char, std::shared_ptr<Tile>> TileMap;
    };
    
}
