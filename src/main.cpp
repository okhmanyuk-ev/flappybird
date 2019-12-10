#include <Platform/defines.h>

#include "application.h"

#if defined(PLATFORM_WINDOWS)
void main(int argc, char* argv[])
{
	FlappyBird::Application().run();
}
#elif defined(PLATFORM_ANDROID)
#include <Platform/system_android.h>

void android_main(android_app* app)
{
	Platform::SystemAndroid::run(app, [] { FlappyBird::Application().run(); });
}
#endif