#include "Intersections.h"
#include <glm.hpp>
#include "Collisions.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/norm.hpp>

namespace Winds
{
    Collision Winds::CheckIntersect(Collider* aCollider1, Collider* aCollider2)
    {
        if (aCollider1->IsOf<BoxCollider>() && aCollider2->IsOf<SphereCollider>())      std::swap(aCollider1, aCollider2);
        if (aCollider1->IsOf<BoxCollider>() && aCollider2->IsOf<PlaneCollider>())		std::swap(aCollider1, aCollider2);
        if (aCollider1->IsOf<SphereCollider>() && aCollider2->IsOf<PlaneCollider>())	std::swap(aCollider1, aCollider2);
        
        if (aCollider1->IsOf<SphereCollider>() && aCollider2->IsOf<SphereCollider>())
        {
            SphereCollider* sphere1 = dynamic_cast<SphereCollider*>(aCollider1);
            SphereCollider* sphere2 = dynamic_cast<SphereCollider*>(aCollider2);
            return SphereSphereIntersect(*sphere1, *sphere2);
        }
        else if (aCollider1->IsOf<BoxCollider>() && aCollider2->IsOf<SphereCollider>())
        {
            BoxCollider* box1 = dynamic_cast<BoxCollider*>(aCollider1);
            SphereCollider* sphere2 = dynamic_cast<SphereCollider*>(aCollider2);
            return BoxSphereIntersect(*box1, *sphere2);
        }
        else if (aCollider1->IsOf<BoxCollider>() && aCollider2->IsOf<BoxCollider>())
        {
            BoxCollider* box1 = dynamic_cast<BoxCollider*>(aCollider1);
            BoxCollider* box2 = dynamic_cast<BoxCollider*>(aCollider2);
            return BoxBoxIntersect(*box1, *box2);
        }
        else if (aCollider1->IsOf<PlaneCollider>() && aCollider2->IsOf<BoxCollider>())
        {
            PlaneCollider* plane = dynamic_cast<PlaneCollider*>(aCollider1);
            BoxCollider* box = dynamic_cast<BoxCollider*>(aCollider2);
            return PlaneBoxIntersect(*plane, *box);
        }
        else if (aCollider1->IsOf<PlaneCollider>() && aCollider2->IsOf<SphereCollider>())
        {
            PlaneCollider* plane = dynamic_cast<PlaneCollider*>(aCollider1);
            SphereCollider* sphere = dynamic_cast<SphereCollider*>(aCollider2);
            return PlaneSphereIntersect(*plane, *sphere);
        }
        return { nullptr, nullptr, glm::vec3() }; // return nulls if outside current scope
    }

    Collision SphereSphereIntersect(const SphereCollider& aSphere1, const SphereCollider& aSphere2)
    {
        Collision returnCollision = { nullptr, nullptr, glm::vec3() };

        glm::vec3 pos1 = glm::vec3(aSphere1.Transform[3]);
        glm::vec3 pos2 = glm::vec3(aSphere2.Transform[3]);
        float dist2 = glm::distance2(pos1, pos2);
        float scale1 = glm::max(glm::length(glm::vec3(aSphere1.Transform[0])),
            glm::max(glm::length(glm::vec3(aSphere1.Transform[1])),
                glm::length(glm::vec3(aSphere1.Transform[2]))));

        float scale2 = glm::max(glm::length(glm::vec3(aSphere2.Transform[0])),
            glm::max(glm::length(glm::vec3(aSphere2.Transform[1])),
                glm::length(glm::vec3(aSphere2.Transform[2]))));

        float r1 = aSphere1.Radius * scale1;
        float r2 = aSphere2.Radius * scale2;
        float radiusSum = r1 + r2;

        if (dist2 < radiusSum * radiusSum)
        {
            float dist = glm::sqrt(dist2);
            glm::vec3 normal = (dist > 0.0001f) ? (pos2 - pos1) / dist : glm::vec3(1, 0, 0);

            glm::vec3 contactPoint = pos1 + normal * r1;

            float penetration = radiusSum - dist;
            glm::vec3 correction = normal * penetration;

            bool s1Dynamic = !aSphere1.IsKinematic;
            bool s2Dynamic = !aSphere2.IsKinematic;

            if (s1Dynamic && s2Dynamic)
            {
                const_cast<SphereCollider*>(&aSphere1)->Position -= correction * 0.5f;
                const_cast<SphereCollider*>(&aSphere2)->Position += correction * 0.5f;

                const_cast<SphereCollider*>(&aSphere1)->Transform[3] = glm::vec4(aSphere1.Position, 1.0f);
                const_cast<SphereCollider*>(&aSphere2)->Transform[3] = glm::vec4(aSphere2.Position, 1.0f);
            }
            else if (s1Dynamic)
            {
                const_cast<SphereCollider*>(&aSphere1)->Position -= correction;
                const_cast<SphereCollider*>(&aSphere1)->Transform[3] = glm::vec4(aSphere1.Position, 1.0f);
            }
            else if (s2Dynamic)
            {
                const_cast<SphereCollider*>(&aSphere2)->Position += correction;
                const_cast<SphereCollider*>(&aSphere2)->Transform[3] = glm::vec4(aSphere2.Position, 1.0f);
            }

            returnCollision = { const_cast<SphereCollider*>(&aSphere1), const_cast<SphereCollider*>(&aSphere2), contactPoint, normal };
        }

        return returnCollision;
    }

    Collision BoxBoxIntersect(const BoxCollider& aBox1, const BoxCollider& aBox2) // Don't fully understand
    {
        Collision returnCollision = { nullptr, nullptr, glm::vec3(), glm::vec3() };

        // Extract rotation and translation
        glm::mat3 rotation1 = glm::mat3(aBox1.Transform);
        glm::mat3 rotation2 = glm::mat3(aBox2.Transform);
        glm::vec3 translation = glm::vec3(aBox2.Transform[3]) - glm::vec3(aBox1.Transform[3]);

        glm::mat3 rotation = rotation1 * glm::transpose(rotation2);
        glm::mat3 absRotation;
        for (int i = 0; i < 3; ++i) {
            absRotation[i] = glm::abs(rotation[i]) + 0.000001f;
        }

        float minPenetration = FLT_MAX;
        glm::vec3 bestAxis = glm::vec3(0.0f);

        // --- 1. Box 1's face axes ---
        for (int i = 0; i < 3; ++i) {
            float ra = aBox1.Extents[i];
            float rb = glm::dot(absRotation[i], aBox2.Extents);
            float penetration = (ra + rb) - glm::abs(glm::dot(translation, rotation1[i]));

            if (penetration < 0.0f) return returnCollision;

            if (penetration < minPenetration) {
                minPenetration = penetration;
                bestAxis = rotation1[i];
            }
        }

        // --- 2. Box 2's face axes ---
        for (int i = 0; i < 3; ++i) {
            float ra = glm::dot(absRotation[i], aBox1.Extents);
            float rb = aBox2.Extents[i];
            float penetration = (ra + rb) - glm::abs(glm::dot(translation, rotation2[i]));

            if (penetration < 0.0f) return returnCollision;

            if (penetration < minPenetration) {
                minPenetration = penetration;
                bestAxis = rotation2[i];
            }
        }

        // --- 3. Cross-product axes ---
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                glm::vec3 axis = glm::cross(rotation1[i], rotation2[j]);

                if (glm::length(axis) < 0.001f) continue;
                axis = glm::normalize(axis);

                float ra = glm::dot(aBox1.Extents, glm::abs(rotation1 * axis));
                float rb = glm::dot(aBox2.Extents, glm::abs(rotation2 * axis));
                float penetration = (ra + rb) - glm::abs(glm::dot(glm::normalize(translation), axis)) * glm::length(translation);

                if (penetration < 0.0f) return returnCollision;

                if (penetration < minPenetration) {
                    minPenetration = penetration;
                    bestAxis = axis;
                }
            }
        }

        // Ensure normal points from box1 to box2
        glm::vec3 normal = glm::normalize(bestAxis);
        if (glm::dot(normal, translation) < 0.0f) {
            normal = -normal;
        }

        // Compute contact point (approximate)
        glm::vec3 contactPoint = glm::vec3(aBox1.Transform[3]) + normal * minPenetration * 0.5f;

        return { const_cast<BoxCollider*>(&aBox1), const_cast<BoxCollider*>(&aBox2), contactPoint, normal };
    }

    Collision BoxSphereIntersect(const BoxCollider& aBox1, const SphereCollider& aSphere2)
    {
        glm::vec3 sphereCenter = glm::vec3(aSphere2.Transform[3]);
        glm::vec3 localSphereCenter = glm::inverse(aBox1.Transform) * glm::vec4(sphereCenter, 1.0f);
        glm::vec3 closestPoint = glm::clamp(localSphereCenter, -aBox1.Extents, aBox1.Extents);
        float dist2 = glm::length2(localSphereCenter - closestPoint);

        if (dist2 < aSphere2.Radius * aSphere2.Radius)
        {
            glm::vec3 normal = localSphereCenter - closestPoint;
            if (glm::length(normal) > 0.0001f)
                normal = glm::normalize(normal);
            else
                normal = glm::vec3(1, 0, 0); // Fallback normal

            glm::vec3 worldContactPoint = aBox1.Transform * glm::vec4(closestPoint, 1.0f);
            return { const_cast<BoxCollider*>(&aBox1), const_cast<SphereCollider*>(&aSphere2), worldContactPoint, normal };
        }

        return { nullptr, nullptr, glm::vec3(0), glm::vec3(0) };
    }

    Collision PlaneBoxIntersect(const PlaneCollider& aPlane, const BoxCollider& aBox)
    {
        glm::vec3 boxCenter = glm::vec3(aBox.Transform[3]);
        glm::mat3 boxRotation = glm::mat3(aBox.Transform);  
        glm::vec3 halfExtents = aBox.Extents * 0.5f; // Can potentially remove 0.5

        glm::vec3 boxToPlane = boxCenter - glm::vec3(aPlane.Position);
        glm::vec3 transformedCenter = glm::inverse(boxRotation) * boxToPlane;

        glm::vec3 normal = glm::normalize(aPlane.Normal);
        float d = glm::dot(normal, aPlane.Position);

        float distance = glm::dot(normal, transformedCenter) - d;
        float r = glm::dot(halfExtents, glm::abs(boxRotation * normal)); // Radius of box along normal
        float distToCenter = glm::dot(normal, boxCenter) - d;

        // Check intersection
        if (glm::abs(distToCenter) > r)
        {
            return { nullptr, nullptr, glm::vec3(0), glm::vec3(0) };
        }

        float penetration = r - distToCenter;

        // Only push if the box is not kinematic
        if (!aBox.IsKinematic && penetration > 0.0f)
        {
            glm::vec3 correction = normal * penetration;

            // Update the collider
            const_cast<BoxCollider*>(&aBox)->Position += correction;
            const_cast<BoxCollider*>(&aBox)->Transform[3] = glm::vec4(const_cast<BoxCollider*>(&aBox)->Position, 1.0f);

            // Recalculate center for contact point logic
            boxCenter += correction;
        }

        glm::vec3 contactPoint = boxCenter - normal * r;

        return { const_cast<PlaneCollider*>(&aPlane), const_cast<BoxCollider*>(&aBox), contactPoint, normal };
    }

    Collision PlaneSphereIntersect(const PlaneCollider& aPlane, const SphereCollider& aSphere)
    {
        float scale = glm::max(glm::length(glm::vec3(aSphere.Transform[0])),
            glm::max(glm::length(glm::vec3(aSphere.Transform[1])),
                glm::length(glm::vec3(aSphere.Transform[2]))));
        float effectiveRadius = aSphere.Radius * scale;

        glm::vec3 normal = glm::normalize(aPlane.Normal);
        float d = glm::dot(normal, aPlane.Position);
        float distance = glm::dot(normal, aSphere.Position) - d;

        if (distance < effectiveRadius)
        {
            glm::vec3 collisionPoint = aSphere.Position - normal * distance;

            float penetrationDepth = effectiveRadius - distance;
            glm::vec3 correction = normal * penetrationDepth;
            const_cast<SphereCollider*>(&aSphere)->Position += correction;

            const_cast<SphereCollider*>(&aSphere)->Transform[3] = glm::vec4(aSphere.Position, 1.0f);

            return { const_cast<PlaneCollider*>(&aPlane), const_cast<SphereCollider*>(&aSphere), collisionPoint, normal };
        }

        return { nullptr, nullptr, glm::vec3(), glm::vec3() };
    }

    bool CheckRayIntersect(const Ray& aRay, Collider* aCollider, RayHit& outHit)
    {
        float distance = 10.0f;

        if (aCollider->IsOf<SphereCollider>())
        {
			SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(aCollider);
			return RaySphereIntersect(aRay, *sphereCollider, distance);
        }
        else if (aCollider->IsOf<BoxCollider>())
        {
			BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(aCollider);
			return RayBoxIntersect(aRay, *boxCollider, distance);
        }
        else if (aCollider->IsOf<MeshCollider>())
        {
            // Comming soon
            // return RayMeshIntersect(aRay, *meshCollider);
        }
        else
            return false; // will always return false - duh

        outHit.Distance = distance;
        outHit.Point = aRay.Origin + aRay.Direction * distance;
        outHit.Collider = aCollider;
        return true;
    }

    bool RaySphereIntersect(const Ray& aRay, const SphereCollider& aSphere, float& outDistance)
    {
        glm::vec3 center = aSphere.Transform[3];
        glm::vec3 diff = center - aRay.Origin;

        float t0 = glm::dot(diff, aRay.Direction);
        float dSquared = glm::dot(diff, diff) - t0 * t0;

        float radiusSquared = aSphere.Radius * aSphere.Radius;
        if (dSquared > radiusSquared)
        {
            return false;
        }

        float t1 = glm::sqrt(radiusSquared - dSquared);

        float Epsilon = 0.000001f;
        float outIntersectionDistance = (t0 > t1 + Epsilon) ? t0 - t1 : t0 + t1;

		outDistance = outIntersectionDistance;

        // return true if intersection distance is positive
        return outIntersectionDistance > Epsilon;
    }

    bool RayBoxIntersect(const Ray& aRay, const BoxCollider& aBox, float& outDistance)
    {
        glm::vec3 min = glm::vec3(aBox.Transform[3]) - aBox.Extents;
        glm::vec3 max = glm::vec3(aBox.Transform[3]) + aBox.Extents;

        glm::vec3 invDir = 1.0f / aRay.Direction;

        float t1 = (min.x - aRay.Origin.x) * invDir.x;
        float t2 = (max.x - aRay.Origin.x) * invDir.x;
        float t3 = (min.y - aRay.Origin.y) * invDir.y;
        float t4 = (max.y - aRay.Origin.y) * invDir.y;
        float t5 = (min.z - aRay.Origin.z) * invDir.z;
        float t6 = (max.z - aRay.Origin.z) * invDir.z;

        float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        // Alt version
        /*if (tmax < 0.0f || tmin > tmax)
            return false;

        outDistance = (tmin >= 0.0f) ? tmin : tmax;
        return outDistance >= 0.0f;*/

        // Martin version
		outDistance = tmin;
        return tmax >= std::max(0.0f, tmin);
    }

    bool RayOBBIntersect(const Ray& aRay, const BoxCollider& aBox, float& outDistance)
    {
        glm::vec3 center = glm::vec3(aBox.Transform[3]);
        glm::mat3 rotation = glm::mat3(aBox.Transform);

        glm::vec3 localOrigin = glm::transpose(rotation) * (aRay.Origin - center);
        glm::vec3 localDirection = glm::transpose(rotation) * aRay.Direction;

        BoxCollider localBox = BoxCollider(glm::vec3(0, 0, 0), aBox.Extents);
        localBox.Extents = aBox.Extents;

        Ray localRay(localOrigin, localDirection);
        return RayBoxIntersect(localRay, localBox, outDistance);
    }
}