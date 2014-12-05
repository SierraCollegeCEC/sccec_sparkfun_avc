import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;
import java.awt.event.*;

public class DataAxisPanel extends JPanel
{
	protected MPU9150Data data;
	protected int[] range;
	protected float pivot;
	protected float scale;
	protected String label;
	private Color[] colors = { Color.RED, Color.GREEN, Color.CYAN };

	public DataAxisPanel( MPU9150Data data, String label, int[] range, float pivot, float scale )
	{
		this.data = data;
		this.label = label;
		this.range = range;
		this.pivot = pivot;
		this.scale = scale;
	}

	// Appearance configuration
	private static final Color axisColor  = Color.GRAY;
	private static final Color axisColor2 = new Color( 0x222222 );
	private static final Color plotColor  = Color.RED;
	private static final Color avgColor   = new Color( 0x550000 );
	private static final Color bgColor    = Color.BLACK;
	private static final int axisOffset = 30;
	private static final int margin = 5;
	private static final int tickSpacing = 15;
	private static final int tickSize = 3;

	public void paintComponent( Graphics graphics )
	{
		Graphics2D g = (Graphics2D)graphics;
		setBackground( bgColor );
		super.paintComponent( g );
		
		int h = this.getSize().height;
		int w = this.getSize().width;
		int yOrigin = h/2;
	
		// Horizontal axis
		int tk = (w-axisOffset-margin)/data.data[0].length;
		for( int i = 0; i < w - margin - axisOffset; i += tk )
		{
			g.setColor( axisColor2 );
			g.drawLine( axisOffset + i, margin, axisOffset + i, h - margin );
			g.setColor( axisColor );
			g.drawLine( axisOffset + i, yOrigin - tickSize, axisOffset + i, yOrigin + tickSize );
		}
		
		// Vertical axis
		for( int i = 0; i < h/2 - margin; i += tickSpacing )
		{
			g.setColor( axisColor2 );
			g.drawLine( axisOffset, yOrigin + i, w - margin, yOrigin + i );
			g.drawLine( axisOffset, yOrigin - i, w - margin, yOrigin - i );
			g.setColor( axisColor );
			g.drawLine( axisOffset, yOrigin + i, axisOffset + tickSize, yOrigin + i );
			g.drawLine( axisOffset, yOrigin - i, axisOffset + tickSize, yOrigin - i );
		}
		g.setColor( axisColor );
		g.drawLine( axisOffset, margin, axisOffset, h - margin );
		g.drawLine( axisOffset, yOrigin, w - margin, yOrigin );
		
		AffineTransform tform = g.getTransform();
		g.rotate( -Math.PI/2 );
		g.drawString( label, -h/2, axisOffset/2 );
		g.setTransform( tform );
		
		for( int i = 0; i < range.length; ++i )
		{
			plotAxis( g, data.data[range[i]], pivot, scale, false, colors[i] );
		}
	}

	private static float adjust( float value, float pivot, float scale )
	{
		return (value - pivot) * scale;
	}

	private void plotAxis( Graphics g, float[] data, float pivot, float scale, boolean bAvg, Color color )
	{
		int h = this.getSize().height;
		int w = this.getSize().width;
	
		int yOrigin = h/2;
		int xOffset = (w-axisOffset-margin)/data.length;
		int yValue = yOrigin;
		int xValue = axisOffset;
		float lastVal = 0;
	
		float sum = 0;
		g.setColor( color );
		for( float Val : data )
		{
			if( Val == 0 )
			{
				sum += pivot;
			}
			else
			{
				sum += Val;
			}
		
			if( lastVal == 0 )
			{
				lastVal = Val;
				continue;
			}
		
			int t0 = (int)adjust( lastVal, pivot, scale );
			int t1 = (int)Val;
			if( Val != 0 )
			{
				t1 = (int)adjust( Val, pivot, scale );
			}
			g.drawLine( xValue, h - (yOrigin + t0), xValue + xOffset, h - (yOrigin + t1) );
			xValue += xOffset;
			lastVal = Val;
		}
	
		if( bAvg )
		{
			float avg = (sum / 100);
			int avgY = h - (yOrigin + (int)adjust( avg, pivot, scale ));
			g.setColor( avgColor );
			g.drawLine( 0, avgY, w, avgY );
		}
	}
}