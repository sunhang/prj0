package sunhang.prj0;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.WindowCompat;

import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;

import sunhang.prj0.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'prj0' library on application startup.
    static {
        System.loadLibrary("prj0");
    }

    private ActivityMainBinding binding;
    private Manager mManager = new Manager();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (Build.VERSION.SDK_INT >= 21) {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                    WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
        getSupportActionBar().hide();
        WindowCompat.setDecorFitsSystemWindows(getWindow(), false);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        mManager.bindImageView(binding.iv);
        Button btn = binding.btn;
        btn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                mManager.start();
            }
        });
    }
}