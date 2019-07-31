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

#include <manifold.h>

#include <assert.h>

using namespace glsl_math;

bool Manifold::Link::operator==(const Manifold::Link& other) const
{
    return index == other.index && side == other.side;
}

bool Manifold::Link::operator!=(const Manifold::Link& other) const
{
    return index != other.index || side != other.side;
}

Manifold::Manifold()
    : m_maxVertexCount(0)
{
}

bool Manifold::build(BuildWorkspace& wks, const uint32_t* triangleIndices, uint32_t triangleIndexCount)
{
    m_triangles.clear();
    m_maxVertexCount = 0;
    for (uint32_t i = 2; i < triangleIndexCount; i += 3)
    {
        Triangle t;
        t.vertex[0] = triangleIndices[i - 2];
        t.vertex[1] = triangleIndices[i - 1];
        t.vertex[2] = triangleIndices[i];
        m_maxVertexCount = max(m_maxVertexCount, t.vertex[0] + 1);
        m_maxVertexCount = max(m_maxVertexCount, t.vertex[1] + 1);
        m_maxVertexCount = max(m_maxVertexCount, t.vertex[2] + 1);
        t.link[0] = InvalidLink;
        t.link[1] = InvalidLink;
        t.link[2] = InvalidLink;
        m_triangles.push_back(t);
    }

    return build(wks);
}

bool Manifold::build(BuildWorkspace& wks, const std::vector<uint32_t>& triangleIndices)
{
    return build(wks, triangleIndices.data(), static_cast<uint32_t>(triangleIndices.size()));
}

bool Manifold::build(BuildWorkspace& wks)
{
    const uint32_t triangleCount = static_cast<uint32_t>(m_triangles.size());
    if (!triangleCount)
        return true;

    auto& vertexRings = wks.vertexRings;
    vertexRings.clear();
    vertexRings.resize(m_maxVertexCount);
    auto& triangleVertexRings = wks.triangleVertexRings;
    triangleVertexRings.clear();
    triangleVertexRings.resize(triangleCount);
    auto& adjList = wks.adjList;

    // initialize linked-list of walls per vertex
    for (uint32_t i = 0; i < m_maxVertexCount; ++i) {
        vertexRings[i] = InvalidLink;
    }
    for (uint32_t i = 0; i < triangleCount; ++i) {
        auto& t = m_triangles[i];
        auto& tvr = triangleVertexRings[i];
        for (uint32_t j = 0; j < 3; ++j) {
            uint32_t vi = t.vertex[j];
            auto& vr = vertexRings[vi];
            tvr.next[j] = vr;
            vr = { i, j };
            t.link[j] = InvalidLink;
        }
    }

    for (uint32_t i = 0; i < m_maxVertexCount; ++i) {
        auto vr = vertexRings[i];
        if (vr == InvalidLink)
            continue;
        adjList.clear();
        for (; vr != InvalidLink; vr = triangleVertexRings[vr.index].next[vr.side])
            adjList.push_back(vr);

        // Match all combinations.
        const uint32_t adjCount = adjList.count();
        for (uint32_t k1 = 0; k1 < adjCount; ++k1) {
            const auto vr1 = adjList[k1];
            auto& t1 = m_triangles[vr1.index];
            const uint32_t v1 = t1.vertex[(vr1.side + 1) % 3];
            const uint32_t s1 = vr1.side;
            for (uint32_t k2 = 0; k2 < adjCount; ++k2) {
                if (k1 == k2)
                    continue;
                const auto vr2 = adjList[k2];
                // always match triangle with lower index with triangle with higher index
                // (to avoid checking the same edge twice)
                if (vr1.index > vr2.index)
                    continue;
                auto& t2 = m_triangles[vr2.index];
                const uint32_t s2 = (vr2.side + 2) % 3;
                const uint32_t v2 = t2.vertex[s2];
                if (v1 == v2) {
                    auto& nb1 = t1.link[s1];
                    auto& nb2 = t2.link[s2];
                    if (nb1 != InvalidLink || nb2 != InvalidLink)
                        return false; // non-manifold!
                    nb1 = { vr2.index, s2 };
                    nb2 = { vr1.index, s1 };
                }
            }
        }
    }
    return true;
}

