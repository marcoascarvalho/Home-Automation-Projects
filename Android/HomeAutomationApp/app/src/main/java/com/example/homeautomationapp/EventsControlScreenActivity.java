package com.example.homeautomationapp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class EventsControlScreenActivity extends Activity {

		@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_events_control_screen);
		
		//Button Event Register Creation
		Button launchEventRegisterActivityButton = (Button) findViewById(R.id.buttonRegister);
		launchEventRegisterActivityButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// Launch Activity Event Register
				Intent intent = new Intent(EventsControlScreenActivity.this, EventRegisterScreenActivity.class);

				// Launch the Activity using the intent
				startActivity(intent);
						
			}
		});
		
		//Button Event Visualization Creation
		Button launchEventVisualizationActivityButton = (Button) findViewById(R.id.buttonVisualization);
		launchEventVisualizationActivityButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// Launch Activity Event Register
				Intent intent = new Intent(EventsControlScreenActivity.this, EventVisualizationScreenActivity.class);

				// Launch the Activity using the intent
				startActivity(intent);
								
			}
		});
		
		
		
	}
		
}