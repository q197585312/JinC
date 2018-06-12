#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jni.h>


//可以不需要，按照行业规范一般需要编写头文件（文档）
#include "com_tz_jni_TestNative.h"
//#include "jni.h"


//宏定义，起说明作用
//增强理解，代表是一个JNI函数
JNIEXPORT jstring JNICALL Java_com_tz_jni_TestNative_stringFromJNI
(JNIEnv * env, jobject jobject){
	return (*env)->NewStringUTF(env, "jni development.");
}

//env，是一个结构体指针的指针，主要用来在C/C++中使用虚拟机的功能
//比如说：访问Java方法，属性，创建Java对象、处理字符串等等

//jobject 结构体
//如果native方法不是静态方法，jobject代表该方法所属的java对象
//如果native方法是静态方法，jobject代表该方法所属Java类的class对象 //TestNative.class


//基本数据
//Java基本数据类型与JNI数据类型的映射关系（在C/C++中用特定的类型来表示Java的数据类型）
/*
boolean jboolean
byte jbyte
char jchar
short jshort
int jint
long jlong
float jfloat
double jdouble
void void
*/

//引用类型（对象）
//String jstring
//object jobject
//基本类型的数组也是引用类型
//byte[] jByteArray
//Class  jclass


//C/C++访问Java的成员

//1.访问属性
//修改属性key的字符串
JNIEXPORT jstring JNICALL Java_com_tz_jni_TestNative_accessField(JNIEnv * env, jobject obj){	
	//得到class
	jclass cls = (*env)->GetObjectClass(env, obj);	
	//jfieldID
	//签名：类型的简称
	//属性，方法
	jfieldID fid = (*env)->GetFieldID(env, cls, "key", "Ljava/lang/String;");	

	//获取key属性的值
	//注意：key为基本数据类型，规则如下
	//(*env)->GetIntField(); (*env)->Get<Type>Field();
	jstring jstr = (*env)->GetObjectField(env, obj, fid);
	
	//jstring转为 C/C++字符串
	char *str = (*env)->GetStringUTFChars(env, jstr, NULL);
	//拼接字符串
	char text[50] = "super ";
	strcat(text,str);

	//拼接完成之后，从C字符串转为jstring
	jstr = (*env)->NewStringUTF(env, text);

	//修改key的属性
	//注意规则：Set<Type>Field
	(*env)->SetObjectField(env, obj, fid, jstr);

	return jstr;
}

//访问静态属性
//count属性+10
JNIEXPORT void JNICALL Java_com_tz_jni_TestNative_accessStaticField(JNIEnv * env, jobject obj){
	//jclass
	jclass cls = (*env)->GetObjectClass(env, obj);
	//jfieldID
	jfieldID fid = (*env)->GetStaticFieldID(env, cls, "count", "I");
	//获取静态属性的值
	//规则：GetStatic<Type>Field
	jint count = (*env)->GetStaticIntField(env, cls, fid);
	count += 10;
	//修改属性的值
	//规则：SetStatic<Type>Field
	(*env)->SetStaticIntField(env, cls, fid, count);
}

//访问java方法
//获取签名
//javap -s -p 类名
//借用java的api产生指定大小的随机数
JNIEXPORT void JNICALL Java_com_tz_jni_TestNative_accessMethod(JNIEnv * env, jobject obj){
	//jclass
	jclass cls = (*env)->GetObjectClass(env, obj);
	//jmethodID
	jmethodID mid = (*env)->GetMethodID(env, cls, "genRandomInt", "(I)I");
	//调用方法，产生了一个随机数
	//规则：Call<Type>Method 返回值类型
	jint random = (*env)->CallIntMethod(env, obj, mid, 200);

	//打印出来看看
	FILE *fp = fopen("D://log.txt", "w");
	//int转为字符串
	char str[50];
	sprintf(str, "%d", random);
	fputs(str, fp);
	fclose(fp);
}


//访问静态方法
//借用java api 产生一个UUID字符串，作为文件的名称
JNIEXPORT void JNICALL Java_com_tz_jni_TestNative_accessStaticMethod(JNIEnv * env, jobject cls){	
	//如果native方法为static，jobject为子类jclass的实例，也就是native方法所属的类的Class实例
	//jclass
	//jclass cls = (*env)->GetObjectClass(env, obj);

	//jmethodID	
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "getUUID", "()Ljava/lang/String;");	

	//调用
	//规则：CallStatic<Type>Method
	jstring uuid = (*env)->CallStaticObjectMethod(env, cls, mid);
	//jstring转为C字符串
	char *uuid_str = (*env)->GetStringUTFChars(env, uuid, NULL);
	char filename[100] = {0};
	sprintf(filename, "D://%s.txt", uuid_str);
	FILE *fp = fopen(filename, "w");
	fputs(uuid_str, fp);
	fclose(fp);
}

//访问构造方法
//使用java.util.Date产生一个当前时间时间戳
JNIEXPORT jobject JNICALL Java_com_tz_jni_TestNative_accessConstructor(JNIEnv * env, jobject obj){
	//Date jclass
	jclass cls = (*env)->FindClass(env, "java/util/Date");
	//构造方法jmethodID
	jmethodID contructor_mid = (*env)->GetMethodID(env, cls, "<init>","()V");
	//实例化一个Date对象
	jobject date_obj = (*env)->NewObject(env, cls, contructor_mid);

	//调用getTime方法
	jmethodID mid = (*env)->GetMethodID(env, cls, "getTime", "()J");
	jlong time = (*env)->CallLongMethod(env, date_obj, mid);
	//jlong -> 字符串
	FILE *fp = fopen("D://log.txt", "w");
	char str[50];
	sprintf(str, "%lld", time);
	fputs(str, fp);
	fclose(fp);

	return date_obj;
}

//调用父类的方法
JNIEXPORT void JNICALL Java_com_tz_jni_TestNative_callNonvirtualMethod(JNIEnv * env, jobject obj){
	//获取一个Man对象
	jclass cls = (*env)->GetObjectClass(env, obj);
	jfieldID fid = (*env)->GetFieldID(env, cls, "human", "Lcom/tz/jni/Human;");
	jobject human_obj = (*env)->GetObjectField(env, obj, fid);

	//执行sayHi方法
	jclass human_cls = (*env)->FindClass(env, "com/tz/jni/Human");//注意：传父类的类名
	jmethodID mid = (*env)->GetMethodID(env, human_cls, "sayHi", "()Ljava/lang/String;");
	
	//执行子类覆盖的方法
	//jstring jstr = (*env)->CallObjectMethod(env, human_obj, mid);
	//执行父类的方法
	jstring jstr = (*env)->CallNonvirtualObjectMethod(env, human_obj, human_cls, mid);

	//jstring->char*
	char * str = (*env)->GetStringUTFChars(env, jstr, NULL);
	FILE *fp = fopen("D://log.txt", "w");	
	fputs(str, fp);
	fclose(fp);

}

//中文字符串乱码
JNIEXPORT jstring JNICALL Java_com_tz_jni_TestNative_chineseChars(JNIEnv * env, jobject obj,jstring jstr){
	//java中传入的中文->C字符串	 
	/*char * str = (*env)->GetStringUTFChars(env, jstr, NULL);
	FILE *fp = fopen("D://log.txt", "w");
	fputs(str, fp);
	fclose(fp);*/

	//C字符串->jstring
	char *cstr = "我说中文";
	//jstring j_str = (*env)->NewStringUTF(env, cstr);	
	//借用Java的转码API，返回GB2312中文编码字符串
	//使用这个构造方法，完成转码
	//public String(byte bytes[], String charsetName)
	//执行这个构造方法需要三个东西
	//1.jmethodID
	//2.byte数组(jbyteArray)参数
	//3.charsetName参数(jstring)

	//String类的jclass
	jclass str_cls = (*env)->FindClass(env, "java/lang/String");
	jmethodID constructor_mid = (*env)->GetMethodID(env, str_cls, "<init>", "([BLjava/lang/String;)V");
	
	//char * -> char[] ->jbyteArray -> jbyte * 
	jbyteArray bytes = (*env)->NewByteArray(env, strlen(cstr));
	//bytes数组赋值
	(*env)->SetByteArrayRegion(env, bytes, 0, strlen(cstr), cstr);

	//charsetName="GB2312"
	jstring charsetName = (*env)->NewStringUTF(env, "GB2312");

	//返回GB2312中文编码jstring
	return (*env)->NewObject(env, str_cls, constructor_mid, bytes, charsetName);	
}

//比较器
int compare(jint *a, jint * b){
	return (*b) - (*a);
}
/*
int compare(void *a, void *b){
return *(jint *)(a)-*(jint *)b; //指针void *类型转为jint *
}*/

//数组处理
//传入int数组，对数组进行排序
JNIEXPORT void JNICALL Java_com_tz_jni_TestNative_giveArray(JNIEnv * env, jobject obj, jintArray arr){
	//基本数据类型，传值
	//引用类型，传引用
	//arr，是指向Java数组的指针

	//Java的int数组(jintArray)->C int数组
	jint *elems = (*env)->GetIntArrayElements(env, arr, NULL);
	//数组的长度
	int len = (*env)->GetArrayLength(env, arr);
	//对(jint)long数组进行
	qsort(elems, len, sizeof(jint), compare);

	//同步
	//释放数组的元素
	//mode参数
	//0，Java数组进行更新，并且释放C/C++数组
	//JNI_ABORT，Java数组不进行更新，但是释放C/C++数组
	//JNI_COMMIT，Java数组进行更新，不释放C/C++数组（函数执行完，数组还是会释放）	
	(*env)->ReleaseIntArrayElements(env, arr, elems, JNI_COMMIT);
}

//返回指定大小的数组
JNIEXPORT jintArray JNICALL Java_com_tz_jni_TestNative_getArray(JNIEnv * env, jobject obj,jint len){
	jintArray jint_arr = (*env)->NewIntArray(env, len);
	//赋值
	//jintArray -> jint *
	jint * elems = (*env)->GetIntArrayElements(env, jint_arr, NULL);
	int i = 0;
	//循环赋值
	for (; i < len; i++){
		elems[i] = i;
	}
	//同步
	(*env)->ReleaseIntArrayElements(env, jint_arr, elems, 0);

	return jint_arr;
}

//JNI引用变量
//C/C++代码必须告知Java虚拟机，什么时候能回收一个对象，什么时候不能回收这个对象
//引用的类型：局部引用和全局引用

//局部引用
//局部引用会在C/C++代码执行完成之后自动释放（可以回收）
//但是，有时候我们需要手动去释放
//1.访问一个很大的Java对象，使用完之后，还要进行复杂的耗时操作
//2.创建了大量的局部引用，占用了太多的内存，而且这些布局引用跟后面的操作没有关联性

//模拟，循环创建对象数组
JNIEXPORT void JNICALL Java_com_tz_jni_TestNative_localRef(JNIEnv * env, jobject obj){
	int i = 0;
	for (; i < 5; i++){
		jclass cls = (*env)->FindClass(env, "java/util/Date");
		jmethodID constructor_mid = (*env)->GetMethodID(env, cls, "<init>", "()V");
		//实例化Date对象
		jobject obj = (*env)->NewObject(env, cls, constructor_mid);
		//Date对象数组
		jobjectArray jobj_arr = (*env)->NewObjectArray(env, 5, cls, obj);
		//提前释放，不要占用内存太久
		//告诉虚拟机垃圾回收器，可以回收这些对象
		(*env)->DeleteLocalRef(env, obj);
		(*env)->DeleteLocalRef(env, jobj_arr);

		//processing 假装这里有很多代码
	}
}

//注意：局部引用不能在多个线程间传递

//全局引用
//可以跨越多个线程，在程序员手动释放之前，一直有效
jstring global_str;

//设置global_str
JNIEXPORT void JNICALL Java_com_tz_jni_TestNative_createGlobalRef(JNIEnv * env, jobject j_obj){
	jstring obj = (*env)->NewStringUTF(env, "jni development is powerful!");
	global_str = (*env)->NewGlobalRef(env, obj);
}

//访问global_str
JNIEXPORT jstring JNICALL Java_com_tz_jni_TestNative_getGlobalRef(JNIEnv * env, jobject obj){
	return global_str;
}

//释放global_str
JNIEXPORT void JNICALL Java_com_tz_jni_TestNative_deleteGlobalRef(JNIEnv * env, jobject obj){
	(*env)->DeleteGlobalRef(env, global_str);
}

JNIEXPORT jstring JNICALL
Java_com_example_administrator_jinc_TestNative_stringFromJNI(JNIEnv *env, jobject instance) {

	// TODO


	return (*env)->NewStringUTF(env, "111");
}