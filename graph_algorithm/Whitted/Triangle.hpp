#pragma once

#include <cstring>
#include "Object.hpp"

// Möller-Trumbore 算法
bool rayTriangleIntersect(const Vector3f& v0, const Vector3f& v1,
                          const Vector3f& v2, const Vector3f& orig,
                          const Vector3f& dir, float& tnear, float& u,
                          float& v) {
    // 背公式
    Vector3f e1 = v1 - v0;
    Vector3f e2 = v2 - v0;
    Vector3f s0 = orig - v0;  // s0表示从v0到射线起点的向量
    Vector3f s1 = crossProduct(dir, e2);
    Vector3f s2 = crossProduct(s0, e1);

    Vector3f s =
        Vector3f(dotProduct(s2, e2), dotProduct(s1, s0), dotProduct(s2, dir)) /
        dotProduct(s1, e1);
    tnear = s.x;
    u = s.y;  // u和v是交点处的barycentric坐标
    v = s.z;

    if (tnear >= 0 && u >= 0 && v >= 0 && (u + v) <= 1) return true;
    return false;
}

class MeshTriangle : public Object {
   public:
    MeshTriangle(const Vector3f* verts, const uint32_t* vertsIndex,
                 const uint32_t& numTris, const Vector2f* st) {
        uint32_t maxIndex = 0;
        for (uint32_t i = 0; i < numTris * 3; ++i)
            if (vertsIndex[i] > maxIndex) maxIndex = vertsIndex[i];
        maxIndex += 1;
        vertices = std::unique_ptr<Vector3f[]>(new Vector3f[maxIndex]);
        memcpy(vertices.get(), verts, sizeof(Vector3f) * maxIndex);
        vertexIndex = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
        memcpy(vertexIndex.get(), vertsIndex, sizeof(uint32_t) * numTris * 3);
        numTriangles = numTris;
        stCoordinates = std::unique_ptr<Vector2f[]>(new Vector2f[maxIndex]);
        memcpy(stCoordinates.get(), st, sizeof(Vector2f) * maxIndex);
    }

    bool intersect(const Vector3f& orig, const Vector3f& dir, float& tnear,
                   uint32_t& index, Vector2f& uv) const override {
        bool intersect = false;
        for (uint32_t k = 0; k < numTriangles; ++k) {
            const Vector3f& v0 = vertices[vertexIndex[k * 3]];
            const Vector3f& v1 = vertices[vertexIndex[k * 3 + 1]];
            const Vector3f& v2 = vertices[vertexIndex[k * 3 + 2]];
            float t, u, v;
            if (rayTriangleIntersect(v0, v1, v2, orig, dir, t, u, v) &&
                t < tnear) {
                tnear = t;  // 用最近的交点更新 tnear
                uv.x = u;   // 交点再三角形上的重心坐标
                uv.y = v;
                index = k;  // k 是相交的三角形的索引
                intersect |= true;  // 确保为 true，无论之前是什么
            }
        }

        return intersect;
    }

    void getSurfaceProperties(const Vector3f&, const Vector3f&,
                              const uint32_t& index, const Vector2f& uv,
                              Vector3f& N, Vector2f& st) const override {
        const Vector3f& v0 = vertices[vertexIndex[index * 3]];
        const Vector3f& v1 = vertices[vertexIndex[index * 3 + 1]];
        const Vector3f& v2 = vertices[vertexIndex[index * 3 + 2]];
        Vector3f e0 = normalize(v1 - v0);
        Vector3f e1 = normalize(v2 - v1);
        N = normalize(crossProduct(e0, e1));
        const Vector2f& st0 = stCoordinates[vertexIndex[index * 3]];
        const Vector2f& st1 = stCoordinates[vertexIndex[index * 3 + 1]];
        const Vector2f& st2 = stCoordinates[vertexIndex[index * 3 + 2]];
        st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;  // 插值得到交点的纹理坐标
    }

    Vector3f evalDiffuseColor(const Vector2f& st) const override {
        float scale = 5;
        // 计算纹理的网格坐标，实现 棋盘格模式
        // fmodf(a, b) 计算 a 除以 b 的余数（类似于 a % b，但适用于浮点数）
        float pattern = (fmodf(st.x * scale, 1) > 0.5) ^ (fmodf(st.y * scale, 1) > 0.5);
        return lerp(Vector3f(0.815, 0.235, 0.031),
                    Vector3f(0.937, 0.937, 0.231), pattern);
    }

    std::unique_ptr<Vector3f[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vector2f[]> stCoordinates;
};
