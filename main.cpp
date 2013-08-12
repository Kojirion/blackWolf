#include "guimanager.h"

int main()
{
    try {
        guiManager app;

        app.run();
    }
    catch (exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }


    return 0;
}
