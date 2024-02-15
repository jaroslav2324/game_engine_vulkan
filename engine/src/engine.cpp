#include "../include/eng/engine.hpp"


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                      const VkAllocationCallbacks* pAllocator, 
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
                                   VkDebugUtilsMessengerEXT debugMessenger, 
                                   const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

// engine startup. Initializes GLFW and Vulkan
engResult Engine::startup(){
    const i32 WIDTH = 800;
    const i32 HEIGHT = 600;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    engResult res;

    res = createVulkanInstance();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    res = setupDebugMessenger();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    res = createVulkanSurface();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    res = pickPhysicalDevice();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    res = createLogicalDevice();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    res = createSwapChain();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    res = createImageViews();
    if (res != ENG_RESULT_SUCCESS){
        return ENG_RESULT_FAILURE;
    }

    return ENG_RESULT_SUCCESS;
}

// engine startup. Stops GLFW and Vulkan
engResult Engine::shutdown(){

    ENG_LOG_INFO("Engine shutting down. Cleaning up everything...")
    //std::cout << "Shutting down...\n";

    for (auto imageView: swapChainImageViews){
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroyDevice(device, nullptr);

    if (ENG_VALIDATION_LAYERS_ENABLED) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, vulkanSurface, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();

    return ENG_RESULT_SUCCESS;
}

// engine loop
engResult Engine::loop(){
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }


    return ENG_RESULT_SUCCESS;
}

engResult Engine::createVulkanInstance(){

    ENG_LOG_INFO("Creating Vulkan instance...")

    if (ENG_VALIDATION_LAYERS_ENABLED && !checkValidationLayerSupport(vulkanValidationLayers)) {
        ENG_LOG_ERROR("Validation layers requested, but not available!")
        return ENG_RESULT_FAILURE;
    }

    VkApplicationInfo info{};
    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info.pApplicationName = "Hello triangle";
    info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    info.pEngineName = "Just engine";
    info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    info.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &info;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (ENG_VALIDATION_LAYERS_ENABLED){
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    //TODO replace lacal extentions
    this->vulkanExtensions = extensions;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (ENG_VALIDATION_LAYERS_ENABLED) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanValidationLayers.size());
        createInfo.ppEnabledLayerNames = vulkanValidationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);
    if (res != VK_SUCCESS) {
        ENG_LOG_ERROR("Failed to create vulkan instance!")
        return ENG_RESULT_FAILURE;
    }
    else{
        ENG_LOG_INFO("Vulkan instance created")
        return ENG_RESULT_SUCCESS;
    }
}

// compares awaited layers with available layers. returns true if all awaited layers found
bool Engine::checkValidationLayerSupport(const std::vector<const char*>& awaitedLayers) {

    VkResult result;
    uint32_t layerCount;
    result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    if (result != VK_SUCCESS) return false;

    std::vector<VkLayerProperties> availableLayers(layerCount);
    result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    if (result != VK_SUCCESS) return false;

    for (const char* layerName : awaitedLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;    
}


std::vector<const char*> Engine::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // TODO check how does this work
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    //std::cout << extensions.size() << std::endl; 

    if (ENG_VALIDATION_LAYERS_ENABLED) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

engResult Engine::setupDebugMessenger(){
    if(!ENG_VALIDATION_LAYERS_ENABLED) return ENG_RESULT_FAILURE;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    VkResult res;
    res = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
    if (res != VK_SUCCESS){
        ENG_LOG_WARNING("failed to set up debug messenger")
        return ENG_RESULT_FAILURE;
    }
    else{
        return ENG_RESULT_SUCCESS;
    }

}

VKAPI_ATTR VkBool32 VKAPI_CALL Engine::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    ENG_SET_CONSOLE_TEXT_COLOUR_DARK_GREY
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    ENG_SET_CONSOLE_TEXT_COLOUR_DEFAULT

    return VK_FALSE;
}

void Engine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

engResult Engine::pickPhysicalDevice(){

    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0){
        ENG_LOG_ERROR("Physical device not found")
        return ENG_RESULT_FAILURE;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    // std::string str = std::to_string(deviceCount);
    // ENG_LOG_ERROR(str.c_str());

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices){
        // choose the most sutable device
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }
    if (candidates.rbegin()->first > 0){
        physicalDevice = candidates.rbegin()->second;
    }
    else{
        ENG_LOG_ERROR("Failed to find a suitable GPU");
        return ENG_RESULT_FAILURE;
    }

    

    return ENG_RESULT_SUCCESS;
}

int Engine::rateDeviceSuitability(VkPhysicalDevice physicalDevice){

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    ENG_LOG_INFO_CYAN(deviceProperties.deviceName);

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
        ENG_LOG_INFO("This is discrete GPU");
    }
    else{
        ENG_LOG_INFO("This is not discrete GPU");
    }

    if (deviceFeatures.geometryShader){
        ENG_LOG_INFO("Geometry shaders are not supported");
    }
    else{
        ENG_LOG_INFO("Geometry shaders are supported");
    }

    int score = 0;

    // device not suitable - return 0
    if (! isDeviceSuitable(physicalDevice)){
        ENG_LOG_ERROR("Device is not suitable");
        score = 0;
        return score;
    }

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
        score += 2000;
    }

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU){
        score += 1500;
    }

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU){
        score += 1000;
    }

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU){
        score += 500;
    }

    score += deviceProperties.limits.maxImageDimension2D;

    std::string scoreStr = std::to_string(score);
    ENG_LOG_INFO("Device score:");
    ENG_LOG_INFO(scoreStr.c_str());

    return score;

}

QueueFamilyIndices Engine::findQueueFamilies(VkPhysicalDevice physicalDevice){
    QueueFamilyIndices indices;

    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int idx = 0;
    for (const auto& queueFamily: queueFamilies){
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = idx;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, idx, vulkanSurface, &presentSupport);
            if (presentSupport){
                indices.presentFamily = idx;
            }

            break;
        }
        idx++;
    }

    return indices;
}

engResult Engine::createLogicalDevice(){

    ENG_LOG_INFO("Creating logical device...");

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    f32 queuePriority = 1.0;

    for (u32 queueFamily: uniqueQueueFamilies){
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // TODO use if needed in the future
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    // to be compatible with out-to-date Vulkan versions
    createInfo.enabledExtensionCount = deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    if (ENG_VALIDATION_LAYERS_ENABLED){
        createInfo.enabledLayerCount = vulkanValidationLayers.size();
        createInfo.ppEnabledLayerNames = vulkanValidationLayers.data();
    }
    else{
        createInfo.enabledLayerCount = 0;
    }

    // create device
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS){
        ENG_LOG_ERROR("Failed to create a logical device");
        return ENG_RESULT_FAILURE;
    }
    // request created with device graphics queue
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

    return ENG_RESULT_SUCCESS;
}

bool Engine::isDeviceSuitable(VkPhysicalDevice physicalDevice){

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);
    bool swapChainOK = false;
    SwapChainSupportDetails details = querySwapChainSupport(physicalDevice);
    swapChainOK = !details.formats.empty() && !details.presentModes.empty();
    if (!swapChainOK){
        ENG_LOG_ERROR("swap chain is not OK")
    }

    return indices.isComplete() && extensionsSupported && swapChainOK;
}

engResult Engine::createVulkanSurface(){
    if (glfwCreateWindowSurface(instance, window, nullptr, &vulkanSurface) != VK_SUCCESS){
        ENG_LOG_ERROR("Failed to create Vulkan window surface");
        return ENG_RESULT_FAILURE;
    }
    return ENG_RESULT_SUCCESS;
}

bool Engine::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice){
    u32 extensionCount;

    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    // check if all of the required extensions are amongst extensions
    for (const auto& extension: availableExtensions){
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails Engine::querySwapChainSupport(VkPhysicalDevice physicalDevice){
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, vulkanSurface, &details.capabilities);

    u32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vulkanSurface, &formatCount, nullptr);
    if (formatCount != 0){
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vulkanSurface, &formatCount, details.formats.data());
    }

    u32 presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vulkanSurface, &presentModeCount, nullptr);
    if (presentModeCount != 0){
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vulkanSurface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR Engine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){

    for (const auto& format: availableFormats){
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
            return format;
        }
    }

    // if no format with SRGB and nolinear colour space
    return availableFormats[0];
}

VkPresentModeKHR Engine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){


    for (const auto& presentMode : availablePresentModes){
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR){
            return presentMode;
        }
    }

    // by default
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Engine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities){

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
        return capabilities.currentExtent;
    }
    else{
        i32 width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            (uint32_t)width,
            (uint32_t)height
        };

        actualExtent.width = std::clamp(actualExtent.width, 
                            capabilities.minImageExtent.width, 
                            capabilities.maxImageExtent.width);

        actualExtent.height = std::clamp(actualExtent.height,
                            capabilities.minImageExtent.height,
                            capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

engResult Engine::createSwapChain(){

    ENG_LOG_INFO("Creating swap chain...");

    SwapChainSupportDetails  SwapChainSupport = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(SwapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(SwapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(SwapChainSupport.capabilities);

    u32 imageCount = SwapChainSupport.capabilities.minImageCount + 1;
    if (SwapChainSupport.capabilities.maxImageCount > 0 && imageCount > SwapChainSupport.capabilities.maxImageCount){
        imageCount = SwapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = vulkanSurface;
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // render directli to images without postprocessing

    QueueFamilyIndices indicies = findQueueFamilies(physicalDevice);
    u32 queueFamilyIndices[] = {indicies.graphicsFamily.value(), indicies.presentFamily.value()};

    if (indicies.graphicsFamily != indicies.presentFamily){
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    } 
    else{
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    swapChainCreateInfo.preTransform = SwapChainSupport.capabilities.currentTransform;

    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;

    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS){
        return ENG_RESULT_FAILURE;
    }    

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    return ENG_RESULT_SUCCESS;
}

engResult Engine::createImageViews(){

    ENG_LOG_INFO("Creating image views...");

    swapChainImageViews.resize(swapChainImages.size());

    for (i32 i = 0; i < swapChainImages.size(); i++){
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS){
            return ENG_RESULT_FAILURE;
        }
    }

    return ENG_RESULT_SUCCESS;
}