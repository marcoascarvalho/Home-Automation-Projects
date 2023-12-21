package com.example.communication;

import android.os.AsyncTask;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

/**
 * Created by Marco on 25/06/2016.
 */
public class CommunicationManager extends AsyncTask<String, Void, Boolean> {

    private Socket socket;
    private PrintWriter writer;
    private OnDataListener listener;
    private static CommunicationManager instance;

    private CommunicationManager()  { }

    @Override
    protected Boolean doInBackground(String... strings) {

        return connect(strings[0], Integer.parseInt(strings[1]));
    }

    public static CommunicationManager getInstance()
    {
        if(instance == null)
        {
            instance = new CommunicationManager();
        }

        return instance;
    }

    public boolean connect(final String serverIp, final int serverPort)
    {
        try
        {
            InetAddress serverAddr = InetAddress.getByName(serverIp);
            socket = new Socket(serverAddr, serverPort);
            writer = new PrintWriter(socket.getOutputStream());
        }
        catch (Exception e)
        {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public void sendData(final char[] data)
    {
        try
        {
            writer.print(data);
            writer.flush();
        }
        catch (Exception ex)
        {
            System.err.println(ex);
        }
    }

    public void receiveData()
    {
        Thread t = new Thread(new Runnable() {
            @Override
            public void run()
            {
                while(true) {
                    try {
                        final byte[] buffer = new byte[1024];
                        final InputStream inputStream = socket.getInputStream();

                        int bytesRead;
                        while ((bytesRead = inputStream.read(buffer)) != -1) {
                            //  byteArrayOutputStream.write(buffer, 0, bytesRead);
                            if (listener != null) {
                                listener.onDataReceived(buffer, bytesRead);
                            }
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        t.start();


    }

    public void addDataListener(OnDataListener listener)
    {
        this.listener = listener;
    }
}
