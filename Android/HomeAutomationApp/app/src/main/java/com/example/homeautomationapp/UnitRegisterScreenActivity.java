package com.example.homeautomationapp;

import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SimpleCursorAdapter;
import android.widget.Spinner;
import android.widget.Toast;
import android.app.AlertDialog.Builder;

import com.example.database.DatabaseOpenHelper;

public class UnitRegisterScreenActivity extends Activity {

	public DatabaseOpenHelper mDbHelper;
	//private SimpleCursorAdapter mAdapter;

	private static final int DATA_TO_BE_UPDATED = 0;

	private RadioGroup mUnitTypeRadioGroup;
	private RadioButton mUnitTypeLight;
	private RadioButton mUnitTypeAppliance;

	//Unit Spinner
	private Spinner mSpinnerUnit;
	private String[] UnitValues = { "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16" };
	private String UnitValue = "0";

	private EditText mUnitName;
	//private String mUnitType;

	private String reg_ID;

	private Button mInsertButton;
	private Button mUpdateButton;

		@Override
	protected void onCreate(Bundle savedInstanceState)
		{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_house_register_screen);

			mUnitName = (EditText) findViewById(R.id.editTextUnitName);
			mUnitTypeRadioGroup = (RadioGroup) findViewById(R.id.radioGroupDeviceOption);
			mUnitTypeLight = (RadioButton) findViewById(R.id.radioLightOption);
			mUnitTypeAppliance = (RadioButton) findViewById(R.id.radioApplianceOption);

			// Create a new DatabaseHelper
			mDbHelper = new DatabaseOpenHelper(this);

			// start with an empty database
			//clearAll();

			// Code to show unit spinner values
			mSpinnerUnit = (Spinner) findViewById(R.id.spinnerUnitId);
			ArrayAdapter<String> adapterUnit = new ArrayAdapter<String>(this,
					android.R.layout.simple_spinner_dropdown_item, UnitValues);


			mSpinnerUnit.setAdapter(adapterUnit);
			mSpinnerUnit.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
				public void onItemSelected(AdapterView<?> arg0,
										   View arg1, int arg2, long arg3) {
					int index = mSpinnerUnit.getSelectedItemPosition();
					UnitValue = UnitValues[index];
					Toast.makeText(getBaseContext(), "UnitValue = " + UnitValue, Toast.LENGTH_SHORT).show();
				}

				public void onNothingSelected(AdapterView<?> arg0) {}
			});


			mUpdateButton = (Button) findViewById(R.id.Updatebutton);
			mUpdateButton.setEnabled(false);
			mUpdateButton.setOnClickListener(new View.OnClickListener() {

				@Override
				public void onClick(View v) {

					updateUnitRegister();



				}
			});


			mInsertButton = (Button) findViewById(R.id.Insertbutton);
			mInsertButton.setOnClickListener(new View.OnClickListener() {

				@Override
				public void onClick(View v) {

					insertUnitRegister();


				}
			});


			//Button View Edit Creation
			Button launchUnitViewEditActivityButton = (Button) findViewById(R.id.ViewEditbutton);
			launchUnitViewEditActivityButton.setOnClickListener(new View.OnClickListener() {

				@Override
				public void onClick(View v) {

					Toast.makeText(UnitRegisterScreenActivity.this, "Open Unit View Edit Screen", Toast.LENGTH_SHORT).show();

					// Launch Activity View Edit Unit
					Intent intent = new Intent(UnitRegisterScreenActivity.this, UnitViewEditScreenActivity.class);
					startActivityForResult(intent, DATA_TO_BE_UPDATED);

				}
			});


		}

	/*
	@Override
	protected void onStart() {
		super.onStart();

		mUpdateButton.setEnabled(false);
	}*/

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {

		Log.i("debug","Entered onActivityResult()");


		if (requestCode == DATA_TO_BE_UPDATED) {
			switch (resultCode) {
				case RESULT_OK:
					reg_ID = data.getStringExtra("HouseDataId");
					mUnitName.setText(data.getStringExtra("HouseDataUnitName"));
					String SpinnerUnitValue = data.getStringExtra("HouseDataUnitId");
					for(int i = 0; i < mSpinnerUnit.getCount(); i++){
						if(mSpinnerUnit.getItemAtPosition(i).toString().equals(SpinnerUnitValue)){
							mSpinnerUnit.setSelection(i);
							break;
						}
					}
					String UnitTypeStatus = data.getStringExtra("HouseDataUnitType");
					setUnitType(UnitTypeStatus);
					mInsertButton.setEnabled(false);
					mUpdateButton.setEnabled(true);

					break;
				case RESULT_CANCELED:
					mInsertButton.setEnabled(true);
					mUpdateButton.setEnabled(false);
					mUnitName.setText("");
					mSpinnerUnit.setSelection(0);
					setUnitType("Light");
					break;
			}
		}
	}


	// Insert several Unit records
	private void insertUnitRegister() {

		ContentValues values = new ContentValues();

		if(mUnitName.getText().toString().trim().length()==0||
		UnitValue.equals("0"))
		{
			showMessage("Error", "Please enter the Unit name or change Unit value");
			return;
		}


		values.put(DatabaseOpenHelper.UNIT_NAME, mUnitName.getText().toString());
		values.put(DatabaseOpenHelper.UNIT_ID, UnitValue);
		values.put(DatabaseOpenHelper.UNIT_TYPE, getUnitType());
		mDbHelper.getWritableDatabase().insert(DatabaseOpenHelper.TABLE_NAME, null, values);

		Log.i("Debug", "insertUnitRegister");
		values.clear();

		showMessage("Success", "Unit: " + mUnitName.getText().toString() + " inserted with success");

	}

	// update  Unit records

	private void updateUnitRegister() {

		ContentValues values = new ContentValues();

		if(mUnitName.getText().toString().trim().length()==0||
				UnitValue.equals("0"))
		{
			showMessage("Error", "Please enter the Unit name or change Unit value");
			return;
		}

		values.put(DatabaseOpenHelper.UNIT_NAME, mUnitName.getText().toString());
		values.put(DatabaseOpenHelper.UNIT_ID, UnitValue);
		values.put(DatabaseOpenHelper.UNIT_TYPE, getUnitType());
		mDbHelper.getWritableDatabase().update(DatabaseOpenHelper.TABLE_NAME, values,
				DatabaseOpenHelper._ID + "=?",
				new String[] { reg_ID } );



		Log.i("Debug", "update  Unit records");
		values.clear();

		showMessage("Success", "Unit: " + mUnitName.getText().toString() + " updated with success");

	}

	private void setUnitType( String UnitType) {

		if(UnitType.equals("Light")){
			mUnitTypeLight.setChecked(true);
		}
		else{
			if(UnitType.equals("Appliance")) {
				mUnitTypeAppliance.setChecked(true);
			}
		}

	}

	private String getUnitType() {

		switch (mUnitTypeRadioGroup.getCheckedRadioButtonId()) {
			case R.id.radioLightOption: {
				return "Light";
			}
			default: {
				return "Appliance";
			}
		}
	}

	public void showMessage(String title,String message)
	{
		Builder builder=new Builder(this);
		builder.setCancelable(true);
		builder.setTitle(title);
		builder.setMessage(message);
		builder.show();
	}


	// Close database
	@Override
	protected void onDestroy() {

		mDbHelper.getWritableDatabase().close();
		//mDbHelper.deleteDatabase();

		super.onDestroy();

	}

}