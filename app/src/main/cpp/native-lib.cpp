#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_administrator_jinc_TestNative_stringFromJNI(JNIEnv *env, jobject instance) {

       /* std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());*/

    return env->NewStringUTF("stringFromJNI");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_administrator_jinc_TestNative_accessField(JNIEnv *env, jobject obj) {

    jclass cls;
    cls = env->GetObjectClass( obj) ;
    //jfieldID
    //签名：类型的简称
    //属性，方法
    jfieldID fid = env->GetFieldID( cls, "key", "Ljava/lang/String;");

    //获取key属性的值
    //注意：key为基本数据类型，规则如下
    //env->GetIntField(); env->Get<Type>Field();
    jobject jstr = env->GetObjectField( obj, fid);
    //jstring转为 C/C++字符串
    printf("jstr:%#x\n",&jstr);

    char *str = (char *) env->GetStringUTFChars((jstring) jstr, JNI_FALSE);
    //拼接字符串
    char text[50] = "super ";
    strcat(text,str);

    //拼接完成之后，从C字符串转为jstring
   jstring jstr2 = env->NewStringUTF( text);

    //修改key的属性
    //注意规则：Set<Type>Field
    env->SetObjectField( obj, fid, jstr2);

    return jstr2;

//    return env->NewStringUTF("accessField");
}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_accessStaticField(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_accessMethod(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_accessStaticMethod(JNIEnv *env, jclass type) {

    // TODO

}

JNIEXPORT jobject JNICALL
Java_com_example_administrator_jinc_TestNative_accessConstructor(JNIEnv *env, jobject instance) {

    // TODO

}


JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_callNonvirtualMethod(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT jstring JNICALL
Java_com_example_administrator_jinc_TestNative_chineseChars(JNIEnv *env, jobject instance,
                                                            jstring text_) {
    const char *text = env->GetStringUTFChars(text_, 0);

    // TODO

    env->ReleaseStringUTFChars(text_, text);

    return env->NewStringUTF("chineseChars");
}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_giveArray(JNIEnv *env, jobject instance,
                                                         jintArray array_) {
    jint *array = env->GetIntArrayElements(array_, NULL);

    // TODO

    env->ReleaseIntArrayElements(array_, array, 0);
}

JNIEXPORT jintArray JNICALL
Java_com_example_administrator_jinc_TestNative_getArray(JNIEnv *env, jobject instance, jint len) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_localRef(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_createGlobalRef(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT jstring JNICALL
Java_com_example_administrator_jinc_TestNative_getGlobalRef(JNIEnv *env, jobject instance) {

    // TODO


    return env->NewStringUTF("getGlobalRef");
}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_deleteGlobalRef(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT jstring JNICALL
Java_com_example_administrator_jinc_TestNative_stringFromJNIStatic(JNIEnv *env, jclass type) {

    // TODO


    return env->NewStringUTF("JniStatic");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_administrator_jinc_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF("MainActivity_stringFromJNI");
}
