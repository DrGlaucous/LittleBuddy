/**
 * 
 */
package com.csr460.iSkipper.device;

import java.util.ArrayDeque;
import java.util.Queue;

import com.csr460.iSkipper.handler.ReceivedPacketHandlerInterface;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;

/**
 * This class keeps buffering the data from serial port, and sends
 * ReceivedPacketEvent when receives a complete response.
 * 
 * @see <a href=
 *      "http://fazecast.github.io/jSerialComm/javadoc/com/fazecast/jSerialComm/SerialPortDataListener.html"
 *      >com.fazecast.jSerialComm.SerialPortDataListener</a>
 *
 * @author Charles Cao (CSR)
 *
 */

public class SerialListener implements SerialPortDataListener
{

	private static final int SERIAL_INITIAL_BUFFER_SIZE = 256;
	private static final int SERIAL_INITIAL_PACKET_QUEUE_SIZE = 8;
	private Queue<Byte> recieveBuffer;
	private Queue<ReceivedPacketEvent> unhandledPackets;
	private Thread eventSender;
	private int numBytesReceived;
	ReceivedPacketHandlerInterface packetHandler;

	public SerialListener(ReceivedPacketHandlerInterface eventHandler)
	{
		recieveBuffer = new ArrayDeque<>(SERIAL_INITIAL_BUFFER_SIZE);
		unhandledPackets = new ArrayDeque<>(SERIAL_INITIAL_PACKET_QUEUE_SIZE);
		this.packetHandler = eventHandler;
		numBytesReceived = 0;
		eventSender = new Thread()
		{
			// Anonymous inner class
			@Override
			public void run()
			{
				while (true)
				{
					while (!unhandledPackets.isEmpty())
					{
						packetHandler.onReceivedPacketEvent(unhandledPackets.poll());
					}
					synchronized (this)
					{
						try
						{
							this.wait();
						} catch (InterruptedException e)
						{
							currentThread().interrupt();
						}
					}

				}
			}
		};
		eventSender.start();
	}

	/**
	 * @see <a href=
	 *      "http://fazecast.github.io/jSerialComm/javadoc/com/fazecast/jSerialComm/SerialPortDataListener.html#getListeningEvents--"
	 *      >com.fazecast.jSerialComm.SerialPortDataListener#getListeningEvents</a>
	 */
	@Override
	public int getListeningEvents()
	{
		return SerialPort.LISTENING_EVENT_DATA_RECEIVED;
	}

	/**
	 * 
	 * The method that invoked by the object of SerialPort when data were received
	 * on the port.</br>
	 * 
	 * It keeps buffering received data and waiting for a complete response(which is
	 * the ASCII string ended with '\n' or '\0'). </br>
	 * 
	 * After receiving a response, it will send a ReceivedPacketEvent to the Handler
	 * and invoke onReceivedPacketEvent() method.
	 * 
	 */
	@Override
	public void serialEvent(SerialPortEvent event)
	{
		byte[] data = event.getReceivedData();
		for (byte b : data)
		{
			recieveBuffer.offer(b);
			numBytesReceived++;
			if (b == '\0' || b == '\n')
			{
				byte[] packet = new byte[numBytesReceived];
				for (int i = 0; i < numBytesReceived; i++)
				{
					packet[i] = recieveBuffer.poll();
				}
				numBytesReceived = 0;
				unhandledPackets.offer(new ReceivedPacketEvent(this, packet));
				synchronized (eventSender)
				{
					eventSender.notifyAll();
				}
			}
		}
	}

}
