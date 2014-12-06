import gnu.io.*;
import java.io.*;
import java.util.*;

public class SerialMonitor implements SerialPortEventListener
{
	public interface SerialListener
	{
		public void recieveSerialData( String line );
	}
	
	SerialPort serialPort;
    
    /** The port we're normally going to use. */
	private static final String PORT_NAMES[] = { 
		"/dev/tty.usbserial-A700e0q1", // Mac OS X
		"/dev/ttyACM0", // Raspberry Pi
		"/dev/ttyUSB0", // Linux
		"COM3", // Windows
	};

	private BufferedReader input;
	private OutputStream output;
	private static final int TIME_OUT = 2000;
	private static final int DATA_RATE = 9600;
	
	// make this an array or something
	private SerialListener listener;

	public void initialize( SerialListener listener )
	{
		// the next line is for Raspberry Pi and 
		// gets us into the while loop and was suggested here was suggested http://www.raspberrypi.org/phpBB3/viewtopic.php?f=81&t=32186
		//System.setProperty( "gnu.io.rxtx.SerialPorts", "/dev/ttyACM0" );

		CommPortIdentifier portId = null;
		Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

		while( portEnum.hasMoreElements() )
		{
			CommPortIdentifier currPortId = (CommPortIdentifier)portEnum.nextElement();
			for( String portName : PORT_NAMES )
			{
				if( currPortId.getName().equals( portName ) )
				{
					portId = currPortId;
					break;
				}
			}
		}
		
		if( portId == null )
		{
			System.out.println( "Could not find COM port." );
			return;
		}

		try
		{
			serialPort = (SerialPort)portId.open( this.getClass().getName(), TIME_OUT );
			serialPort.setSerialPortParams( DATA_RATE, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE );
			input = new BufferedReader( new InputStreamReader( serialPort.getInputStream() ) );
			output = serialPort.getOutputStream();

			serialPort.addEventListener( this );
			serialPort.notifyOnDataAvailable( true );
		}
		catch( Exception e )
		{
			System.err.println( e.toString() );
		}
		this.listener = listener;
	}

	public synchronized void close()
	{
		if( serialPort != null )
		{
			serialPort.removeEventListener();
			serialPort.close();
		}
	}

	public synchronized void serialEvent(SerialPortEvent oEvent)
	{
		if( oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE )
		{
			try
			{
				String inputLine = input.readLine();
				listener.recieveSerialData( inputLine );
			}
			catch( Exception e )
			{
				System.err.println( e.toString() );
			}
		}
	}
}