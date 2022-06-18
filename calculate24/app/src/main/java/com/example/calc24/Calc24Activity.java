package com.example.calc24;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.calc24.databinding.ActivityScrollingBinding;

import java.util.Random;

public class Calc24Activity extends AppCompatActivity {

    static {
        System.loadLibrary("calc24");
    }

    private ActivityScrollingBinding binding;
    private int numIndex = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityScrollingBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        Toolbar toolbar = binding.toolbar;
        setSupportActionBar(toolbar);
        toolbar.setTitle(getTitle());
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_scrolling, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    public void NumOnClick(View view) {
        if (numIndex >= 4) {
            Toast.makeText(this, "请先删除或清空已选数字", Toast.LENGTH_SHORT).show();
            return;
        }
        TextView[] numViews = {binding.num1, binding.num2, binding.num3, binding.num4};
        numViews[numIndex++].setText(((Button)view).getText());
    }

    public void RandomNumOnClick(View view) {
        if (numIndex >= 4) {
            numIndex = 0;
        }
        TextView[] numViews = {binding.num1, binding.num2, binding.num3, binding.num4};
        Random r = new Random();
        while (numIndex < 4) {
            numViews[numIndex++].setText(String.valueOf(r.nextInt(13) + 1));
        }
    }

    public void CommandOnClick(View view) {
        TextView[] numViews = {binding.num1, binding.num2, binding.num3, binding.num4};
        TextView outputView = findViewById(R.id.output);
        if (view == binding.buttonDelete) {
            if (numIndex > 4) numIndex = 4;
            if (numIndex > 0) {
                numViews[--numIndex].setText("");
                outputView.setText("");
            }
        } else if (view == binding.buttonClear) {
            for (TextView v : numViews) {
                v.setText("");
            }
            outputView.setText("");
            numIndex = 0;
        } else if (view == binding.buttonCheck || view == binding.buttonCalc) {
            if (numIndex < 4) {
                Toast.makeText(this, "请先选择4个数字", Toast.LENGTH_SHORT).show();
                return;
            }
            int[] nums = new int[4];
            for (int i = 0; i < 4; i++) {
                nums[i] = Integer.parseInt((String)numViews[i].getText());
            }
            String solutions = calc24(nums);
            if (solutions == null || solutions.length() == 0) {
                outputView.setText("这组数计算24无解。");
            } else {
                if (view == binding.buttonCheck) {
                    outputView.setText("这组数计算24有解。");
                } else {
                    outputView.setText(solutions);
                }
            }
        }
    }

    private native String calc24(int[] nums);
}