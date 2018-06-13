package com.example.administrator.jinc;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        TestNative testNative = new TestNative();
        int[] arrs = new int[]{10, 3, 4, 7, -1, -2, 8};
        testNative.giveArray(arrs);
        StringBuffer buffer = new StringBuffer();
        for (int arr : arrs) {
            buffer.append(arr);
            buffer.append(",");
        }
        tv.setText(buffer.toString());
        testNative.createGlobalRef();
        tv.setText(testNative.getGlobalRef());
        testNative.deleteGlobalRef();
        try {
            testNative.exeception();
        } catch (Exception e) {
            tv.setText(e.getLocalizedMessage());
        }

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
