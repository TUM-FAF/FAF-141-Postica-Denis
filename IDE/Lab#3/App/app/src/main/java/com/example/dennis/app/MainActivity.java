package com.example.dennis.app;

import android.app.Activity;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.media.RingtoneManager;
import android.net.Uri;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.app.NotificationCompat;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import android.os.Handler;

public class MainActivity extends AppCompatActivity {

    private Handler mHandler = new Handler();
    private long startTime;
    private long elapsedTime;
    private final int REFRESH_RATE = 100;
    private String  minutes, seconds;
    private long secs, mins;
    private int working_periods = 0;
    private int resting_periods = 0;
    private boolean working = true;
    public static boolean notification;
    private boolean stopped = false;
    public static long work_session,break_session,lbreak_session,working_sessions;
    private TextView action;
    private Intent intent;
    public static final String wd="work session duration",bd="break duration",lbd="long break duration",ws="work sessions",dn="notifications";

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

        action = (TextView)findViewById(R.id.taskname);

        intent = new Intent(MainActivity.this, SettingsActivity.class);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        setContentView(R.layout.activity_main);
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
        working = !working;
        ((TextView) findViewById(R.id.timer)).setText("00:00");
    }

    public void start_pressed(View view) {
        LoadPreferences();
        showButtons();
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
        ((Button) findViewById(R.id.bskip)).setVisibility(View.VISIBLE);
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

    private void checkMode(){

        if (mins == (work_session-1) && secs == 59 && working == true && working_periods<working_sessions) {
            working_periods += 1;
            working = false;
            breakNotification();
            action.setText(R.string.thebreak);
            startTime = System.currentTimeMillis();
            mHandler.removeCallbacks(startTimer);
            mHandler.postDelayed(startTimer, 0);
        }
        if (mins == (break_session-1) && secs == 59 && working == false && resting_periods<(working_sessions-1)) {
            resting_periods += 1;
            working = true;
            workNotification();
            action.setText(R.string.working);
            startTime = System.currentTimeMillis();
            mHandler.removeCallbacks(startTimer);
            mHandler.postDelayed(startTimer, 0);
        }
        if(working_periods==working_sessions){
            action.setText(R.string.l_break);
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
        startActivity(intent);
    }

    public void breakNotification(){
        if (notification==true){
            return;
        }
        else {
            Uri sound = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);

            PendingIntent notifyPIntent = PendingIntent.getActivity(getApplicationContext(), 0, new Intent(), 0);

            NotificationCompat.Builder nb = new NotificationCompat.Builder(this);
            nb.setSmallIcon(R.drawable.ic_launcher);
            nb.setSound(sound);
            nb.setContentTitle(getResources().getString(R.string.recreation));
            nb.setContentText(getResources().getString(R.string.r_message));
            nb.setContentIntent(notifyPIntent);

            NotificationManager nm = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
            nm.notify(100, nb.build());
        }
    }

    public void workNotification(){
        if (notification == true){
            return;
        }
        else {
            Uri sound = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);

            PendingIntent notifyPIntent = PendingIntent.getActivity(getApplicationContext(), 0, new Intent(), 0);

            NotificationCompat.Builder nb = new NotificationCompat.Builder(this);
            nb.setSmallIcon(R.drawable.ic_launcher);
            nb.setSound(sound);
            nb.setContentTitle(getResources().getString(R.string.work));
            nb.setContentText(getResources().getString(R.string.w_message));
            nb.setContentIntent(notifyPIntent);

            NotificationManager nm = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
            nm.notify(100, nb.build());
        }
    }

    private void LoadPreferences(){
        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        work_session=Long.parseLong(sharedPreferences.getString(wd, "25"));
        break_session = Long.parseLong(sharedPreferences.getString(bd, "5"));
        lbreak_session = Long.parseLong(sharedPreferences.getString(lbd, "15"));
        working_sessions = Long.parseLong(sharedPreferences.getString(ws,"4"));
        notification = Boolean.parseBoolean(sharedPreferences.getString(dn,"false"));
    }

    public void close_app(View view) {
        finish();
    }
}

