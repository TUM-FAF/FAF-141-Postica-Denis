package com.example.dennis.app;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.view.View.OnClickListener;
import android.widget.TextView;
import android.widget.Toast;

/**
 * Created by Dennis on 11-May-16.
 */
public class SettingsActivity extends AppCompatActivity {

    private EditText wsession_duration,break_duration,lbreak_duration,work_sessions;
    private CheckBox disable;
    private Button close;
    private CheckBox d;
    private boolean ans;
    public static final String MyPREFERENCES = "MyPrefs" ;
    public static final String wd="work session duration",bd="break duration",lbd="long break duration",ws="work sessions",dn="notifications";
    static SharedPreferences sharedpreferences;
    static String wduration,bduration,lbduration,wsessions;
    private Intent intent;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        intent = new Intent(SettingsActivity.this, MainActivity.class);

        wsession_duration=(EditText)findViewById(R.id.wsession_duration);
        break_duration=(EditText)findViewById(R.id.break_duration);
        lbreak_duration=(EditText)findViewById(R.id.lbreak_duration);
        work_sessions=(EditText)findViewById(R.id.work_sessions);
        disable=(CheckBox)findViewById(R.id.disable_notifications);
        close=(Button)findViewById(R.id.close_button);

        sharedpreferences = getSharedPreferences(MyPREFERENCES, Context.MODE_PRIVATE);
        wsession_duration=(EditText)findViewById(R.id.wsession_duration);
        break_duration=(EditText)findViewById(R.id.break_duration);
        lbreak_duration=(EditText)findViewById(R.id.lbreak_duration);
        work_sessions=(EditText)findViewById(R.id.work_sessions);
        d = (CheckBox)findViewById(R.id.disable_notifications);
        wduration = sharedpreferences.getString(wd, "25");
        bduration=sharedpreferences.getString(bd, "5");
        lbduration=sharedpreferences.getString(lbd, "15");
        wsessions=sharedpreferences.getString(ws,"4");
        ans=Boolean.parseBoolean(sharedpreferences.getString(dn,"false"));
        if(ans==false){
            disable.setChecked(false);
        }
        else {
            disable.setChecked(true);
        }
        wsession_duration.setText(wduration);
        break_duration.setText(bduration);
        lbreak_duration.setText(lbduration);
        work_sessions.setText(wsessions);

    }

    public static long getWsession_duration(){

        return Long.parseLong(sharedpreferences.getString("work session duration","25"));
    }

    public static long getBreak_duration(){

        return Long.parseLong(sharedpreferences.getString(bd, "5"));
    }

    public static long getLbreak_duration(){

        return Long.parseLong(sharedpreferences.getString(lbd, "15"));
    }

    public static int getWork_sessions(){

        return Integer.parseInt(sharedpreferences.getString(ws, "4"));
    }

    public static boolean getDisable(){

            return Boolean.parseBoolean(sharedpreferences.getString(dn,"false"));
    }

    public void save_data(View view) {
        SharedPreferences.Editor editor = sharedpreferences.edit();
        String a = wsession_duration.getText().toString();
        String b = break_duration.getText().toString();
        String c = lbreak_duration.getText().toString();
        String d = work_sessions.getText().toString();
        String e;
        if (disable.isChecked()){
            e="true";
        }
        else{
            e="false";
        }
        editor.putString(wd, a);
        editor.putString(bd, b);
        editor.putString(lbd,c);
        editor.putString(ws, d);
        editor.putString(dn, e);
        editor.commit();
        Toast.makeText(SettingsActivity.this, R.string.toast_memoryshort, Toast.LENGTH_SHORT).show();
    }

    public void close_settings(View view) {
        startActivity(intent);
    }
}
