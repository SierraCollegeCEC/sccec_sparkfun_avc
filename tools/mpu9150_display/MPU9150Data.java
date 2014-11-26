public class MPU9150Data
{
	public static final int TEMP = 0;
	public static final int ACCX = 1;
	public static final int ACCY = 2;
	public static final int ACCZ = 3;
	public static final int GYRX = 4;
	public static final int GYRY = 5;
	public static final int GYRZ = 6;
	public static final int CMPX = 7;
	public static final int CMPY = 8;
	public static final int CMPZ = 9;
	
	public float[][] data;
	
	public MPU9150Data( int count )
	{
		data = new float[10][count];
	}
}