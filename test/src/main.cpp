
#include "../../engine/include/eng/engine.hpp"

int main() {

    Engine eng;
    eng.startup();
    eng.loop();
    eng.shutdown();

    return EXIT_SUCCESS;
}