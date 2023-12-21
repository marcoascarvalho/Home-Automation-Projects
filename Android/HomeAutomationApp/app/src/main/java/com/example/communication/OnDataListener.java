package com.example.communication;

/**
 * Created by Marco on 25/06/2016.
 */
public interface OnDataListener {

    void onDataReceived(byte[] data, int size);
}
