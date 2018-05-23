/**
 * Application3D.hpp skeleton
 * Contributors:
 *      * Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#ifndef APPLICATION3D_9YCQ0OVR
#define APPLICATION3D_9YCQ0OVR

#define GLM_FORCE_RADIANS
#include "system/Application.hpp"

class Camera;

/// \class Application3D
/// Demo : draw a 3D shape
class Application3D : public Application
{
    public:
        Application3D();

    protected:
        virtual void loop();

    private:
        float time = 0.f;
		Camera* m_Camera;
         
};

#endif /* end of include guard: APPLICATION3D_9YCQ0OVR */
