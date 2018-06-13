package com.example.administrator.jinc;

public class Cryptor {

	
	public native  void crypt(String normal_path, String crypt_path);
	
	
	public native  void decrypt(String crypt_path, String decrypt_path);

}