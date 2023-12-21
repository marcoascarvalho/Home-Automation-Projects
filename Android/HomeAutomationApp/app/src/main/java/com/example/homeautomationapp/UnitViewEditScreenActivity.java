package com.example.homeautomationapp;

import android.app.ListActivity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.SimpleCursorAdapter;

import com.example.database.DatabaseOpenHelper;

/**
 * Created by marco on 22/01/2017.
 */
public class UnitViewEditScreenActivity extends ListActivity {

    private DatabaseOpenHelper mDbHelper;
    private SimpleCursorAdapter mAdapter;

    private Cursor mDatacursor;
    private String mdataId = "NULL";
    private String mdataUnitName, mdataUnitId, mdataUnitType;

    private Button mUpdateDatabutton;
    private Button meraseButton;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.listactivity_unit_viewedit_screen);

        mUpdateDatabutton = (Button) findViewById(R.id.UpdateDatabutton);
        meraseButton = (Button) findViewById(R.id.EraseDatabutton);

        mUpdateDatabutton.setEnabled(false);
        meraseButton.setEnabled(false);

        mDbHelper = new DatabaseOpenHelper(this);

        // Create a cursor
        Cursor c = readInsertedUnits();

        mAdapter = new SimpleCursorAdapter(this, R.layout.listactivity_unitlist_layout, c,
                DatabaseOpenHelper.columns, new int[] { R.id._id, R.id.unit_name, R.id.unit_id, R.id.unit_type },
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


        /* just update the item based on litview click */
        /*
        getListView().setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {

                getListView().setSelection(position);

                Cursor datacursor = mAdapter.getCursor();
                String dataId = datacursor.getString(0); // _id
                String dataUnitName = datacursor.getString(1); // unit_name
                String dataUnitId = datacursor.getString(2); // unit_id
                String dataUnitType = datacursor.getString(3); // unit_type

                Intent data = new Intent();
                data.putExtra("HouseDataId", dataId);
                data.putExtra("HouseDataUnitName", dataUnitName);
                data.putExtra("HouseDataUnitId", dataUnitId);
                data.putExtra("HouseDataUnitType", dataUnitId);

                Log.i("dataId", dataId);
                Log.i("dataUnitName", dataUnitName);
                Log.i("dataUnitId", dataUnitId);
                Log.i("dataUnitType", dataUnitType);

                // return data Intent and finish
                setResult(RESULT_OK, data);
                finish();

            }
        });
        */

        getListView().setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {

                 //getListView().setChoiceMode(getListView().CHOICE_MODE_SINGLE);
                getListView().setSelection(position);
                //getListView().setItemChecked(position,true);
                //view.setSelected(true);

                mDatacursor = mAdapter.getCursor();
                Log.i("datacursor.UnitName1", mDatacursor.getString(1));

                mdataId = mDatacursor.getString(0); // _id
                mdataUnitName = mDatacursor.getString(1); // unit_name
                mdataUnitId = mDatacursor.getString(2); // unit_id
                mdataUnitType = mDatacursor.getString(3); // unit_type

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



        mUpdateDatabutton = (Button) findViewById(R.id.UpdateDatabutton);
        mUpdateDatabutton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {

                //Cursor datacursor = mAdapter.getCursor();
                //String dataId = mDatacursor.getString(0); // _id
                //String dataUnitName = mDatacursor.getString(1); // unit_name
                //String dataUnitId = mDatacursor.getString(2); // unit_id
                //String dataUnitType = mDatacursor.getString(3); // unit_type

                Intent data = new Intent();

                checkNullData();

                data.putExtra("HouseDataId", mdataId);
                data.putExtra("HouseDataUnitName", mdataUnitName);
                data.putExtra("HouseDataUnitId", mdataUnitId);
                data.putExtra("HouseDataUnitType", mdataUnitId);

                Log.i("dataId", mdataId);
                Log.i("dataUnitName", mdataUnitName);
                Log.i("dataUnitId", mdataUnitId);
                Log.i("dataUnitType", mdataUnitType);

                // return data Intent and finish
                setResult(RESULT_OK, data);
                finish();


            }
        });



        meraseButton = (Button) findViewById(R.id.EraseDatabutton);
        meraseButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {

                //String dataId = mDatacursor.getString(0); // _id

                checkNullData();

                // Erase data
                mDbHelper.getWritableDatabase().delete(DatabaseOpenHelper.TABLE_NAME, DatabaseOpenHelper._ID + "=?", new String[] { mdataId });

                // Redisplay data
                mAdapter.getCursor().requery();
                mAdapter.notifyDataSetChanged();
            }
        });


    }
        // Returns all artist records in the database
    private Cursor readInsertedUnits() {

        Log.i("Debug", "readInsertedUnits()");

        return mDbHelper.getWritableDatabase().query(DatabaseOpenHelper.TABLE_NAME,
                DatabaseOpenHelper.columns, null, new String[] {}, null, null,
                null);

    }

    // Delete all records
    private void clearAll() {

        mDbHelper.getWritableDatabase().delete(DatabaseOpenHelper.TABLE_NAME, null, null);

    }

    private void checkNullData() {

        if(mdataId.equals("NULL")){
            mDatacursor = mAdapter.getCursor();

            mdataId = mDatacursor.getString(0); // _id
            mdataUnitName = mDatacursor.getString(1); // unit_name
            mdataUnitId = mDatacursor.getString(2); // unit_id
            mdataUnitType = mDatacursor.getString(3); // unit_type

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

        mDbHelper.getWritableDatabase().close();
        //mDbHelper.deleteDatabase();

        super.onDestroy();

    }


}


