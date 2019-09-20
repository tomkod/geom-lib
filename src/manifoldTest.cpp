// Basic triangle manifold data structure unit tests
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

#include <algorithm>
#include <assert.h>

using namespace glsl_math;

#if ManifoldUnitTests

static void assertTest(bool value)
{
    // TODO: display message somehow
    assert(value);
}

static void visitRingTest()
{
    //   0---1---2---10
    //  /0\1/2\3/4\10/
    // 3---4---5---6
    //  \5/6\7/8\9/11.
    //   7---8---9---11
    std::vector<uint32_t> triangleIndices{
        0, 4, 3,  0, 1, 4,  1, 5, 4,  1, 2, 5,  2, 6, 5,
        3, 4, 7,  4, 8, 7,  4, 5, 8,  5, 9, 8,  5, 6, 9,
        2, 10, 6,  6, 11, 9};
    Manifold::BuildWorkspace wks;
    Manifold m;
    assertTest(m.build(wks, triangleIndices));

    uint32_t visitedCount;

    visitedCount = 0;
    m.visitTrianglesClockwise({ 7, 0 }, { 2, 1 }, [&m, &visitedCount](Manifold::Link it) {
        const auto& t = m.m_triangles[it.index];
        assertTest(t.vertex[it.side] == 4);
        ++visitedCount;
    });
    assertTest(visitedCount == 6);

    visitedCount = 0;
    m.visitTrianglesAnticlockwise({ 7, 0 }, { 2, 1 }, [&m, &visitedCount](Manifold::Link it) {
        const auto& t = m.m_triangles[it.index];
        assertTest(t.vertex[it.side] == 5);
        ++visitedCount;
    });
    assertTest(visitedCount == 6);

    visitedCount = 0;
    m.visitTrianglesAnticlockwise({ 9, 0 }, { 4, 1 }, [&m, &visitedCount](Manifold::Link it) {
        const auto& t = m.m_triangles[it.index];
        assertTest(t.vertex[it.side] == 6);
        ++visitedCount;
    });
    assertTest(visitedCount == 4);

    visitedCount = 0;
    m.visitTrianglesClockwise({ 5, 0 }, { 0, 1 }, [&m, &visitedCount](Manifold::Link it) {
        const auto& t = m.m_triangles[it.index];
        assertTest(t.vertex[it.side] == 3);
        ++visitedCount;
    });
    assertTest(visitedCount == 2);

    visitedCount = 0;
    m.visitTrianglesClockwise({ 0, 2 }, InvalidLink, [&m, &visitedCount](Manifold::Link it) {
        const auto& t = m.m_triangles[it.index];
        assertTest(t.vertex[it.side] == 3);
        ++visitedCount;
    });
    assertTest(visitedCount == 2);

    visitedCount = 0;
    m.visitTrianglesClockwise(InvalidLink, { 5, 2 }, [&m, &visitedCount](Manifold::Link it) {
        const auto& t = m.m_triangles[it.index];
        assertTest(t.vertex[it.side] == 3);
        ++visitedCount;
    });
    assertTest(visitedCount == 2);

    visitedCount = 0;
    m.visitTrianglesAnticlockwise({ 5, 2 }, InvalidLink, [&m, &visitedCount](Manifold::Link it) {
        const auto& t = m.m_triangles[it.index];
        assertTest(t.vertex[it.side] == 3);
        ++visitedCount;
    });
    assertTest(visitedCount == 2);

    visitedCount = 0;
    m.visitTrianglesAnticlockwise(InvalidLink, { 0, 2 }, [&m, &visitedCount](Manifold::Link it) {
        const auto& t = m.m_triangles[it.index];
        assertTest(t.vertex[it.side] == 3);
        ++visitedCount;
    });
    assertTest(visitedCount == 2);
}

static void visitEdgesTest()
{
    //   0--1-2-3--4
    //  /0\1\2|3/4/5\
    // /    \\|//    \
    // 5------6------7
    //      //|\\  10/
    //    /6/7|8\9\ /
    //   8-9-10-11-12
    std::vector<uint32_t> triangleIndices{
        0, 6, 5,  0, 1, 6,  1, 2, 6,  2, 3, 6,  3, 4, 6,  4, 7, 6,
        6, 9, 8,  6, 10, 9,  6, 11, 10,  6, 12, 11,  6, 7, 12 };
    Manifold::BuildWorkspace wks;
    Manifold m;
    assertTest(m.build(wks, triangleIndices));

    uint32_t visitedCount;

    const uint32_t triangleIndexCount = static_cast<uint32_t>(triangleIndices.size());
    std::vector<bool> visitedEdge(triangleIndexCount);

    visitedCount = 0;
    for(uint32_t i = 0; i < triangleIndexCount; ++i)
        visitedEdge[i] = false;
    m.visitEdgesClockwise({ 7, 0 }, { 2, 1 }, [&visitedCount, &visitedEdge](Manifold::Link it) {
        visitedEdge[it.index * 3 + it.side] = true;
        ++visitedCount;
    });
    assertTest(visitedCount == 6);
    assertTest(visitedEdge[7 * 3 + 0]);
    assertTest(visitedEdge[7 * 3 + 2]);
    assertTest(visitedEdge[6 * 3 + 2]);
    assertTest(visitedEdge[2 * 3 + 2]);
    assertTest(visitedEdge[1 * 3 + 2]);
    assertTest(visitedEdge[0 * 3 + 1]);

    visitedCount = 0;
    for (uint32_t i = 0; i < triangleIndexCount; ++i)
        visitedEdge[i] = false;
    m.visitEdgesClockwise({ 7, 0 }, InvalidLink, [&visitedCount, &visitedEdge](Manifold::Link it) {
        visitedEdge[it.index * 3 + it.side] = true;
        ++visitedCount;
    });
    assertTest(visitedCount == 3);
    assertTest(visitedEdge[7 * 3 + 0]);
    assertTest(visitedEdge[7 * 3 + 2]);
    assertTest(visitedEdge[6 * 3 + 2]);

    visitedCount = 0;
    for (uint32_t i = 0; i < triangleIndexCount; ++i)
        visitedEdge[i] = false;
    m.visitEdgesClockwise(InvalidLink, {2, 1}, [&visitedCount, &visitedEdge](Manifold::Link it) {
        visitedEdge[it.index * 3 + it.side] = true;
        ++visitedCount;
    });
    assertTest(visitedCount == 3);
    assertTest(visitedEdge[2 * 3 + 2]);
    assertTest(visitedEdge[1 * 3 + 2]);
    assertTest(visitedEdge[0 * 3 + 1]);

    visitedCount = 0;
    for (uint32_t i = 0; i < triangleIndexCount; ++i)
        visitedEdge[i] = false;
    m.visitEdgesAnticlockwise({ 2, 1 }, { 7, 0 }, [&visitedCount, &visitedEdge](Manifold::Link it) {
        visitedEdge[it.index * 3 + it.side] = true;
        ++visitedCount;
    });
    assertTest(visitedCount == 6);
    assertTest(visitedEdge[2 * 3 + 1]);
    assertTest(visitedEdge[2 * 3 + 2]);
    assertTest(visitedEdge[1 * 3 + 2]);
    assertTest(visitedEdge[0 * 3 + 1]);
    assertTest(visitedEdge[7 * 3 + 2]);
    assertTest(visitedEdge[6 * 3 + 2]);

    visitedCount = 0;
    for (uint32_t i = 0; i < triangleIndexCount; ++i)
        visitedEdge[i] = false;
    m.visitEdgesClockwise({ 3, 2 }, { 8, 2 }, [&visitedCount, &visitedEdge](Manifold::Link it) {
        visitedEdge[it.index * 3 + it.side] = true;
        ++visitedCount;
    });
    assertTest(visitedCount == 6);
    assertTest(visitedEdge[3 * 3 + 2]);
    assertTest(visitedEdge[3 * 3 + 1]);
    assertTest(visitedEdge[4 * 3 + 1]);
    assertTest(visitedEdge[5 * 3 + 1]);
    assertTest(visitedEdge[10 * 3 + 2]);
    assertTest(visitedEdge[9 * 3 + 2]);

    visitedCount = 0;
    for (uint32_t i = 0; i < triangleIndexCount; ++i)
        visitedEdge[i] = false;
    m.visitEdgesAnticlockwise({ 8, 2 }, { 3, 2 }, [&visitedCount, &visitedEdge](Manifold::Link it) {
        visitedEdge[it.index * 3 + it.side] = true;
        ++visitedCount;
    });
    assertTest(visitedCount == 6);
    assertTest(visitedEdge[8 * 3 + 2]);
    assertTest(visitedEdge[8 * 3 + 0]);
    assertTest(visitedEdge[9 * 3 + 0]);
    assertTest(visitedEdge[10 * 3 + 0]);
    assertTest(visitedEdge[5 * 3 + 2]);
    assertTest(visitedEdge[4 * 3 + 2]);
}

void runManifoldUnitTests()
{
    visitRingTest();
    visitEdgesTest();
}

#endif
