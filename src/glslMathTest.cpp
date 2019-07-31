// GLSL-like math library unit tests
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


#include <glslMath.h>

#include <stdio.h>
#include <assert.h>

namespace glsl_math
{

#if GlslMathUnitTests

static void assertTest(bool value)
{
    // TODO: display message somehow
    assert(value);
}

void runUnitTests()
{
    constexpr double eps = 1e-8;
    //if (numframes) return;
    {
        mat2 m = mat2(1, 2, 4, 3);
        vec2 a = vec2(1, 2);
        vec2 b = m*a;
        vec2 c = a*m;
        assertTest(b == vec2(9, 8));
        assertTest(c == vec2(5, 10));
        //consolef("%g %g %g\n", b.x, b.y);
        //consolef("%g %g %g\n", c.x, c.y);
        mat2 id = m*inverse(m);
        assertTest(fabs(id[0].x - 1) < eps && fabs(id[1].y - 1) < eps);
    }
    {
        mat3 m = mat3(1, 2, 3, 6, 5, 4, 7, 9, 8);
        vec3 a = vec3(1, 2, 3);
        vec3 b = m*a;
        vec3 c = a*m;
        assertTest(b == vec3(34, 39, 35));
        assertTest(c == vec3(14, 28, 49));
        //consolef("%g %g %g\n", b.x, b.y, b.z);
        //consolef("%g %g %g\n", c.x, c.y, c.z);
        mat3 id = m*inverse(m);
        assertTest(fabs(id[0].x - 1) < eps && fabs(id[1].y - 1) < eps && fabs(id[2].z - 1) < eps);
    }
    {
        mat4 m = mat4(1, 2, 3, 4, 6, 5, 4, 3, 7, 9, 8, 6, 9, 6, 3, 2);
        vec4 a = vec4(1, 2, 3, 4);
        vec4 b = m*a;
        vec4 c = a*m;
        assertTest(b == vec4(70, 63, 47, 36));
        assertTest(c == vec4(30, 40, 73, 38));
        //consolef("%g %g %g %g\n", b.x, b.y, b.z, b.w);
        //consolef("%g %g %g %g\n", c.x, c.y, c.z, c.w);
        mat4 id = m*inverse(m);
        assertTest(fabs(id[0].x - 1) < eps && fabs(id[1].y - 1) < eps
            && fabs(id[2].z - 1) < eps && fabs(id[3].w - 1) < eps);
    }
    {
        vec3f n(1, 2, 3), r(2, -1, 3);
        n = normalize(n);
        r = normalize(r);
        n = reflect(n, r);
        assertTest(length(n) - 1.0f < 1e-8f);
        assertTest(sign(n.x) == -1.0f);
    }
}
#endif

} // namespace glsl_math
