package com.example.homeautomationapp;

import android.app.Activity;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class GeneralConfigurationScreenActivity extends Activity {

	//private static final Uri CONTACTS_CONTENT_URI = ContactsContract.Contacts.CONTENT_URI;

		@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_general_configuration_screen);
		
		//Button House Register Creation
		Button launchHouseRegisterActivityButton = (Button) findViewById(R.id.buttonHouse);
		launchHouseRegisterActivityButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// Launch Activity Event Register
				Intent intent = new Intent(GeneralConfigurationScreenActivity.this, HouseRegisterScreenActivity.class);

				// Launch the Activity using the intent
				startActivity(intent);
				//startActivityForResult(intent, PICK_CONTACT_REQUEST);
								
			}
		});
				
		//Button Network Register Creation
		Button launchNetworkRegisterActivityButton = (Button) findViewById(R.id.buttonNetwork);
		launchNetworkRegisterActivityButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// Launch Activity Event Register
				Intent intent = new Intent(GeneralConfigurationScreenActivity.this, NetworkRegisterScreenActivity.class);

				// Launch the Activity using the intent
				startActivity(intent);
										
			}
		});
		
	}
		
}

	//@Override
	//protected void onActivityResult(int requestCode, int resultCode, Intent data) {

		// Ensure that this call is the result of a successful PICK_CONTACT_REQUEST request
		/*if (resultCode == Activity.RESULT_OK
				&& requestCode == PICK_CONTACT_REQUEST) {

			// These details are covered in the lesson on ContentProviders
			ContentResolver cr = getContentResolver();
			Cursor cursor = cr.query(data.getData(), null, null, null, null);

			if (null != cursor && cursor.moveToFirst()) {
				String id = cursor
						.getString(cursor.getColumnIndex(CONTACTS_ID));
				String where = DATA_CONTACT_ID + " = ? AND " + DATA_MIMETYPE
						+ " = ?";
				String[] whereParameters = new String[] { id,
						STRUCTURED_POSTAL_CONTENT_ITEM_TYPE };
				Cursor addrCur = cr.query(DATA_CONTENT_URI, null, where,
						whereParameters, null);

				if (null != addrCur && addrCur.moveToFirst()) {
					String formattedAddress = addrCur
							.getString(addrCur
									.getColumnIndex(STRUCTURED_POSTAL_FORMATTED_ADDRESS));

					if (null != formattedAddress) {

						// Process text for network transmission
						formattedAddress = formattedAddress.replace(' ', '+');

						// Create Intent object for starting Google Maps application
						Intent geoIntent = new Intent(
								android.content.Intent.ACTION_VIEW,
								Uri.parse("geo:0,0?q=" + formattedAddress));

						// Use the Intent to start Google Maps application using Activity.startActivity()
						startActivity(geoIntent);
					}
				}
				if (null != addrCur)
					addrCur.close();
			}
			if (null != cursor)
				cursor.close();
		}
	}*/