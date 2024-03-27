#ifndef GRIDWORLDENV_GRIDWORLD_H
#define GRIDWORLDENV_GRIDWORLD_H

#include "IAgentStep.h"
#include "raylib.h"
#include "rlgl.h"
#include <istream>
#include <utility>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace GWEnv {
namespace {
    template<class T> using vec = std::vector<T>;

    enum Action { UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD, PLACE, REMOVE, NONE };

    const char* ActionName[] = {
        "Up", "Down", "Left", "Right",
        "Forward","Backward", "Place", "Remove"
    };

    struct Coordinates {
        int x, y, z;
    };
}

class GridWorld {
public:
    GridWorld(int _w, int _h, int _d, int _blockTypes, const char* path, bool save)
            : w(_w), h(_h), d(_d), nBlockTypes(_blockTypes),
              filePath(path),
              grid(w, vec<vec<int>>(h, vec<int>(d, 0)))
    {
        if (!save) {
            LoadFromFile();
        }

        camera.position =  {6.5f, 8.5f, 8.5f};       // Camera position
        camera.target = {0.0f, 0.0f, 0.0f};         // Camera looking at point
        camera.up = {0.0f, 1.0f, 0.0f};             // Camera up vector (rotation towards target)
        camera.fovy = 45.0f;                                            // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;                         // Camera projection type
    }

    // Called every frame
    void Render();

    GridWorld(const GridWorld &) = delete;
    GridWorld& operator=(const GridWorld &) = delete;

private:
    // Draw placed blocks and current agent position
    void DrawBlocks() const;

    // Draw the xy, xz, yz planes using grids
    void DrawPlanes() const;

    bool AddBlock(int x, int y, int z, int blockType);

    void RemoveBlock(int x, int y, int z);

    void Move(Action direction);

    void SaveTarget() const;

    void LoadFromFile();

    inline void Record() {
        startingPos = agentPos;
        sequence.clear();
    }

    void SaveSequence() const;

    Action Step();

    // Whether a block could be placed at coordinates. See Coordinates struct comment
    [[nodiscard]] bool QueryPlacement(int x, int y, int z) const;

    // Resize the 3D grid based on user input to an empty grid
    // TODO: implement user input. See raygui.h
    // 2/18/24: could just take dimensions by command line arguments 
    // and not have our world be resizable throughout its lifetime.
    void ResizeGrid(uint32_t _w, uint32_t _h, uint32_t _d);

    //----------------------- Member Variables --------------------------------//
    Coordinates agentPos = {0, 0, 0};
    Camera camera = {0};

    // Dimensions of grid
    int w, h, d;

    // Number of block types in environment
    int nBlockTypes;

    constexpr static Color blockColors[13] = {
        BLACK,
        BLUE,
        YELLOW,
        ORANGE,
        PINK,
        RED,
        MAROON,
        GREEN,
        LIME,
        PURPLE,
        VIOLET,
        BEIGE,
        BROWN,
    };

    int currBlockType = 1;
    vec<vec<vec<int>>> grid;
    float spacing = 1.0f;
    std::string filePath;
    bool recording = false;
    Coordinates startingPos{};
    vec<Action> sequence;
};

}
#endif //GRIDWORLDENV_GRIDWORLD_H
