import java.util.Properties

plugins {
    id("com.android.library")
}

val localProperties = Properties().apply {
    rootProject.file("local.properties").inputStream().use {
        load(it)
    }
}

val sdkDir = File(
    localProperties.getProperty("sdk.dir")
        ?: throw GradleException("sdk.dir not found in local.properties")
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

val cmakeVersion = findLatestVersion(
    File(sdkDir, "cmake"),
    "3.25.0"
)

val ndkVersion = findLatestVersion(
    File(sdkDir, "ndk"),
    "29.0.14206865"
)


android {
    namespace = "at.zimo.klug"
    compileSdk = 36

    defaultConfig {
        minSdk = 24
        ndkVersion = ndkVersion
    }

    externalNativeBuild {
        cmake {
            path = file("cpp/CMakeLists.txt")
            version = cmakeVersion
        }
    }
}
