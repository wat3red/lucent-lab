#include <oxorany/oxorany.h>
#include <xorstr.h>
#include "il2cpp_utils.h"

std::unordered_map<std::string, Il2CppClass*> Il2CppUtils::cachedKlasses = {};

Il2CppType* GetTypeByName(const char* typeName) {
    Il2CppClass* klass = nullptr;
    LOGD("GetTypeByName: typeName = '%s'", typeName);

    std::unordered_map<std::string, std::string> map = {
        {oxorany("object"), oxorany("System.Object")},
        {oxorany("string"), oxorany("System.String")},
        {oxorany("int"),    oxorany("System.Int32")},
        {oxorany("bool"),   oxorany("System.Boolean")},
        {oxorany("byte"),   oxorany("System.Byte")},
        {oxorany("float"),  oxorany("System.Single")},
        {oxorany("double"), oxorany("System.Double")},
        {oxorany("short"),  oxorany("System.Int16")},
        {oxorany("long"),   oxorany("System.Int64")},
        {oxorany("char"),   oxorany("System.Char")}
    };

    auto it = map.find(std::string(typeName));  // <= оборачиваем в std::string
    std::string fullname = (it == map.end()) ? typeName : it->second;
    size_t dot = fullname.find('.');
    std::string ns = fullname.substr(0, dot);
    std::string name = fullname.substr(dot + 1);

    klass = Il2CppUtils::FindKlass(ns.c_str(), name.c_str());
    if (!klass) {
        LOGE("GetTypeByName: failed to find class for type '%s'", typeName);
        return nullptr;
    }
    LOGD("GetTypeByName: found class %s.%s => %p", ns.c_str(), name.c_str(), klass);

    return &klass->_1.byval_arg;
}

MethodInfo* Il2CppUtils::FindMethod(Il2CppClass* klass, const char* methodName, int paramCount, std::initializer_list<std::string> paramTypeNames) {
    void* iter = nullptr;

    // LOGD("Il2CppUtils::FindMethod has been called with klass = %s, methodName = %s", klass->_1.name, methodName);

    while (MethodInfo* method = il2cpp_class_get_methods(klass, &iter)) {
        if (strcmp(method->name, methodName) != 0)
            continue;

        if ((method->parameters_count != paramCount) && (paramCount != -1))
            continue;

        bool match = true;
        int index = 0;
        for (const std::string& typeName : paramTypeNames) {
            // LOGD("Checking parameter type: %s", typeName.c_str());

            Il2CppType* expected = GetTypeByName(typeName.c_str());
            const Il2CppType* actual = method->parameters[index];

            if (!expected || !actual) {
                match = false;
                break;
            }

            Il2CppClass* klassExpected = il2cpp_class_from_type(expected);
            Il2CppClass* klassActual = il2cpp_class_from_type(actual);

            if (!klassExpected || !klassActual) {
                match = false;
                break;
            }

            if (!expected || klassActual != klassExpected) {
                match = false;
                break;
            }
            index++;
        }

        if (match) {
            // LOGD("FindMethod: method %s found, offset: %p", methodName, (void*)((uintptr_t)method->methodPointer - g_LibUnityBase));
            return method;
        }
    }

    LOGE("FindMethod: method %s not found in %s", methodName, klass->_1.name);
    return nullptr;
}

Il2CppClass* Il2CppUtils::FindKlass(const char* namespaceName, const char* className) {
    const char* images[]{
        ("Assembly-CSharp.dll"),
        ("Bolt.Api.dll"),
        ("mscorlib.dll"),
        ("UnityEngine.UIElementsModule.dll"),
        ("System.Xml.dll"),
        ("System.dll"),
        ("Bolt.dll"),
        ("System.Data.dll"),
        ("UnityEngine.CoreModule.dll"),
        ("Unity.RenderPipelines.Core.Runtime.dll"),
        ("System.Core.dll"),
        ("Unity.ProBuilder.dll"),
        ("Newtonsoft.Json.dll"),
        ("Unity.TextMeshPro.dll"),
        ("Google.Protobuf.dll"),
        ("Unity.Services.Vivox.dll"),
        ("UnityEngine.UI.dll"),
        ("FMODUnity.dll"),
        ("UnityEngine.TextCoreTextEngineModule.dll"),
        ("Assembly-CSharp-firstpass.dll"),
        ("zxing.unity.dll"),
        ("Unity.Postprocessing.Runtime.dll"),
        ("Photon3Unity3D.dll"),
        ("Facebook.Unity.dll"),
        ("Unity.LevelPlay.dll"),
        ("UnityEngine.Purchasing.Stores.dll"),
        ("RpcSupport.dll"),
        ("UnityEngine.IMGUIModule.dll"),
        ("Unity.Timeline.dll"),
        ("Mono.Security.dll"),
        ("System.Net.Http.dll"),
        ("System.IO.Compression.dll"),
        ("UnityEngine.AndroidJNIModule.dll"),
        ("AdjustSdk.Scripts.dll"),
        ("IngameDebugConsole.Runtime.dll"),
        ("UnityEngine.dll"),
        ("JetBrains.Lifetimes.dll"),
        ("Firebase.App.dll"),
        ("System.Drawing.dll"),
        ("UnityEngine.PropertiesModule.dll"),
        ("Axlebolt.Ballistics.Simulation.dll"),
        ("Axlebolt.Standoff2.DLCSupport.Runtime.dll"),
        ("UnityEngine.PhysicsModule.dll"),
        ("UnityEngine.AnimationModule.dll"),
        ("AppMetrica.dll"),
        ("K4os.Compression.LZ4.dll"),
        ("System.Numerics.dll"),
        ("UnityEngine.Purchasing.dll"),
        ("System.Xml.Linq.dll"),
        ("Axlebolt.Ballistics.GrenadePractice.dll"),
        ("BoltMatchmaking.dll"),
        ("Unity.Services.Core.Internal.dll"),
        ("UnityEngine.UnityWebRequestModule.dll"),
        ("UnityEngine.Purchasing.Security.dll"),
        ("UnityEngine.TextCoreFontEngineModule.dll"),
        ("Bolt.Matchmaking.dll"),
        ("Unity.ProBuilder.Poly2Tri.dll"),
        ("Firebase.Crashlytics.dll"),
        ("Firebase.Messaging.dll"),
        ("Firebase.Platform.dll"),
        ("UnityEngine.UIModule.dll"),
        ("UnityEngine.InputLegacyModule.dll"),
        ("UnityEngine.ParticleSystemModule.dll"),
        ("Firebase.RemoteConfig.dll"),
        ("UnityEngine.AudioModule.dll"),
        ("UnityEngine.TextRenderingModule.dll"),
        ("UnityEngine.Physics2DModule.dll"),
        ("UnityEngine.SharedInternalsModule.dll"),
        ("UnityEngine.UnityAnalyticsModule.dll"),
        ("Axlebolt.FpsOverlay.dll"),
        ("Unity.Services.Core.Registration.dll"),
        ("Unity.ProBuilder.KdTree.dll"),
        ("UnityEngine.VideoModule.dll"),
        ("Google.Play.Common.dll"),
        ("Axlebolt.Integrations.Intercom.dll"),
        ("Axlebolt.Logging.dll"),
        ("Firebase.DynamicLinks.dll"),
        ("GoogleSignIn.dll"),
        ("UnityEngine.AssetBundleModule.dll"),
        ("UnityEngine.DirectorModule.dll"),
        ("Firebase.Analytics.dll"),
        ("FMODUnityResonance.dll"),
        ("Axlebolt.Standoff2.DLCSupport.Providers.StreamingAssets.Runtime.dll"),
        ("UnityEngine.ClothModule.dll"),
        ("UnityEngine.JSONSerializeModule.dll"),
        ("UnityEngine.UnityWebRequestAssetBundleModule.dll"),
        ("AndroidRuntimePermissions.Runtime.dll"),
        ("Axlebolt.Toast.dll"),
        ("NativeGallery.Runtime.dll"),
        ("UnityEngine.ImageConversionModule.dll"),
        ("UnityEngine.TilemapModule.dll"),
        ("Google.Play.Core.dll"),
        ("Purchasing.Common.dll"),
        ("UnityEngine.GridModule.dll"),
        ("UnityEngine.ScreenCaptureModule.dll"),
        ("UnityEngine.SpriteShapeModule.dll"),
        ("Google.Play.Integrity.dll"),
        ("Unity.Services.Core.Configuration.dll"),
        ("Unity.Services.Core.dll"),
        ("Unity.Services.Core.Scheduler.dll"),
        ("Axlebolt.Integrations.Intercom.Android.dll"),
        ("Axlebolt.Integrations.Varioqub.Android.dll"),
        ("Facebook.Unity.Settings.dll"),
        ("Axlebolt.NetCode.Serialization.dll"),
        ("NativeShare.Runtime.dll"),
        ("Axlebolt.Integrations.GDPR.dll"),
        ("Axlebolt.FPSTracking.Runtime.dll"),
        ("Unity.Services.Core.Device.dll"),
        ("Axlebolt.Warden.Android.dll"),
        ("Beebyte.Obfuscator.dll"),
        ("System.Runtime.Serialization.dll"),
        ("UnityEngine.Purchasing.SecurityCore.dll"),
        ("Axlebolt.Integrations.Varioqub.Base.dll"),
        ("Axlebolt.Integrations.VK.Android.dll"),
        ("Axlebolt.Integrations.VK.dll"),
        ("System.Runtime.CompilerServices.Unsafe.dll"),
        ("Axlebolt.Integrations.TWA.dll"),
        ("Axlebolt.Integrations.Varioqub.dll"),
        ("System.Configuration.dll"),
        ("System.IO.Compression.FileSystem.dll"),
        ("UnityEngine.Purchasing.WinRTCore.dll"),
        ("Axlebolt.Integrations.QRCode.dll"),
        ("Unity.Services.Core.Telemetry.dll"),
        ("Axlebolt.Ballistics.GrenadeLogic.dll"),
        ("Axlebolt.Integrations.Utils.Runtime.dll"),
        ("Unity.Services.Core.Environments.dll"),
        ("UnityEngine.Purchasing.AppleCore.dll"),
        ("UnityEngine.Purchasing.AppleMacosStub.dll"),
        ("UnityEngine.Purchasing.AppleStub.dll"),
        ("Axlebolt.Integrations.GameCenter.dll"),
        ("Facebook.Unity.Android.dll"),
        ("Unity.Services.Core.Environments.Internal.dll"),
        ("Unity.Services.Core.Threading.dll"),
        ("UnityEngine.Purchasing.WinRTStub.dll"),
        ("__Generated")
    };

    // LOGE("Il2CppUtils::FindKlass has been called with namespaceName = %s, className = %s", namespaceName, className);

    if (auto search = cachedKlasses.find(className); search != cachedKlasses.end()) {
        return search->second;
    }

    for (int i = 0; i < (sizeof(images) / sizeof(const char*)); i++) {
        Il2CppAssembly* assembly = il2cpp_domain_assembly_open(images[i]);
        if (!assembly) {
            LOGE("Skiping null assembly with name %s", images[i]);
            continue;
        }

        Il2CppImage* image = *(Il2CppImage**)(assembly);
        if (!image) {
            LOGE("Could not retrieve image from assembly %s", images[i]);
            continue;
        }

        // Attempt to find the class within this image.
        Il2CppClass* klass = il2cpp_class_from_name(image, namespaceName, className);
        if (klass) {
            // LOGD("Success: Class %s found sucessfully", className);
            cachedKlasses.insert({ std::string(namespaceName) + oxorany(".") + className, klass });
            return klass;
        }
    }
    LOGE("Class %s not found in any assembly", className);

    return nullptr;
}

void* Il2CppUtils::ReplaceVirtualMethod(Il2CppClass* klass, uint8_t oldSlot, uint8_t newSlot) {
    void* oldMethodPtr = (void*)klass->vtable[oldSlot].methodPtr;
    klass->vtable[oldSlot] = klass->vtable[newSlot];
    return oldMethodPtr;
}

void* Il2CppUtils::ReplaceVirtualMethod(const char* namespaceName, const char* className, uint8_t oldSlot, uint8_t newSlot) {
    Il2CppClass* klass = Il2CppUtils::FindKlass(namespaceName, className);
    void* oldMethodPtr = (void*)klass->vtable[oldSlot].methodPtr;
    klass->vtable[oldSlot] = klass->vtable[newSlot];
    return oldMethodPtr;
}

void* Il2CppUtils::ReplaceVirtualMethod(const char* namespaceName, const char* className, uint8_t slot, void* hook) {
    Il2CppClass* klass = Il2CppUtils::FindKlass(namespaceName, className);
    void* oldMethodPtr = (void*)klass->vtable[slot].methodPtr;
    klass->vtable[slot].methodPtr = (Il2CppMethodPointer*)hook;
    return oldMethodPtr;
}

void* Il2CppUtils::ReplaceVirtualMethod(Il2CppClass* klass, uint8_t slot, void* hook) {
    void* oldMethodPtr = (void*)klass->vtable[slot].methodPtr;
    klass->vtable[slot].methodPtr = (Il2CppMethodPointer*)hook;
    return oldMethodPtr;
}

#define METHOD_ATTRIBUTE_VIRTUAL 0x0040
#define IS_VIRTUAL(method) ((method)->flags & METHOD_ATTRIBUTE_VIRTUAL)
int Il2CppUtils::FindVirtualMethodSlot(Il2CppClass* klass, const char* methodName, int params) {
    void* iter = nullptr;
    int slot = 0;
    while (MethodInfo* method = il2cpp_class_get_methods(klass, &iter)) {
        if (IS_VIRTUAL(method)) {
            if (!strcmp(method->name, methodName) && method->parameters_count == params) {
                return slot;
            }
            slot++;
        }
    }
    return -1;
}