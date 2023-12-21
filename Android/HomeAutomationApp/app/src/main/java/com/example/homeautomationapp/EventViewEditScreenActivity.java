package com.example.homeautomationapp;

import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.database.Cursor;
//import android.widget.CursorAdapter;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.SimpleCursorAdapter;

import com.example.database.EventDatabaseOpenHelper;

public class EventViewEditScreenActivity extends ListActivity {

	private EventDatabaseOpenHelper mDbEventHelper;
	private SimpleCursorAdapter mAdapter;

	//private CursorAdapter mAdapter;

	private Cursor mDatacursor;

	private String mdataId = "NULL";
	private String mdataUnitName, mdataDateOn, mdataDateOff;

	private Button mUpdateDatabutton;
	private Button meraseButton;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.listactivity_event_visualization_screen);

		mUpdateDatabutton = (Button) findViewById(R.id.UpdateDatabutton);
		meraseButton = (Button) findViewById(R.id.EraseDatabutton);

		mUpdateDatabutton.setEnabled(false);
		meraseButton.setEnabled(false);

		// Create a new EventDatabaseOpenHelper for the events
		mDbEventHelper = new EventDatabaseOpenHelper(this);

		// Create a cursor
		Cursor cur = readInsertedEvents();

		mAdapter = new SimpleCursorAdapter(this, R.layout.listactivity_eventlist_layout, cur,
				EventDatabaseOpenHelper.columns, new int[] { R.id._id, R.id.unit_name, R.id.date_on, R.id.date_off },
				0); //suported only in sdk version 11 3.0.

		setListAdapter(mAdapter);

		Button eraseAllButton = (Button) findViewById(R.id.EraseAllDatabutton);
		eraseAllButton.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {

				// Erase all data
				clearAll();

				// Redisplay data
				mAdapter.getCursor().requery();
				mAdapter.notifyDataSetChanged();
			}
		});

		getListView().setOnItemClickListener(new AdapterView.OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
									int position, long id) {

				//getListView().setChoiceMode(getListView().CHOICE_MODE_SINGLE);
				getListView().setSelection(position);
				//getListView().setItemChecked(position,true);
				//view.setSelected(true);

				mDatacursor = mAdapter.getCursor();
				Log.i("datacursor.EventName1", mDatacursor.getString(1));

				mdataId = mDatacursor.getString(0); // _id
				mdataUnitName = mDatacursor.getString(1); // event_name
				mdataDateOn = mDatacursor.getString(2); // date_ON
				mdataDateOff = mDatacursor.getString(3); // date_OFF

				for (int i=0; i< getListView().getChildCount(); i++)
				{
					View v = getListView().getChildAt(i);
					v.setBackgroundColor(Color.TRANSPARENT);
				}

				//view.setBackgroundResource(R.drawable.my_selector);

				//view.setBackgroundColor(Color.LTGRAY);
				//getListView().setBackgroundColor(ColorDrawable.);

				//getListView().setBackgroundColor(Color.LTGRAY);
				view.setBackgroundColor(Color.GREEN);
				//view.invalidate();

				//Log.i("datacursor.UnitName2", mDatacursor.getString(1));

				mUpdateDatabutton.setEnabled(true);
				meraseButton.setEnabled(true);

			}
		});


		mUpdateDatabutton.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {

				Intent data = new Intent();

				checkNullData();

				data.putExtra("EventDataId", mdataId);
				data.putExtra("EventDataUnitName", mdataUnitName);
				data.putExtra("EventDataDateOn", mdataDateOn);
				data.putExtra("EventDataDateOff", mdataDateOff);

				Log.i("EventDataId", mdataId);
				Log.i("EventDataUnitName", mdataUnitName);
				Log.i("EventDataDateOn", mdataDateOn);
				Log.i("EventDataDateOff", mdataDateOff);

				// return data Intent and finish
				setResult(RESULT_OK, data);
				finish();


			}
		});


		meraseButton.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {

				//String dataId = mDatacursor.getString(0); // _id

				checkNullData();

				// Erase data
				mDbEventHelper.getWritableDatabase().delete(EventDatabaseOpenHelper.TABLE_NAME, EventDatabaseOpenHelper._ID + "=?", new String[] { mdataId });

				// Redisplay data
				mAdapter.getCursor().requery();
				mAdapter.notifyDataSetChanged();
			}
		});


	}

	private Cursor readInsertedEvents() {

		Log.i("Debug", "readInsertedEvents()");

		return mDbEventHelper.getWritableDatabase().query(EventDatabaseOpenHelper.TABLE_NAME,
				EventDatabaseOpenHelper.columns, null, new String[] {}, null, null,
				null);



	}

	// Delete all records
	private void clearAll() {

		mDbEventHelper.getWritableDatabase().delete(EventDatabaseOpenHelper.TABLE_NAME, null, null);

	}

	private void checkNullData() {

		if(mdataId.equals("NULL")){
			mDatacursor = mAdapter.getCursor();

			mdataId = mDatacursor.getString(0); // _id
			mdataUnitName = mDatacursor.getString(1); // event_name
			mdataDateOn = mDatacursor.getString(2); // date_ON
			mdataDateOff = mDatacursor.getString(3); // date_OFF

		}
	}

	public void onBackPressed()
	{
		// this call would exit the activity
		super.onBackPressed();

		setResult(RESULT_CANCELED, null);

		finish();
	}

	// Close database
	@Override
	protected void onDestroy() {

		mDbEventHelper.getWritableDatabase().close();
		//mDbHelper.deleteDatabase();

		super.onDestroy();

	}

}