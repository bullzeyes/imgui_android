#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <vector>
#include "VulkanMain.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_vulkan.h>
#include "imgui_impl_android.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "imgui_impl_android", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "imgui_impl_android", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "imgui_impl_android", __VA_ARGS__))

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_init(JNIEnv * env, jobject obj, jobject surface, jint surfaceWidth, jint surfaceHeight, jint windowWidth, jint windowHeight, jstring outputDir);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_pause(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_destroy(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint pointers);
    JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_updateAccelerometerEvent(JNIEnv * env, jobject obj, jlong timestamp, jfloat x, jfloat y, jfloat z);
};

static void check_vk_result(VkResult err)
{
    if (err == 0) return;
    printf("VkResult %d\n", err);
    if (err < 0)
        abort();
}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_init(JNIEnv * env, jobject obj,  jobject surface, jint surfaceWidth, jint surfaceHeight, jint windowWidth, jint windowHeight, jstring outputDir)
{
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);

    if (IsVulkanReady()) {
        DeleteVulkan();
    }

    InitVulkan(nativeWindow);


    jboolean isCopy = false;
    const char *nativeString = env->GetStringUTFChars(outputDir, &isCopy);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAndroid_InitForVulkan(surfaceWidth, surfaceHeight, windowWidth, windowHeight, nativeString);

    extern VulkanDeviceInfo device;
    extern VulkanRenderInfo render;

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device.instance_;
    init_info.PhysicalDevice = device.gpuDevice_;
    init_info.Device = device.device_;
    init_info.QueueFamily = device.queueFamilyIndex_;
    init_info.Queue = device.queue_;
    init_info.PipelineCache = nullptr;
    init_info.DescriptorPool = g_DescriptorPool;
    init_info.Allocator = g_Allocator;
    init_info.MinImageCount = g_MinImageCount;
    init_info.ImageCount = wd->ImageCount;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, render.renderPass_);

    // Upload Fonts
    {
        // Use any command queue
        VkCommandPool command_pool = render.cmdPool_;// wd->Frames[wd->FrameIndex].CommandPool;
        VkCommandBuffer command_buffer = *render.cmdBuffer_;// wd->Frames[wd->FrameIndex].CommandBuffer;

        VkResult err = vkResetCommandPool(device.device_, command_pool, 0);
        check_vk_result(err);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(command_buffer, &begin_info);
        check_vk_result(err);

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        err = vkEndCommandBuffer(command_buffer);
        check_vk_result(err);
        err = vkQueueSubmit(device.queue_, 1, &end_info, VK_NULL_HANDLE);
        check_vk_result(err);

        err = vkDeviceWaitIdle(device.device_);
        check_vk_result(err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    env->ReleaseStringUTFChars(outputDir, nativeString);
}


JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_pause(JNIEnv * env, jobject obj)
{

}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_step(JNIEnv * env, jobject obj)
{
    if (IsVulkanReady()) {
        VulkanDrawFrame();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplAndroid_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        extern VulkanRenderInfo render;
        // Record Imgui Draw Data and draw funcs into command buffer
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *render.cmdBuffer_);

    }
}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_destroy(JNIEnv * env, jobject obj)
{
    if (IsVulkanReady()) {
        DeleteVulkan();

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplAndroid_Shutdown();
        ImGui::DestroyContext();
    }
}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_updateTouchEvent(JNIEnv * env, jobject obj, jint action, jfloat x, jfloat y, jint pointers)
{

}

JNIEXPORT void JNICALL Java_com_example_bullzeyes_imgui_1android_VKViewJniLib_updateAccelerometerEvent(JNIEnv * env, jobject obj, jlong timestamp, jfloat x, jfloat y, jfloat z)
{

}
