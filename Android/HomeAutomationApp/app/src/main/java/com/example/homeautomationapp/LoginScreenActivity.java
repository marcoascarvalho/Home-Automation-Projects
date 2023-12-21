package com.example.homeautomationapp;

import android.app.Activity;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.communication.CommunicationManager;

import java.util.concurrent.ExecutionException;

public class LoginScreenActivity extends Activity {

	//private static final Uri CONTACTS_CONTENT_URI = ContactsContract.Contacts.CONTENT_URI;

	private EditText editTextUserName;
	private EditText editTextHousePassword;
	private TextView textViewMessage;

		@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_login_screen);

		editTextUserName = (EditText) findViewById(R.id.editTextUserName);
		editTextHousePassword = (EditText) findViewById(R.id.editTextHousePassword);
		textViewMessage	= (TextView) findViewById(R.id.textViewMessage);



			final Button SignInButton = (Button) findViewById(R.id.buttonSignIn);
			SignInButton.setOnClickListener(new View.OnClickListener() {

				@Override
				public void onClick(View v) {

				String username = editTextUserName.getText().toString();
				String password = editTextHousePassword.getText().toString();


				if(username.equals("Admin") & password.equals("Admin")){

					// Launch Activity
					Intent intent = new Intent(LoginScreenActivity.this, NetworkRegisterScreenActivity.class);

					// Launch the Activity using the intent
					startActivity(intent);

				}
				else{
					//Toast.makeText(getApplicationContext(), "Wrong Username or Password. Try Again.", Toast.LENGTH_SHORT).show();

					//textViewMessage.setTextColor(Color.RED);
					textViewMessage.setTextColor(getResources().getColor(R.color.red_color));
					textViewMessage.setText(getString(R.string.wrong_username_password_string));
					editTextUserName.setText("");
					editTextHousePassword.setText("");
				}
				}
			});

			/*
			final Button CancelButton = (Button) findViewById(R.id.buttonCancel);
			CancelButton.setOnClickListener(new View.OnClickListener() {

				@Override
				public void onClick(View v) {
					//CommunicationManager.getInstance().sendData(editTextHouseName.getText().toString().toCharArray());

					//Log.i(TAG, "Entered cancelButton.OnClickListener.onClick()");

					// Reset some fields
					editTextUserName.setText("");
					editTextHousePassword.setText("");

					// Indicate result and finish
					finish();

				}
			}); */

	}

	@Override
	public void onResume() {
		super.onResume();

		editTextUserName.setText("");
		editTextHousePassword.setText("");
		textViewMessage.setTextColor(getResources().getColor(R.color.white_color));
		textViewMessage.setText(getString(R.string.start_message_string));

	}
		
}

