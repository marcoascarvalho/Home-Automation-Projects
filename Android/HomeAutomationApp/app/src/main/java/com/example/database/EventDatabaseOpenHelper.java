package com.example.database;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class EventDatabaseOpenHelper extends SQLiteOpenHelper {

	public final static String TABLE_NAME = "events";
	public final static String _ID = "_id";
	public final static String UNIT_NAME = "name";
	public final static String DATE_ON = "dateON";
	public final static String DATE_OFF = "dateOFF";

	//| ONMonth |  ONDay  |  ONHour  |  ONMinute  |  OffMonth |  OffDay  |  OffHour  |  OffMinute
	public final static String[] columns = { _ID, UNIT_NAME, DATE_ON, DATE_OFF, };


	final private static String CREATE_CMD =

	"CREATE TABLE events (" + _ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
			+ UNIT_NAME + " TEXT NOT NULL, "
			+ DATE_ON + " TEXT NOT NULL, "
			+ DATE_OFF + " TEXT NOT NULL)";

	/*
	final private static String CREATE_CMD =

			"CREATE TABLE house_units (" + _ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
					+ UNIT_NAME + " TEXT NOT NULL)";
	//				+ UNIT_ID + " TEXT NOT NULL"
	//				+ UNIT_TYPE + " TEXT NOT NULL)";

*/

	final private static String NAME = "events_db";
	final private static Integer VERSION = 1;
	final private Context mContext;

	public EventDatabaseOpenHelper(Context context) {
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
