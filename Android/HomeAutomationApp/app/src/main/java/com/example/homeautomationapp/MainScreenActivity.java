package com.example.homeautomationapp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.util.Log;
import android.widget.Toast;

public class MainScreenActivity extends Activity {

	private static final String TAG = "HomeAuto";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main_screen);
		
		//Button Direct Control Creation
		Button launchIndividualControlActivityButton = (Button) findViewById(R.id.buttonDirectControl);
		launchIndividualControlActivityButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

				Log.i(TAG, "Open Direct Control Screen");

				Toast.makeText(MainScreenActivity.this, "Open Direct Control Screen", Toast.LENGTH_SHORT).show();

				// Launch Activity Individual Control
				Intent intent = new Intent(MainScreenActivity.this, IndividualControlScreenActivity.class);

				// Launch the Activity using the intent
				startActivity(intent);
				
			}
		});
		
		//Button Events Control Creation
		Button launchEventsControlActivityButton = (Button) findViewById(R.id.buttonEvents);
		launchEventsControlActivityButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// Launch Activity Individual Control
				Intent intent = new Intent(MainScreenActivity.this, EventsControlScreenActivity.class);

				// Launch the Activity using the intent
				startActivity(intent);
						
			}
		});
		
		//Button General Configurations Creation
		Button launchGeneralConfigurationActivityButton = (Button) findViewById(R.id.buttonConfigurations);
		launchGeneralConfigurationActivityButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// Launch Activity Individual Control
				Intent intent = new Intent(MainScreenActivity.this, GeneralConfigurationScreenActivity.class);

				// Launch the Activity using the intent
				startActivity(intent);
								
			}
		});
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main_screen, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
