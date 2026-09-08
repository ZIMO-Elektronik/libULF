import java.util.Properties

plugins {
    id("com.android.library")
}

val localProperties = Properties().apply {
    val file = rootProject.file("local.properties")

    if (file.exists()) {
        file.inputStream().use {
            load(it)
        }
    }
}

fun getProperty(
    gradleProperty: String,
    environmentVariables: List<String>
): String? {
    // 1. Check gradle properties
    providers.gradleProperty(gradleProperty).orNull?.let {
        return it
    }
    
    val extraProperty = gradleProperty.split(".").mapIndexed { index, value ->
        if (index > 0) value.replaceFirstChar{it.uppercase()} else value }.joinToString("")
    // 2. Check rootProject.extra
    if (rootProject.extra.has(extraProperty)) {
        rootProject.extra[extraProperty]?.toString()?.let {
            return it
        }
    }

    // 3. Check project.extra
    if (project.extra.has(extraProperty)) {
        project.extra[extraProperty]?.toString()?.let {
            return it
        }
    }

    // 4. Check environment variables
    environmentVariables.firstNotNullOfOrNull {
        System.getenv(it)
    }?.let {
        return it
    }

    // 5. Check local.properties
    gradleProperty?.let {
        localProperties.getProperty(it)?.let { value ->
            return value
        }
    }
    return null
}



val sdkDir = File(
    getProperty(
        gradleProperty = "sdk.dir",
        environmentVariables = listOf(
            "ANDROID_SDK_ROOT",
            "ANDROID_HOME"
        )
    ) ?: throw GradleException(
        "Android SDK not found."
    )
)

fun compareVersions(a: String, b: String): Int {
    val av = a.split(".").map { it.toIntOrNull() ?: 0 }
    val bv = b.split(".").map { it.toIntOrNull() ?: 0 }

    for (i in 0 until maxOf(av.size, bv.size)) {
        val ai = av.getOrElse(i) { 0 }
        val bi = bv.getOrElse(i) { 0 }

        if (ai != bi) {
            return ai.compareTo(bi)
        }
    }

    return 0
}

fun findLatestVersion(
    directory: File,
    minimumVersion: String
): String =
    directory.listFiles()
        ?.asSequence()
        ?.filter { it.isDirectory }
        ?.map { it.name }
        ?.filter { compareVersions(it, minimumVersion) >= 0 }
        ?.maxWithOrNull(::compareVersions)
        ?: throw GradleException(
            "No version >= $minimumVersion found in $directory"
        )

val cmakeMinVersion = "3.25.0"
val ndkMinVersion = "29.0.14206865"

val cmakeVer = getProperty(
    gradleProperty = "cmake.version",
    environmentVariables = listOf(
        "CMAKE_VERSION"
    )
) ?: findLatestVersion(
    File(sdkDir, "cmake"),
    cmakeMinVersion
)

val ndkVer = getProperty(
    gradleProperty = "ndk.version",
    environmentVariables = listOf(
        "NDK_VERSION"
    )
) ?: findLatestVersion(
    File(sdkDir, "ndk"),
    ndkMinVersion
)

android {
    namespace = "at.zimo.klug"
    compileSdk = 36

    defaultConfig {
        minSdk = 24
        ndkVersion = ndkVer
    }

    externalNativeBuild {
        cmake {
            path = file("cpp/CMakeLists.txt")
            version = cmakeVer
        }
    }
}
