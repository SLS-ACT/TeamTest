/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import org.cocos2dx.lib.Cocos2dxHelper.Cocos2dxHelperListener;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.Toast;
import android.view.WindowManager;

import com.ztgame.mobileappsdk.common.IZTLibBase;
import com.ztgame.mobileappsdk.common.IZTListener;
import com.ztgame.mobileappsdk.common.ZTConsts;
import com.ztgame.mobileappsdk.common.ZTPayInfo;

public abstract class Cocos2dxActivity extends Activity implements Cocos2dxHelperListener {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = Cocos2dxActivity.class.getSimpleName();
	
	//private native void OnLoginSuc(final String uid, final String token);
	//private native void InitJNIBridge();
	//private native void nativePayFailure();
	
	//public static native void OnInitSuc();
	private native void OnLoginSuc(final String uid, final String token);
	private native void OnInitSuc();
	private native void OnLogoutSuc(final boolean bSDKSurface);
	private native void InitJNIBridge();
	private native void nativePayFailure();
	private native void SureExitGame();
	// ===========================================================
	// Fields
	// ===========================================================
	
	private Cocos2dxGLSurfaceView mGLSurfaceView;
	private Cocos2dxHandler mHandler;
	private static Context sContext = null;
	private static int isExit = 0;
	
	public static Context getContext() {
		return sContext;
	}
	
	private void startSDK() {
		// TODO Auto-generated method stub
		//Log.d("lihaofeng", "KD on login");
		Cocos2dxActivity.this.runOnUiThread(new Runnable() {
			 @Override
             public void run() {
                 try {
                	 IZTLibBase.getInstance().loginZTGame("1", "逐鹿中原", true);
                 }
                 catch(Exception e)
                 {
                     e.printStackTrace();
                 }
             }
		});
		//Log.d("demo", "KD on login end");
	}
	private void logoutSDK() {
		// TODO Auto-generated method stub
		//Log.d("lihaofeng", "KD on login");
		Cocos2dxActivity.this.runOnUiThread(new Runnable() {
			 @Override
             public void run() {
                 try {
                	 if (IZTLibBase.getInstance().isLogined()) {
                		 IZTLibBase.getInstance().switchAccountZTGame();
                	 }
                	 else
                	 {
                		 Toast.makeText(Cocos2dxActivity.this, "未登录", Toast.LENGTH_SHORT).show();
                	 }
                 }
                 catch(Exception e)
                 {
                     e.printStackTrace();
                 }
             }
		});
		//Log.d("demo", "KD on login end");
	}
	// ===========================================================
	// Constructors
	// ===========================================================
	
	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		Log.d("demo", "lihaofeng oncreate");
		super.onCreate(savedInstanceState);
		sContext = this;
    	this.mHandler = new Cocos2dxHandler(this);

    	this.init();
    	getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); 
    	
    	new ProgressDialog(Cocos2dxActivity.this);
    	// 初始化
        IZTLibBase.newInstance(Cocos2dxActivity.this);
        IZTLibBase.getInstance().initZTGame("5009","地下城传说", true, mListener);
        
        InitJNIBridge();
        
		Cocos2dxHelper.init(this, this);
	}

	private IZTListener mListener = new IZTListener() {
        @Override
        public void onFinished(int what, int errcode, JSONObject json_obj) {
            //Toast.makeText(Cocos2dxActivity.this, String.valueOf(what), Toast.LENGTH_SHORT).show();
            switch (what) {
            	case ZTConsts.ZTGAME_INIT:
            			OnInitSuc();
            		break;
                case ZTConsts.ZTGAME_LOGIN:
                    if (0 == errcode) {
                        // 客户端登录成功，登录流程下一步，把返回的信息发送到服务器进行验证
                        //Toast.makeText(Cocos2dxActivity.this, json_obj.toString(), Toast.LENGTH_SHORT).show();
//                        loginVerify(json_obj);
                        
                        try {
							String accid = json_obj.getString("accid");
							String token = json_obj.getString("token");
							
							OnLoginSuc(accid, token);
							
						} catch (JSONException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
                    }
                    else if(-1 == errcode){
                    	startSDK();
                    }
                    else {
                        Toast.makeText(Cocos2dxActivity.this, json_obj.toString(), Toast.LENGTH_SHORT).show();
                    }
                    break;
                case ZTConsts.ZTGAME_PAY:
                    if (0 == errcode) {
                        // 客户端付费成功，付费流程完成需要第三方平台通过通知的形式直接返回到服务器
                        //Toast.makeText(Cocos2dxActivity.this, json_obj.toString(), Toast.LENGTH_SHORT).show();
                        Log.d("giant", "pay ok");
                    }
                    else {
                        //Toast.makeText(Cocos2dxActivity.this, json_obj.toString(), Toast.LENGTH_SHORT).show();
                        Log.d("giant", "pay error");
                        nativePayFailure();
                    }
                    break;
                case ZTConsts.ZTGAME_QUIT:
                    // 只有真正需要退出游戏才返回这个消息，所以这里可以不验证后面的错误码和错误信息
                    //Toast.makeText(Cocos2dxActivity.this, json_obj.toString(), Toast.LENGTH_SHORT).show();
                	SureExitGame();
                    Cocos2dxActivity.this.finish();
                    break;
                case ZTConsts.ZTGAME_ANTIADDICTION:
                    // 防沉迷查询返回信息
                    //Toast.makeText(Cocos2dxActivity.this, json_obj.toString(), Toast.LENGTH_SHORT).show();
                    break;
                case ZTConsts.ZTGAME_SWITCH:
                	if (0 == errcode) {
                        // 客户端登录成功，登录流程下一步，把返回的信息发送到服务器进行验证
                        //Toast.makeText(Cocos2dxActivity.this, json_obj.toString(), Toast.LENGTH_SHORT).show();
//                        loginVerify(json_obj);
                        
                        try {
                        	if(json_obj == null) {
                        		OnLogoutSuc(false);
                        	}
                        	else
                        	{
								String accid = json_obj.getString("accid");
								String token = json_obj.getString("token");
								//OnLogoutSuc(true);
								OnLoginSuc(accid, token);
                        	}
							
						} catch (JSONException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
                    }
                    else {
                        Toast.makeText(Cocos2dxActivity.this, json_obj.toString(), Toast.LENGTH_SHORT).show();
                    }
                	break;
                case ZTConsts.ZTGAME_LOGOUT:
                	Log.d("giant","logout");
                	OnLogoutSuc(true);
                	break;
                default:
                    break;
            }
        }
    };
    
    public void SendAndroidCharge(final String productId,final String productName, final int nPrice, final int nRadio, final int nIndex) {
    	Cocos2dxActivity.this.runOnUiThread(new Runnable() {
			 @Override
            public void run() {
                try {
                	// 支付
                    if (IZTLibBase.getInstance().isLogined()) {
                        // 订单信息
                        ZTPayInfo payInfo = new ZTPayInfo();
                        payInfo.setAmount(nPrice);
                        payInfo.setProductId(productId);
                        payInfo.setProductName(productName);
                        payInfo.setExtra(String.valueOf(nIndex));               // 扩展信息，需要回传游戏服务器的请设置这个字段
                        payInfo.setExchangeRatio(nRadio);
                        IZTLibBase.getInstance().payZTGame(payInfo);
                    }
                    else {
                        Toast.makeText(Cocos2dxActivity.this, "请先登录", Toast.LENGTH_SHORT).show();
                    }
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                }
            }
		});
	}
    
    public void SetAndroidZoneID(final int nZoneID) {
    	 if (IZTLibBase.getInstance().isLogined()) {
             
             IZTLibBase.getInstance().setZoneId(String.valueOf(nZoneID));
         }
         else {
             Toast.makeText(this, "请先登录", Toast.LENGTH_SHORT).show();
         }
    }
	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_BACK:
                IZTLibBase.getInstance().quitZTGame();
                return true;
            default:
                return super.onKeyDown(keyCode, event);
        }
    }
    
	@Override
	protected void onResume() {
		//Log.d("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "onResume");
		super.onResume();

		Cocos2dxHelper.onResume();
		IZTLibBase.getInstance().onResumeZTGame();
		this.mGLSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		Log.d("lihaofeng", "onPause");
		super.onPause();

		Cocos2dxHelper.onPause();
		this.mGLSurfaceView.onPause();
	}

	@Override
    protected void onStop() {
		Log.d("lihaofeng", "onStop");
        IZTLibBase.getInstance().onStopZTGame();
        super.onStop();
    }
	 @Override
	protected void onDestroy() {
		Log.d("lihaofeng", "onDestroy");
        IZTLibBase.getInstance().destroyZTGame();
        IZTLibBase.delInstance();
        super.onDestroy();
	}
	@Override
	public void showDialog(final String pTitle, final String pMessage) {
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_DIALOG;
		msg.obj = new Cocos2dxHandler.DialogMessage(pTitle, pMessage);
		this.mHandler.sendMessage(msg);
	}

	@Override
	public void showEditTextDialog(final String pTitle, final String pContent, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) { 
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_EDITBOX_DIALOG;
		msg.obj = new Cocos2dxHandler.EditBoxMessage(pTitle, pContent, pInputMode, pInputFlag, pReturnType, pMaxLength);
		this.mHandler.sendMessage(msg);
	}
	
	@Override
	public void runOnGLThread(final Runnable pRunnable) {
		this.mGLSurfaceView.queueEvent(pRunnable);
	}

	// ===========================================================
	// Methods
	// ===========================================================
	public void init() {
		
    	// FrameLayout
        ViewGroup.LayoutParams framelayout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                       ViewGroup.LayoutParams.FILL_PARENT);
        FrameLayout framelayout = new FrameLayout(this);
        framelayout.setLayoutParams(framelayout_params);

        // Cocos2dxEditText layout
        ViewGroup.LayoutParams edittext_layout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                       ViewGroup.LayoutParams.WRAP_CONTENT);
        Cocos2dxEditText edittext = new Cocos2dxEditText(this);
        edittext.setLayoutParams(edittext_layout_params);

        // ...add to FrameLayout
        framelayout.addView(edittext);

        // Cocos2dxGLSurfaceView
        this.mGLSurfaceView = this.onCreateView();

        // ...add to FrameLayout
        framelayout.addView(this.mGLSurfaceView);

        this.mGLSurfaceView.setCocos2dxRenderer(new Cocos2dxRenderer());
        this.mGLSurfaceView.setCocos2dxEditText(edittext);

        // Set framelayout as the content view
		setContentView(framelayout);
	}
	
    public Cocos2dxGLSurfaceView onCreateView() {
    	return new Cocos2dxGLSurfaceView(this);
    }
    
	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
