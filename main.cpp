#include "guimanager.h"

int main()
{
    try {
        guiManager app;

        app.run();
    }
    catch (exception& e)
    {
        cerr << "Exception: " << e.what() << "\n";
    }


    return 0;
}
