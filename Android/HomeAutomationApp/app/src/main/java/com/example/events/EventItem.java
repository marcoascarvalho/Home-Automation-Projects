package com.example.events;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import android.content.Intent;

public class EventItem {

	//public static final String ITEM_SEP = System.getProperty("line.separator");

	public enum UnitStatus {
		On, Off
	};

	/*
	public final static String UNIT_ID = "unit_id";
	public final static String UNIT_STATUS = "unit_status";
	public final static String DATE_ON = "date_on";
	public final static String DATE_OFF = "date_off";
	public final static String TIME_ON = "time_on";
	public final static String TIME_OFF = "time_off";

	public final static SimpleDateFormat FORMAT = new SimpleDateFormat(
			"yyyy-MM-dd HH:mm:ss", Locale.US);
	*/

	public String mUnitId ="0";
	public String mUnitName = " ";
	//private UnitStatus mUnitStatus = UnitStatus.Off;
	//public Date mDateOn = new Date();
	//public Date mDateOff = new Date();

	public int mMonthOn;
	public int mDayOn;
	public int mHourOn;
	public int mMinOn;

	public int mMonthOff;
	public int mDayOff;
	public int mHourOff;
	public int mMinOff;

	/*
	EventItem(String unitId, Date dateOn, Date dateOff) {
		this.mUnitId = unitId;
		this.mDateOn = dateOn;
		this.mDateOff = dateOff;
	}*/

	// Create a new ToDoItem from data packaged in an Intent
	/*
	EventItem(Intent intent) {

		mUnitId = intent.getStringExtra(EventItem.UNIT_ID);

		try {
			mDateOn = EventItem.FORMAT.parse(intent.getStringExtra(EventItem.DATE_ON));
		} catch (ParseException e) {
			mDateOn = new Date();
		}

		try {
			mDateOff = EventItem.FORMAT.parse(intent.getStringExtra(EventItem.DATE_OFF));
		} catch (ParseException e) {
			mDateOff = new Date();
		}
	}*/

	public String getUnitId() {

		return mUnitId;
	}

	public void setUnitId(String unitId) {

		mUnitId = unitId;
	}

	/*
	public UnitStatus getUnitStatus() {
		return mUnitStatus;
	}*/

	/*
	public void setStatus(UnitStatus status) {
		mUnitStatus = status;
	}*/
	/*
	public Date getDate(Date DateOn_or_Off) {
		return DateOn_or_Off;
	}

	public Date setDate(Date DateOn_or_Off) {
		return DateOn_or_Off;
	}*/

	// Take a set of String data values and 
	// package them for transport in an Intent
	/*
	public static void packageIntent(Intent intent, String title,
			String dateOn, String dateOff) {

		intent.putExtra(EventItem.UNIT_ID, title);
		intent.putExtra(EventItem.DATE_ON, dateOn);
		intent.putExtra(EventItem.DATE_OFF, dateOff);
	
	}

	public String toString() {
		return mUnitId + ITEM_SEP + FORMAT.format(mDateOn) + ITEM_SEP
				+ FORMAT.format(mDateOff);
	}

	public String toLog() {
		return "UnitId:" + mUnitId + ITEM_SEP + "DateOn:" + FORMAT.format(mDateOn) + ITEM_SEP + "DateOff:"
				+ FORMAT.format(mDateOff) + "\n";
	}*/

}
