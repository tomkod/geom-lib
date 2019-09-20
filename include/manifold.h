// Basic triangle manifold data structure
// Copyright (C) 2019 Tomasz Dobrowolski
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <glslMath.h>

#include <vector>

#ifdef NDEBUG
#define ManifoldUnitTests 0
#else
#define ManifoldUnitTests 1
#endif

class Manifold
{
public:
    struct Link
    {
        uint32_t index : 30;
        uint32_t side : 2;

        bool operator==(const Link& other) const;
        bool operator!=(const Link& other) const;
    };

    struct Triangle
    {
        uint32_t vertex[3];
        Link link[3];
    };

    struct TriangleVertexRings
    {
        Link next[3];
    };

    struct BuildWorkspace
    {
    	std::vector<Link> vertexRings;
    	std::vector<TriangleVertexRings> triangleVertexRings;
        std::vector<Link> adjList;
    };

    std::vector<Triangle> m_triangles;
    uint32_t m_maxVertexCount;

    Manifold();

    bool build(BuildWorkspace& wks, const uint32_t* triangleIndices, uint32_t triangleIndexCount);

    bool build(BuildWorkspace& wks, const std::vector<uint32_t>& triangleIndices);

    bool build(BuildWorkspace& wks);

    uint32_t getEdgeKey(Link link) const;

    Link nextClockwise(Link link) const;
    Link nextAnticlockwise(Link link) const;

    template <typename F>
    bool visitTrianglesClockwise(Link start, Link end, const F& visitFunc) const;
    template <typename F>
    bool visitTrianglesAnticlockwise(Link start, Link end, const F& visitFunc) const;

    template <typename F>
    bool visitEdgesClockwise(Link start, Link end, const F& visitFunc) const;
    template <typename F>
    bool visitEdgesAnticlockwise(Link start, Link end, const F& visitFunc) const;
};

constexpr static Manifold::Link InvalidLink = {0, 3};

inline uint32_t Manifold::getEdgeKey(Link link) const
{
    const auto& t = m_triangles[link.index];
    const auto otlink = t.link[link.side];
    if (otlink == InvalidLink || otlink.index > link.index)
        return link.index * 3 + link.side;
    return otlink.index * 3 + otlink.side;
}

inline Manifold::Link Manifold::nextAnticlockwise(Link link) const
{
    //     0 2---2---0 2
    //    / \ \     / / \    * = start, & = prev
    //   2   0 1  &0 1   2
    //  /     \ \ / /  *  .
    // 2---1---1 1 1---0---0

    return m_triangles[link.index].link[(link.side + 1) % 3];
}

inline Manifold::Link Manifold::nextClockwise(Link link) const
{
    //     0 1---1---2 1
    //    / \ \     / / \    * = start, & = prev
    //   2   0 0*  2 0&  1
    //  /     \ \ / /     .
    // 2---1---1 0 0---2---2

    return m_triangles[link.index].link[(link.side + 2) % 3];
}

// "start" and/or "end" can be InvalidLink
// returns true if start connected with end
template <typename F>
bool Manifold::visitTrianglesClockwise(Link start, Link end, const F& visitFunc) const
{
    //     2 1---1---2 1
    //    / \ \     / / \      & = end
    //   1   2 0   2 0   1
    //  /     \ \ / /  &  \    * = start
    // 1---0---0 0 0---2---2
    // 2---2---0 0 0---0---1
    //  \     / / \ \  *  /
    //   1   0 2   0 2   1
    //    \ / /     \ \ /
    //     1 2---1---1 2


    auto it = start;
    while (it != InvalidLink) {

        visitFunc(it);

        it.side = (it.side + 2) % 3;
        if (it == end)
            return true;
        it = m_triangles[it.index].link[it.side];
    }

    it = end;
    while (it != InvalidLink) {

        it.side = (it.side + 1) % 3;

        visitFunc(it);

        it = m_triangles[it.index].link[it.side];
    }
    return false;
}

// "start" and/or "end" can be InvalidLink
// returns true if start connected with end
template <typename F>
bool Manifold::visitTrianglesAnticlockwise(Link start, Link end, const F& visitFunc) const
{
    //     2 1---1---2 1
    //    / \ \     / / \      & = end
    //   1   2 0   2 0   1
    //  /  &  \ \ / /     \    * = start
    // 1---0---0 0 0---2---2
    // 2---2---0 0 0---0---1
    //  \  *  / / \ \     /
    //   1   0 2   0 2   1
    //    \ / /     \ \ /
    //     1 2---1---1 2

    auto it = start;
    while (it != InvalidLink) {

        it.side = (it.side + 1) % 3;

        visitFunc(it);

        if (it == end)
            return true;
        it = m_triangles[it.index].link[it.side];
    }

    it = end;
    while (it != InvalidLink) {

        visitFunc(it);

        it.side = (it.side + 2) % 3;
        it = m_triangles[it.index].link[it.side];
    }
    return false;
}

// "start" and/or "end" can be InvalidLink
// returns true if start connected with end
template <typename F>
bool Manifold::visitEdgesClockwise(Link start, Link end, const F& visitFunc) const
{
    //     2 1---1---2 1
    //    / \ \     / / \      & = end
    //   1   2 0   2 0   1
    //  /     \ \ / /  &  \    * = start
    // 1---0---0 0 0---2---2
    //           0 0---0---1
    //          / \ \  *  /
    //         2   0 2   1
    //        /     \ \ /
    //       2---1---1 2

    auto it = start;
    if (it != InvalidLink) {
        visitFunc(it); // visit start

        do {
            it.side = (it.side + 2) % 3;
            if (it == end)
                return true;

            visitFunc(it);

            it = m_triangles[it.index].link[it.side];
        } while (it != InvalidLink);
    }

    it = end;
    while (it != InvalidLink) {

        it.side = (it.side + 1) % 3;

        visitFunc(it);

        it = m_triangles[it.index].link[it.side];
    }
    return false;
}

// "start" and/or "end" can be InvalidLink
// returns true if start connected with end
template <typename F>
bool Manifold::visitEdgesAnticlockwise(Link start, Link end, const F& visitFunc) const
{
    //     2 1---1---2 1
    //    / \ \     / / \      & = end
    //   1   2 0   2 0   1
    //  /  &  \ \ / /     \    * = start
    // 1---0---0 0 0---2---2
    // 2---2---0 0
    //  \  *  / / .
    //   1   0 2   0
    //    \ / /     .
    //     1 2---1---1

    auto it = start;
    if (it != InvalidLink) {
        visitFunc(it); // visit start

        do {
            it.side = (it.side + 1) % 3;
            if (it == end)
                return true;

            visitFunc(it);

            it = m_triangles[it.index].link[it.side];
        } while (it != InvalidLink);
    }

    it = end;
    while (it != InvalidLink) {

        it.side = (it.side + 2) % 3;

        visitFunc(it);

        it = m_triangles[it.index].link[it.side];
    }
    return false;
}

#if ManifoldUnitTests
void runManifoldUnitTests();
#endif
