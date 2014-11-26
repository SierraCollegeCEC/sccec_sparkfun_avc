import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;
import java.io.*;
import gnu.io.*;
import java.awt.event.*;
import java.util.ArrayList;

public class MPU9150Mon implements SerialMonitor.SerialListener
{
	private MPU9150Data data;
	private static final int SAMPLE_COUNT = 200;
	private int sampleIndex;

	private ArrayList<DataAxisPanel> graphs;
	private SerialMonitor ser;
	
	private Timer timer;
	private ActionListener timerAction = new ActionListener()
	{
		@Override
		public void actionPerformed(ActionEvent ae)
		{
			for( DataAxisPanel graph : graphs )
			{
				graph.repaint();
			}
		}
	};
	
	public MPU9150Mon()
	{
		sampleIndex = 0;
		
		ser = new SerialMonitor();
		ser.initialize( this );
		
		graphs = new ArrayList<DataAxisPanel>();
		data = new MPU9150Data( SAMPLE_COUNT );
	}
	
	public void recieveSerialData( String line )
	{
		//System.out.println( line );
		String[] tokens = line.split( "\\s+" );
			
		data.data[MPU9150Data.TEMP][sampleIndex] = Float.parseFloat( tokens[0] );
		data.data[MPU9150Data.CMPX][sampleIndex] = Float.parseFloat( tokens[1] );
		data.data[MPU9150Data.CMPY][sampleIndex] = Float.parseFloat( tokens[2] );
		data.data[MPU9150Data.CMPZ][sampleIndex] = Float.parseFloat( tokens[3] );
		data.data[MPU9150Data.GYRX][sampleIndex] = Float.parseFloat( tokens[4] );
		data.data[MPU9150Data.GYRY][sampleIndex] = Float.parseFloat( tokens[5] );
		data.data[MPU9150Data.GYRZ][sampleIndex] = Float.parseFloat( tokens[6] );
		data.data[MPU9150Data.ACCX][sampleIndex] = Float.parseFloat( tokens[7] );
		data.data[MPU9150Data.ACCY][sampleIndex] = Float.parseFloat( tokens[8] );
		data.data[MPU9150Data.ACCZ][sampleIndex] = Float.parseFloat( tokens[9] );

		sampleIndex++;
		if( sampleIndex >= SAMPLE_COUNT )
		{
			sampleIndex = 0;
		}
		//System.out.println( Arrays.toString(temps) );
	}
	
	public void start()
	{
		JPanel main = new JPanel();
		main.setLayout( new BoxLayout( main, BoxLayout.PAGE_AXIS ) );
		
		graphs.add( new DataAxisPanel( data,
			"Accelerometer",
			new int[]{ MPU9150Data.ACCX, MPU9150Data.ACCY, MPU9150Data.ACCZ },
			0, 0.005f ) );
		graphs.add( new DataAxisPanel( data, 
			"Gyroscope",
			new int[]{ MPU9150Data.GYRX, MPU9150Data.GYRY, MPU9150Data.GYRZ },
			0, 0.002f ) );
		
		graphs.add( new DataAxisPanel( data, 
			"Compass",
			new int[]{ MPU9150Data.CMPX, MPU9150Data.CMPY, MPU9150Data.CMPZ },
			0, 0.1f ) );
		
		/*
		graphs.add( new DataAxisPanel( data, 
			"Temperature",
			new int[]{ MPU9150Data.TEMP },
			32, 100.0f ) );
		*/
		
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
		
		for( DataAxisPanel panel : graphs )
		{
			main.add( panel );
		}
		
		frame.add( main );
		frame.setSize( 1000, 720 );
		frame.setVisible( true );
		
		timer = new Timer( 5, timerAction );
		timer.start();
		
		System.out.println( "Started" );
	}
	
	public static void main( String[] args )
	{
		final MPU9150Mon mon = new MPU9150Mon();
		SwingUtilities.invokeLater( new Runnable()
        {
            @Override
            public void run()
            {
				mon.start();
            }
        } );
	}
}