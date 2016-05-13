package com.example.dennis.app;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.InputFilter;
import android.view.View;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

/**
 * Created by Dennis on 11-May-16.
 */
public class SettingsActivity extends AppCompatActivity {

    private EditText wsession_duration,break_duration,lbreak_duration,work_sessions;
    private CheckBox disable;
    public static final String MyPREFERENCES = "MyPrefs" ;
    public static final String wd="work session duration",bd="break duration",lbd="long break duration",ws="work sessions",dn="notifications";
    static SharedPreferences sharedpreferences;
    static String wduration,bduration,lbduration,wsessions;
    private boolean ans;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        disable=(CheckBox)findViewById(R.id.disable_notifications);
        wsession_duration=(EditText)findViewById(R.id.wsession_duration);
        break_duration=(EditText)findViewById(R.id.break_duration);
        lbreak_duration=(EditText)findViewById(R.id.lbreak_duration);
        work_sessions=(EditText)findViewById(R.id.work_sessions);

        wsession_duration.setFilters(new InputFilter[]{ new InputFilterMinMax("1", "45")});
        break_duration.setFilters(new InputFilter[]{ new InputFilterMinMax("1", "15")});
        lbreak_duration.setFilters(new InputFilter[]{ new InputFilterMinMax("1", "45")});
        work_sessions.setFilters(new InputFilter[]{ new InputFilterMinMax("1", "10")});

        sharedpreferences = getSharedPreferences(MyPREFERENCES, Context.MODE_PRIVATE);
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

    private void SavePreferences(String key, String value){
        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());

        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putString(key, value);
        editor.commit();
    }

    public void save_data(View view) {
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
        SavePreferences(MainActivity.wd, a);
        SavePreferences(MainActivity.bd, b);
        SavePreferences(MainActivity.lbd,c);
        SavePreferences(MainActivity.ws, d);
        SavePreferences(MainActivity.dn, e);
        saving();
        Toast.makeText(SettingsActivity.this, R.string.toast_memoryshort, Toast.LENGTH_SHORT).show();
    }

    public void close_settings(View view) {
        finish();
    }

    public void saving(){
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
    }
}
