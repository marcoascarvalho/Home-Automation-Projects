package com.example.homeautomationapp;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Spinner;
import android.widget.Toast;

import com.example.communication.CommunicationManager;

public class DirectControlScreenActivity extends Activity {

    private static final String TAG = "HomeAutoAPP";

    private static final char HA_SET_UNIT = '0';
    private static final char ON_cmd = '3';
    private static final char OFF_cmd = '4';
    private static final char HA_LIGHT_DIM = '2';
    private static final char Dimmer_cmd = '5';


    private RadioGroup mCommandRadioGroup;
    private RadioButton mDimmerButton;
    private EditText mDimmerPercent;

    //House Spinner
    private Spinner mSpinnerHouse;
    private String[] HouseValues = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P" };
    public String HouseValue = "A";

    //Unit Spinner
    private Spinner mSpinnerUnit;
    private String[] UnitValues = { "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16" };
    private String[] UnitValuesChar = { "1","2","3","4","5","6","7","8","9",":",";","<","=",">","?" };
    private String UnitValue = "1";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_direct_control_screen);

        mSpinnerHouse = (Spinner) findViewById(R.id.spinnerHouse);
        //ArrayAdapter<String> adapterHouse = new ArrayAdapter<String>(this,
        //        android.R.layout.simple_spinner_item, HouseValues);
        ArrayAdapter<String> adapterHouse = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_dropdown_item, HouseValues);

        mSpinnerHouse.setAdapter(adapterHouse);
        mSpinnerHouse.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> arg0,
                                       View arg1, int arg2, long arg3) {
                int index = mSpinnerHouse.getSelectedItemPosition();
                HouseValue = HouseValues[index];
                Toast.makeText(getBaseContext(), "HouseValue = " + HouseValue, Toast.LENGTH_SHORT).show();
            }

            public void onNothingSelected(AdapterView<?> arg0) {}
        });

        mSpinnerUnit = (Spinner) findViewById(R.id.spinnerUnit);
        //ArrayAdapter<String> adapterUnit = new ArrayAdapter<String>(this,
        //        android.R.layout.simple_spinner_item, UnitValues);
        ArrayAdapter<String> adapterUnit = new ArrayAdapter<String>(this,
                  android.R.layout.simple_spinner_dropdown_item, UnitValues);


        mSpinnerUnit.setAdapter(adapterUnit);
        mSpinnerUnit.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> arg0,
                                       View arg1, int arg2, long arg3) {
                int index = mSpinnerUnit.getSelectedItemPosition();
                UnitValue = UnitValuesChar[index];
                Toast.makeText(getBaseContext(), "UnitValue = " + UnitValue, Toast.LENGTH_SHORT).show();
            }

            public void onNothingSelected(AdapterView<?> arg0) {}
        });

        mCommandRadioGroup = (RadioGroup) findViewById(R.id.radioGroup1);

        mDimmerPercent = (EditText) findViewById(R.id.editTextDimmerPercent);
        mDimmerPercent.setEnabled(false);

        mCommandRadioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener(){
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {

                RadioButton rdb = (RadioButton) findViewById(checkedId);

                if (rdb.getText().equals("Dimmer")) {
                    mDimmerPercent.setEnabled(true);
                } else {
                    mDimmerPercent.setEnabled(false);
                }
            }
        });


        final Button cancelButton = (Button) findViewById(R.id.buttonDevicePointCancel);
        cancelButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Log.i(TAG, "Entered cancelButton.OnClickListener.onClick()");

                // Indicate result and finish
                finish();


            }
        });

        final Button OkButton = (Button) findViewById(R.id.buttonDevicePointOK);
        OkButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                char Devicecmd = getDeviceCommand();
                String DevicePayload;

                if (Devicecmd == Dimmer_cmd) {

                    char DimmerPercentageChar = (char) getDimmerPercentage();

                    DevicePayload = HA_LIGHT_DIM + HouseValue + UnitValue + DimmerPercentageChar;
                }
                else {
                    DevicePayload = HA_SET_UNIT + HouseValue + UnitValue + Devicecmd;
                }


                Toast.makeText(getApplicationContext(), "DevicePayload: " + DevicePayload, Toast.LENGTH_SHORT).show();

                CommunicationManager.getInstance().sendData(DevicePayload.toCharArray());
            }
        });
    }

    private char getDeviceCommand() {

        switch (mCommandRadioGroup.getCheckedRadioButtonId()) {
            case R.id.radioDevicePointOn: {
                return ON_cmd;
            }
            case R.id.radioDevicePointOff: {
                return OFF_cmd;
            }
            case R.id.radioLightPointDimmer: {
                return Dimmer_cmd;
            }
            default: {
                return OFF_cmd;
            }
        }
    }

    private int getDimmerPercentage() {
        if (mDimmerPercent.getText().toString().equals("")){
            return 99;
        }
        else {
            return Integer.parseInt(mDimmerPercent.getText().toString());
        }
    }

}
