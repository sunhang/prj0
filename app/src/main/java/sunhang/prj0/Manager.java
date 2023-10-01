package sunhang.prj0;

import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;
import android.widget.ImageView;
import android.widget.Toast;

import java.lang.ref.WeakReference;


public class Manager {
    private Handler mHandle = new Handler(Looper.getMainLooper());
    private ImageView mIv;

    private WeakReference<Thread> mWeakRefThread;

    void bindImageView(ImageView iv) {
        mIv = iv;
    }

    public void handleYu12Data(byte[] data, int w, int h) {
        final Bitmap bm = pToBitmap(data, w, h, true);
        mHandle.post(new Runnable() {
            @Override
            public void run() {
                mIv.setImageBitmap(bm);
            }
        });
    }

    public native void nativeWork();

    public void start() {
        Thread t = mWeakRefThread != null ? mWeakRefThread.get() : null;
        if (t != null && t.isAlive()) {
            Toast.makeText(mIv.getContext(), "已经存在正在运行的进程", Toast.LENGTH_SHORT).show();
            return;
        }

        t = new Thread() {
            @Override
            public void run() {
                nativeWork();
            }
        };
        mWeakRefThread =  new WeakReference<>(t);
        t.start();
    }

    private static Bitmap pToBitmap(byte[] data, int w, int h, boolean uv) {
        int plane = w * h;
        int[] colors = new int[plane];
        int off = plane >> 2;
        int yPos = 0, uPos = plane + (uv ? 0 : off), vPos = plane + (uv ? off : 0);
        for(int j = 0; j < h; j++) {
            for(int i = 0; i < w; i++) {
                // YUV byte to RGB int
                final int y1 = data[yPos] & 0xff;
                final int u = (data[uPos] & 0xff) - 128;
                final int v = (data[vPos] & 0xff) - 128;
                final int y1192 = 1192 * y1;
                int r = (y1192 + 1634 * v);
                int g = (y1192 - 833 * v - 400 * u);
                int b = (y1192 + 2066 * u);

                r = (r < 0) ? 0 : ((r > 262143) ? 262143 : r);
                g = (g < 0) ? 0 : ((g > 262143) ? 262143 : g);
                b = (b < 0) ? 0 : ((b > 262143) ? 262143 : b);
                colors[yPos] = ((r << 6) & 0xff0000) |
                        ((g >> 2) & 0xff00) |
                        ((b >> 10) & 0xff);

                if((yPos++ & 1) == 1) {
                    uPos++;
                    vPos++;
                }
            }
            if((j & 1) == 0) {
                uPos -= (w >> 1);
                vPos -= (w >> 1);
            }
        }
        return Bitmap.createBitmap(colors, w, h, Bitmap.Config.RGB_565);
    }
}
