#pragma once

#include <cstdint>
#include "glm/vec3.hpp"
#include <ranges>

namespace Mineclone {
    enum class CubeFace : uint8_t {
        // +Z
        NORTH = 0,
        // -Z
        SOUTH = 1,
        // +X
        EAST  = 2,
        // -X
        WEST  = 3,
        // +Y
        UP    = 4,
        // -Y
        DOWN  = 5,
        UNKNOWN
    };

    inline std::string toLower(std::string s) {
        namespace rv = std::ranges;
        auto lower_view = s | rv::views::transform([](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

        return {lower_view.begin(), lower_view.end()};
    }

    inline CubeFace getCubeFace(const std::string& name) {
        std::string lower = toLower(name);
        static const std::unordered_map<std::string_view, CubeFace> lookup {
                {"top",    CubeFace::UP},
                {"bottom", CubeFace::DOWN},
                {"north",  CubeFace::NORTH},
                {"south",  CubeFace::SOUTH},
                {"east",   CubeFace::EAST},
                {"west",   CubeFace::WEST},
                {"up",     CubeFace::UP},
                {"down",   CubeFace::DOWN}
        };

        if (auto it = lookup.find(lower); it != lookup.end()) {
            return it->second;
        }
        return CubeFace::UNKNOWN;
    }

    inline std::string getCubeFaceName(const CubeFace& face) {
        switch(face) {
            case CubeFace::NORTH:
                return "North";
            case CubeFace::SOUTH:
                return "South";
            case CubeFace::EAST:
                return "East";
            case CubeFace::WEST:
                return "West";
            case CubeFace::UP:
                return "Up";
            case CubeFace::DOWN:
                return "Down";
            default:
            case CubeFace::UNKNOWN:
                return "Unknown";
        }
    }

    inline int getCubeFaceIndex(const std::string& name) {
        std::string lower = toLower(name);
        static const std::unordered_map<std::string_view, CubeFace> lookup {
                {"top",    CubeFace::UP},
                {"bottom", CubeFace::DOWN},
                {"north",  CubeFace::NORTH},
                {"south",  CubeFace::SOUTH},
                {"east",   CubeFace::EAST},
                {"west",   CubeFace::WEST},
                {"up",     CubeFace::UP},
                {"down",   CubeFace::DOWN}
        };

        if (auto it = lookup.find(lower); it != lookup.end()) {
            return static_cast<int>(it->second);
        }
        return static_cast<int>(CubeFace::UNKNOWN );
    }

    struct FaceData {
        glm::vec3 normal;
        glm::vec3 tangent;     // for normal mapping
        glm::vec3 corners[4];  // CCW winding order
    };



    static inline constexpr FaceData FACE_DATA[6] = {
            // [0] NORTH (+Z)
            FaceData{
                    .normal = glm::vec3{0,0,1},
                    .tangent = glm::vec3{-1,0,0},
                    .corners = {
                            glm::vec3{1,0,1},
                            glm::vec3{0,0,1},
                            glm::vec3{0,1,1},
                            glm::vec3{1,1,1}
                    }
            },
            // [1] SOUTH (-Z)
            FaceData{
                    .normal = glm::vec3{0,0,-1},
                    .tangent = glm::vec3{1,0,0},
                    .corners = {
                            glm::vec3{0,0,0},
                            glm::vec3{1,0,0},
                            glm::vec3{1,1,0},
                            glm::vec3{0,1,0}
                    }
            },
            // [2] EAST (+X)
            FaceData{
                    .normal = glm::vec3{1,0,0},
                    .tangent = glm::vec3{0,0,-1},
                    .corners = {
                            glm::vec3{1,0,0},
                            glm::vec3{1,0,1},
                            glm::vec3{1,1,1},
                            glm::vec3{1,1,0}
                    }
            },
            // [3] WEST (-X)
            FaceData{
                    .normal = glm::vec3{-1,0,0},
                    .tangent = glm::vec3{0,0,1},
                    .corners = {
                            glm::vec3{0,0,1},
                            glm::vec3{0,0,0},
                            glm::vec3{0,1,0},
                            glm::vec3{0,1,1}
                    }
            },
            // [4] UP (+Y)
            FaceData{
                    .normal = glm::vec3{0,1,0},
                    .tangent = glm::vec3{1,0,0},
                    .corners = {
                            glm::vec3{0,1,0},
                            glm::vec3{1,1,0},
                            glm::vec3{1,1,1},
                            glm::vec3{0,1,1}
                    }
            },
            // [5] DOWN (-Y)
            FaceData{
                    .normal = glm::vec3{0,-1,0},
                    .tangent = glm::vec3{1,0,0},
                    .corners = {
                            glm::vec3{0,0,1},
                            glm::vec3{1,0,1},
                            glm::vec3{1,0,0},
                            glm::vec3{0,0,0}
                    }
            }
    };

    constexpr const FaceData& getFaceData(CubeFace face) {
        return FACE_DATA[static_cast<uint8_t>(face)];
    }

}