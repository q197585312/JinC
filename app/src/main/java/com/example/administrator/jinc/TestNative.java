package com.example.administrator.jinc;

import java.util.Date;
import java.util.Random;
import java.util.UUID;

public class TestNative {

	//非静态的属性
	public String key = "jason's key";
	//静态属性
	public static int count = 10;
	//父类类型的引用，指向子类类型的对象
	private Human human = new Man();
	
	//
	//private int count = 9;
	public native static String stringFromJNIStatic();
	//本地方法
	public native String stringFromJNI();
	
	public native String accessField();
	
	public native void accessStaticField();
	
	public native void accessMethod();
	
	public native static void accessStaticMethod();
	
	public native Date accessConstructor();
	
	public native void callNonvirtualMethod();
	
	public native String chineseChars(String text);
	
	public native void giveArray(int[] array);
	
	public native int[] getArray(int len);
	
	public native void localRef();
	
	public native void createGlobalRef();
	
	public native String getGlobalRef();
	
	public native void deleteGlobalRef();
	
	public static void main(String[] args) {
		TestNative t = new TestNative();
		//调用stringFromJNI方法，实际会去查找调用
		//jstring Java_com_tz_jni_TestNative_stringFromJNI函数
		//javah工具：帮助我们生成一个.h头文件，当中包含了native方法的函数声明
		//我们也可以不适用javah工具，自己手写函数名称
		System.out.println(t.stringFromJNI());
		
		//System.out.println(t.accessField());
		//System.out.println(t.key);
		//t.accessStaticField();
		//System.out.println(count);
		//t.accessMethod();
		//t.accessStaticMethod();
		//Date date = t.accessConstructor();
		//System.out.println(date.getTime());
		
		//t.callNonvirtualMethod();
		
		//System.out.println(t.chineseChars("我是中国人"));
		
		/*int[] array = {89,100,34,2,10,5};
		//排序
		t.giveArray(array);
		for (int i : array) {
			System.out.println(i);
		}*/
		
		//List<Integer> list = null;
		//比较器 Comparator 比较器，指定比较的规则
		//Collections.sort(list, c);
		/*int[] array = t.getArray(5);
		for (int i : array) {
			System.out.println(i);
		}*/
		
		//t.localRef();
		
		t.createGlobalRef();
		
		//休眠
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		System.out.println(t.getGlobalRef());
		
		//休眠
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		//释放
		t.deleteGlobalRef();	
		//空指针异常
		System.out.println(t.getGlobalRef());
	}
	
	/**
	 * 产生一个指定大小的随机数
	 * @param max
	 * @return
	 */
	public int genRandomInt(int max){
		Random random = new Random();
		return random.nextInt(max);
	}
	
	/**
	 * 删除UUID随机字符串
	 * @return
	 */
	public static String getUUID(){
		UUID uuid = UUID.randomUUID();
		return uuid.toString().replaceAll("-", "");
	}


	static {
		System.loadLibrary("native-lib");
	}
}
