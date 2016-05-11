package com.example.dennis.app;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import android.os.Handler;

public class MainActivity extends AppCompatActivity {

    private EditText tempTextView;
    private Handler mHandler = new Handler();
    private long startTime;
    private long elapsedTime;
    private final int REFRESH_RATE = 100;
    private String  minutes, seconds;
    private long secs, mins;
    private int working_periods = 0;
    private int resting_periods = 0;
    private boolean working = true;
    private boolean stopped = false;
    private long work_session,break_session,lbreak_session,working_sessions;
    public static final String MyPREFERENCE = "MyPref" ;
    public static final String setc="settings called";
    static SharedPreferences sharedpreferences;
    static boolean set_called=false;
    private Runnable startTimer = new Runnable() {
        public void run() {
            elapsedTime = System.currentTimeMillis() - startTime;
            checkMode();
            updateTimer(elapsedTime);
            mHandler.postDelayed(this, REFRESH_RATE);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        checkScreenDensity();

        sharedpreferences = getSharedPreferences(MyPREFERENCE, Context.MODE_PRIVATE);
        set_called=Boolean.parseBoolean(sharedpreferences.getString(setc,"false"));
    }

    public void stop_pressed(View view) {
        hideButtons();
        mHandler.removeCallbacks(startTimer);
        stopped = true;
    }

    public void skip_pressed(View view) {
        hideButtons();
        mHandler.removeCallbacks(startTimer);
        stopped = false;
        if (working == true){
            working_periods+=1;
        }
        else {
            resting_periods+=1;
        }
        working = !working;
        ((TextView) findViewById(R.id.timer)).setText("00:00");
    }

    public void start_pressed(View view) {
        showButtons();
        if (set_called == false) {
            work_session = 24;
            break_session = 5;
            lbreak_session = 15;
            work_session = 4;
        } else {
            work_session=SettingsActivity.getWsession_duration();
            break_session = SettingsActivity.getBreak_duration();
            lbreak_session = SettingsActivity.getLbreak_duration();
            working_sessions = SettingsActivity.getWork_sessions();
        }
        if (stopped) {
            startTime = System.currentTimeMillis() - elapsedTime;
        } else {
            startTime = System.currentTimeMillis();
        }
        mHandler.removeCallbacks(startTimer);
        mHandler.postDelayed(startTimer, 0);
    }

    private void showButtons() {
        ((Button) findViewById(R.id.bstart)).setVisibility(View.GONE);
        ((Button) findViewById(R.id.bskip)).setVisibility(View.VISIBLE);
        ((Button) findViewById(R.id.bstop)).setVisibility(View.VISIBLE);
    }

    private void hideButtons() {
        ((Button) findViewById(R.id.bstart)).setVisibility(View.VISIBLE);
        ((Button) findViewById(R.id.bskip)).setVisibility(View.GONE);
        ((Button) findViewById(R.id.bstop)).setVisibility(View.GONE);
    }

    private void updateTimer(float time) {
        secs = (long) (time / 1000);
        mins = (long) ((time / 1000) / 60);
        secs = secs % 60;
        seconds = String.valueOf(secs);
        if (secs == 0) {
            seconds = "00";
        }
        if (secs < 10 && secs > 0) {
            seconds = "0" + seconds;
        }
        mins = mins % 60;
        minutes = String.valueOf(mins);
        if (mins == 0) {
            minutes = "00";
        }
        if (mins < 10 && mins > 0) {
            minutes = "0" + minutes;
        }

        ((TextView) findViewById(R.id.timer)).setText( minutes + ":" + seconds);
    }
    private void checkScreenDensity() {
        tempTextView = (EditText) findViewById(R.id.taskname);
        switch (getResources().getDisplayMetrics().densityDpi) {
            case DisplayMetrics.DENSITY_LOW:
                tempTextView.setVisibility(View.GONE);
                break;
            case DisplayMetrics.DENSITY_MEDIUM:
                tempTextView.setVisibility(View.GONE);
                break;
            case DisplayMetrics.DENSITY_HIGH:
                tempTextView.setVisibility(View.VISIBLE);
                break;
        }
    }

    private void checkMode(){

        if (mins == (work_session-1) && secs == 59 && working == true && working_periods<working_sessions) {
            working_periods += 1;
            working = false;
            startTime = System.currentTimeMillis();
            mHandler.removeCallbacks(startTimer);
            mHandler.postDelayed(startTimer, 0);
        }
        if (mins == (break_session-1) && secs == 59 && working == false && resting_periods<(working_sessions-1)) {
            resting_periods += 1;
            working = true;
            startTime = System.currentTimeMillis();
            mHandler.removeCallbacks(startTimer);
            mHandler.postDelayed(startTimer, 0);
        }
        if (mins == (lbreak_session-1) && secs == 59 && working == false && working_periods>=working_sessions) {
            working_periods = 0;
            resting_periods = 0;
            working = false;
            startTime = System.currentTimeMillis();
            mHandler.removeCallbacks(startTimer);
            mHandler.postDelayed(startTimer, 0);
        }
    }

    public void getSetting(View view) {
        SharedPreferences.Editor editor = sharedpreferences.edit();
        editor.putString(setc,"true");
        editor.commit();
        Intent intent = new Intent(MainActivity.this, SettingsActivity.class);
        startActivity(intent);
    }
}
