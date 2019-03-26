import java.awt.event.KeyEvent;
import jssc.SerialPortException;

public class PCPlayer{

	final private SerialComm port;
	long time = System.nanoTime();
	//one second 
	long deltaTime = 110000000; 
	
	public PCPlayer(String portname) throws SerialPortException {
		port = new SerialComm(portname);
		
			
	}
	public void run() {
		while (true) {
			if (System.nanoTime()>time) {
				if (StdDraw.isKeyPressed(KeyEvent.VK_UP)) {
					System.out.println("up");
					int input = 'w';
					port.writeByte((byte)input);
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_DOWN)) {
					System.out.println("down");
					int input = 's';
					port.writeByte((byte)input);
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_RIGHT)) {
					System.out.println("right");
					int input = 'd';
					port.writeByte((byte)input);
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_LEFT)) {
					System.out.println("left");
					int input = 'a';
					port.writeByte((byte)input);
				}
				time += deltaTime; 
			}
			
		}
		
		
	}
	public static void main(String[] args) throws SerialPortException {
		PCPlayer player = new PCPlayer("/dev/cu.usbserial-DN0513FV");
		player.run(); 
	
	}
}