package com.example.database;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DatabaseOpenHelper extends SQLiteOpenHelper {

	public final static String TABLE_NAME = "house_units";
	public final static String _ID = "_id";
	public final static String UNIT_NAME = "name";
	public final static String UNIT_ID = "id";
	public final static String UNIT_TYPE = "type";
	public final static String[] columns = { _ID, UNIT_NAME, UNIT_ID, UNIT_TYPE };


	final private static String CREATE_CMD =

	"CREATE TABLE house_units (" + _ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
			+ UNIT_NAME + " TEXT NOT NULL, "
			+ UNIT_ID + " TEXT NOT NULL, "
			+ UNIT_TYPE + " TEXT NOT NULL)";



	/*
	final private static String CREATE_CMD =

			"CREATE TABLE house_units (" + _ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
					+ UNIT_NAME + " TEXT NOT NULL)";
	//				+ UNIT_ID + " TEXT NOT NULL"
	//				+ UNIT_TYPE + " TEXT NOT NULL)";

*/

	final private static String NAME = "units_db";
	final private static Integer VERSION = 1;
	final private Context mContext;

	public DatabaseOpenHelper(Context context) {
		super(context, NAME, null, VERSION);
		this.mContext = context;
	}

	@Override
	public void onCreate(SQLiteDatabase db) {
		db.execSQL(CREATE_CMD);
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		// N/A
	}

	public void deleteDatabase() {
		mContext.deleteDatabase(NAME);
	}
}
