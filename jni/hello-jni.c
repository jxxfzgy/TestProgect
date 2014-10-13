/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jstring Java_com_zgy_slidingmenudemo02_lib_Libjava_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello from JNI !");
}

void Java_com_zgy_slidingmenudemo02_lib_Libjava_nativeReadDirectory(JNIEnv *env, jobject thiz, jstring path, jobject arrayList)
{
    jboolean isCopy;
    /* Get C string */
    const char* psz_path = (*env)->GetStringUTFChars(env, path, &isCopy);

    DIR* p_dir = opendir(psz_path);
    (*env)->ReleaseStringUTFChars(env, path, psz_path);
    if(!p_dir)
        return;

    jclass arrayClass = (*env)->FindClass(env, "java/util/ArrayList");
    jmethodID methodID = (*env)->GetMethodID(env, arrayClass, "add", "(Ljava/lang/Object;)Z");

    struct dirent* p_dirent;
    jstring str;
    while(1) {
        errno = 0;
        p_dirent = readdir(p_dir);
        if(p_dirent == NULL) {
            if(errno > 0) /* error reading this entry */
                continue;
            else if(errno == 0) /* end of stream */
                break;
        }
        str = (*env)->NewStringUTF(env, p_dirent->d_name);
        (*env)->CallBooleanMethod(env, arrayList, methodID, str);
        (*env)->DeleteLocalRef(env, str);
    }
    closedir(p_dir);
}

jboolean Java_com_zgy_slidingmenudemo02_lib_Libjava_nativeIsPathDirectory(JNIEnv *env, jobject thiz, jstring path)
{
    jboolean isCopy;
    /* Get C string */
    const char* psz_path = (*env)->GetStringUTFChars(env, path, &isCopy);

    jboolean isDirectory;
    struct stat buf;
    if(stat(psz_path, &buf) != 0)
        /* couldn't stat */
        isDirectory = JNI_FALSE;
    else {
        if(S_ISDIR(buf.st_mode))
            isDirectory = JNI_TRUE;
        else
            isDirectory = JNI_FALSE;
    }

    (*env)->ReleaseStringUTFChars(env, path, psz_path);
    return isDirectory;
}

