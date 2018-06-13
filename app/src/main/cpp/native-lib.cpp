#include <jni.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>


int compare(const void *a, const void *b) {
    return *(jint *) (a) - *(jint *) b;
}

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
    cls = env->GetObjectClass(obj);
    //jfieldID
    //签名：类型的简称
    //属性，方法
    jfieldID fid = env->GetFieldID(cls, "key", "Ljava/lang/String;");

    //获取key属性的值
    //注意：key为基本数据类型，规则如下
    //env->GetIntField(); env->Get<Type>Field();
    jobject jstr = env->GetObjectField(obj, fid);
    //jstring转为 C/C++字符串

    char *str = (char *) env->GetStringUTFChars((jstring) jstr, JNI_FALSE);
    //拼接字符串
    char text[50] = "super ";
    strcat(text, str);

    //拼接完成之后，从C字符串转为jstring
    jstring jstr2 = env->NewStringUTF(text);

    //修改key的属性
    //注意规则：Set<Type>Field
    env->SetObjectField(obj, fid, jstr2);

    return jstr2;

//    return env->NewStringUTF("accessField");
}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_accessStaticField(JNIEnv *env, jobject instance) {
    // TODO

}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_administrator_jinc_TestNative_accessMethod(JNIEnv *env, jobject obj) {
    // TODO
    jclass cls = env->GetObjectClass(obj);
    //jmethodID
    jmethodID mid = env->GetMethodID(cls, "genRandomInt", "(I)I");
    //调用方法，产生了一个随机数
    //规则：Call<Type>Method 返回值类型
    jint random = env->CallIntMethod(obj, mid, 200);

    //打印出来看看
    FILE *fp = fopen("/mnt/sdcard/tex.txt", "w");
    //int转为字符串
    char str[50];
    sprintf(str, "%d", random);
    fputs(str, fp);
    fclose(fp);
    return env->NewStringUTF(str);


}

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_accessStaticMethod(JNIEnv *env, jclass type) {

    // TODO

}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_administrator_jinc_TestNative_accessConstructor(JNIEnv *env, jobject instance) {
    jclass cls = env->FindClass("java/util/Date");
    jmethodID jmethodID1 = env->GetMethodID(cls, "<init>", "()V");
    jmethodID jmethodIDGetTime = env->GetMethodID(cls, "getTime", "()J");
    jobject jobject1 = env->NewObject(cls, jmethodID1);
    jlong time = env->CallLongMethod(jobject1, jmethodIDGetTime);
    return time;

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

extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_giveArray(JNIEnv *env, jobject instance,
                                                         jintArray arr) {
    //基本数据类型，传值
    //引用类型，传引用
    //arr，是指向Java数组的指针

    //Java的int数组(jintArray)->C int数组
    jint *elems = env->GetIntArrayElements(arr, NULL);
    //数组的长度
    int len = env->GetArrayLength(arr);
    //对(jint)long数组进行
    qsort(elems, len, sizeof(jint), compare);

    //同步
    //释放数组的元素
    //mode参数
    //0，Java数组进行更新，并且释放C/C++数组
    //JNI_ABORT，Java数组不进行更新，但是释放C/C++数组
    //JNI_COMMIT，Java数组进行更新，不释放C/C++数组（函数执行完，数组还是会释放）
    env->ReleaseIntArrayElements(arr, elems, JNI_COMMIT);
}

JNIEXPORT jintArray JNICALL
Java_com_example_administrator_jinc_TestNative_getArray(JNIEnv *env, jobject instance, jint len) {

    // TODO

}
jstring global_str;
jfieldID key_fid;

jmethodID random_mid;

JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_localRef(JNIEnv *env, jobject instance) {

    int i = 0;
    for (; i < 5; i++) {
        jclass cls = env->FindClass("java/util/Date");
        jmethodID constructor_mid = env->GetMethodID(cls, "<init>", "()V");
        //实例化Date对象
        jobject obj = env->NewObject(cls, constructor_mid);
        //Date对象数组
        jobjectArray jobj_arr = env->NewObjectArray(5, cls, obj);
        //提前释放，不要占用内存太久
        //告诉虚拟机垃圾回收器，可以回收这些对象
        env->DeleteLocalRef(obj);
        env->DeleteLocalRef(jobj_arr);

        //processing 假装这里有很多代码
    }
}
extern "C"
    JNIEXPORT void JNICALL
    Java_com_example_administrator_jinc_TestNative_createGlobalRef(JNIEnv *env, jobject instance) {
        jstring obj = env->NewStringUTF( "jni development is powerful!");
        global_str = static_cast<jstring>(env->NewGlobalRef(obj));

    }
extern "C"
    JNIEXPORT jstring JNICALL
    Java_com_example_administrator_jinc_TestNative_getGlobalRef(JNIEnv *env, jobject instance) {

        return global_str;
    }
extern "C"
    JNIEXPORT void JNICALL
    Java_com_example_administrator_jinc_TestNative_deleteGlobalRef(JNIEnv *env, jobject instance) {
    env->DeleteGlobalRef(global_str);

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

extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_exeception(JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass( jobj);
    jfieldID fid = env->GetFieldID( cls, "key2", "Ljava/lang/String;");
    //检测是否发生Java异常
    jthrowable exception = env->ExceptionOccurred();
    if (exception != NULL){
        //让Java代码可以继续运行
        //清空异常信息
        env->ExceptionClear();

        //补救措施
        fid = env->GetFieldID( cls, "key", "Ljava/lang/String;");
    }

    //获取属性的值
    jstring jstr = static_cast<jstring>(env->GetObjectField(jobj, fid));
    char *str = const_cast<char *>(env->GetStringUTFChars(jstr, NULL));

    //对比属性值是否合法

    if (strcmp(str, "super jason") != 0){
        //认为抛出异常，给Java层处理
        jclass newExcCls = env->FindClass( "java/lang/IllegalArgumentException");
        env->ThrowNew(newExcCls,"key's value is invalid!");
    }

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_cached(JNIEnv *env, jobject jobj) {

      jclass cls = env->GetObjectClass( jobj);
      //获取jfieldID只获取一次
      //局部静态变量
      static jfieldID key_id = NULL;
      if (key_id == NULL){
          key_id = env->GetFieldID( cls, "key", "Ljava/lang/String;");
          printf("--------GetFieldID-------\n");
      }

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_jinc_TestNative_initIds(JNIEnv *env, jclass jcls) {
    key_fid = env->GetFieldID( jcls, "key", "Ljava/lang/String;");
    random_mid = env->GetMethodID( jcls, "genRandomInt", "(I)I");
}