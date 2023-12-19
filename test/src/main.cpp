
#include "../../engine/include/eng/engine.hpp"




// class HelloTriangleApplication {

// private:


//     bool checkValidationLayerSupport() {

//         uint32_t layerCount;
//         vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

//         std::vector<VkLayerProperties> availableLayers(layerCount);
//         vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

//         for (const char* layerName : validationLayers) {
//             bool layerFound = false;

//             for (const auto& layerProperties : availableLayers) {
//                 if (strcmp(layerName, layerProperties.layerName) == 0) {
//                     layerFound = true;
//                     break;
//                 }
//             }

//             if (!layerFound) {
//                 return false;
//             }
//     }

//     return true;
// }

// };



int main() {

    //std::cout << test_sum(1, 2) << std::endl;
    //std::cout << test_min(1, 2) << std::endl;

    Engine eng;
    eng.eng_startup();
    std::cout << "Starting loop\n";
    eng.loop();
    eng.eng_shutdown();

    return EXIT_SUCCESS;
}