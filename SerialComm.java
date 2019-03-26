import jssc.*;

public class SerialComm {

	SerialPort port;

	private boolean debug;  // Indicator of "debugging mode"
	
	// This function can be called to enable or disable "debugging mode"
	void setDebug(boolean mode) {
		debug = mode;
	}	
	

	// Constructor for the SerialComm class
	public SerialComm(String name) throws SerialPortException {
		port = new SerialPort(name);		
		port.openPort();
		port.setParams(SerialPort.BAUDRATE_9600,
			SerialPort.DATABITS_8,
			SerialPort.STOPBITS_1,
			SerialPort.PARITY_NONE);
		
		debug = false; // Default is to NOT be in debug mode
	}
		
	public String toHex(byte b) {
		String ff; 
		String sf;
		byte f =  (byte)((byte)((b >> 4)) & (byte)15);
		if((int)f<10) {
			ff = Integer.toString((int)f);
		}
		else {
			ff= String.valueOf((char)((int)(f+87)));
		}
		byte s = (byte)  ( b & (byte)15);
		if((int)s<10) {
			sf = Integer.toString((int)s);
		}
		else {
			sf= String.valueOf((char)((int)(s+87)));
		}
		return "<0x"+ff+sf+">";
	}
	
	// TODO: Add writeByte() method 
	public void writeByte(byte b) {
		int d= (int)b;
		if (debug) {
			try {
				port.writeByte(b);
			} catch (SerialPortException e) {
				// TODO Auto-generated catch block
				System.out.println("Could not write");
				e.printStackTrace();
			}
			System.out.println(this.toHex(b));
		}
		else {
			try {
				port.writeByte(b);
			} catch (SerialPortException e) {
				// TODO Auto-generated catch block
				System.out.println("Could not write");
				e.printStackTrace();
			} 
		}
	}
	// TODO: Add available() method
	public boolean available() throws SerialPortException {
		int x = port.getInputBufferBytesCount(); 
		if (x > 0) {
			return true; 
		}
		else {
			return false; 
		}
	}
	// TODO: Add readByte() method	
	public byte readByte() throws SerialPortException {
		byte[] array = port.readBytes(1); 
		byte x = array[0]; 
		if (debug) {
			System.out.println(toHex(x));
		}
		return x;
	}
	// TODO: Add a main() method
	static void main() {
		
	}
}
