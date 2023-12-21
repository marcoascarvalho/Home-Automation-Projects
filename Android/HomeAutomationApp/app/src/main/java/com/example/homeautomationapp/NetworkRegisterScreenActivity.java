package com.example.homeautomationapp;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.example.communication.CommunicationManager;
import com.example.communication.OnDataListener;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.ExecutionException;

public class NetworkRegisterScreenActivity extends Activity implements OnDataListener{

    private EditText editTextViewHouseIP;
    private EditText editTextViewHousePort;
    private EditText editTextHouseName;
    private EditText editTextUserName;
    private EditText editTextHousePassword;
    private EditText textViewData;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_network_register_screen);

        editTextViewHouseIP = (EditText) findViewById(R.id.editTextViewHouseIP);
        editTextViewHousePort = (EditText) findViewById(R.id.editTextHousePort);
        editTextHouseName = (EditText) findViewById(R.id.editTextHouseName);
        editTextUserName = (EditText) findViewById(R.id.editTextUserName);
        editTextHousePassword = (EditText) findViewById(R.id.editTextHousePassword);
        textViewData = (EditText) findViewById(R.id.textViewData);

        final Button OKButton = (Button) findViewById(R.id.buttonOK);
        OKButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                String ip = editTextViewHouseIP.getText().toString();
                String port = editTextViewHousePort.getText().toString();
                String username = editTextUserName.getText().toString();
                String password = editTextHousePassword.getText().toString();

                try {
                    if(username == "Admin" & password == "Admin"){
                        boolean connected = CommunicationManager.getInstance().execute(new String[]{ip, port}).get();
                        Toast.makeText(getApplicationContext(), "Connected: " + connected, Toast.LENGTH_SHORT).show();

                        if(connected)
                        {
                            CommunicationManager.getInstance().receiveData();
                        }
                    }
                    else{
                        Toast.makeText(getApplicationContext(), "Wrong Username or Password. Try Again.", Toast.LENGTH_SHORT).show();
                    }




                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (ExecutionException e) {
                    e.printStackTrace();
                }


            }
        });

        final Button CancelButton = (Button) findViewById(R.id.buttonCancel);
        CancelButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                //CommunicationManager.getInstance().sendData(editTextHouseName.getText().toString().toCharArray());

                //Log.i(TAG, "Entered cancelButton.OnClickListener.onClick()");

                // Reset some fields
                editTextHouseName.setText("");
                editTextUserName.setText("");
                editTextHousePassword.setText("");

                // Indicate result and finish
                finish();

            }
        });

        CommunicationManager.getInstance().addDataListener(this);
    }

    @Override
    public void onDataReceived(final byte[] data, final int size) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String value = new String(data);
                textViewData.setText(value.substring(0, size));
            }
        });
    }
}
