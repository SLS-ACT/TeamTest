package com.mztgame.dxccs.uc;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.PixelFormat;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class ChannelSplash extends Activity {

	private static final int LOAD_DISPLAY_TIME = 2000;
	
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    
	    getWindow().setFormat(PixelFormat.RGBA_8888);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_DITHER);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);

	    setContentView(R.layout.channelsplash);
	    
	    new Handler().postDelayed(new Runnable() {
	        public void run() {
	            /* Create an Intent that will start the Main WordPress Activity. */
	            Intent mainIntent = new Intent(ChannelSplash.this, uc.class);
	            ChannelSplash.this.startActivity(mainIntent);
	            ChannelSplash.this.finish();
	        }
	    }, LOAD_DISPLAY_TIME); //2000 for release
	}
}
