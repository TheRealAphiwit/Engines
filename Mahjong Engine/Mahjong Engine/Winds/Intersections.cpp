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

    bool CheckRayIntersect(const Ray& aRay, Collider* aCollider)
    {
        if (aCollider->IsOf<SphereCollider>())
        {
            SphereCollider* sphere = dynamic_cast<SphereCollider*>(aCollider);
            return RaySphereIntersect(aRay, *sphere);
        }
        else if (aCollider->IsOf<BoxCollider>())
        {
            BoxCollider* box = dynamic_cast<BoxCollider*>(aCollider);
            return RayBoxIntersect(aRay, *box);
        }

        return false;
    }
    bool RaySphereIntersect(const Ray& aRay, const SphereCollider& aSphere)
    {
        // vector between ray origin & sphere center
        glm::vec3 center = aSphere.Transform[3];
        glm::vec3 diff = center - aRay.Origin;
        
        // project diff onto ray direction 
        float t0 = glm::dot(diff, aRay.Direction);

        return false;
    }
}