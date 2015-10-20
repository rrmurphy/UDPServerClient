import java.net.*;

public class ServerUDP {
    
    private static final int PORT_NO_INDEX = 0;
    
    private static final int OPERATION_INDEX = 2;
    
    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Usage: server port");
            System.exit(1);
        }
        int portNo = Integer.parseInt(args[PORT_NO_INDEX]);
        byte[] buffer = new byte[256];
        try {
            DatagramSocket socket = new DatagramSocket(portNo);
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
            for (;;) {
                socket.receive(packet);
                
                switch (buffer[OPERATION_INDEX]) {
                    case 80: // Disemvoweling
                        // System.out.println("Disemvoweling");
                        disemvowel(buffer);
                        break;
                    case 10: // Uppercasing
                        // System.out.println("Uppercasing");
                        uppercase(buffer);
                        break;
                    case 5: // cLength
                        // System.out.println("cLength");
                        cLength(buffer);
                        break;
                }
                
                InetAddress ipAddress = packet.getAddress();
                int port = packet.getPort();
                
                DatagramPacket sendPacket = new DatagramPacket(buffer, buffer[0], 
                        ipAddress, port);
                socket.send(sendPacket);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    private static void disemvowel(byte[] buffer) {
        String s = getString(buffer);
        toResponseString(buffer, s.replaceAll("[AEIOUaeiou]", ""));
    }
    
    private static void uppercase(byte[] buffer) {
        String s = getString(buffer);
        toResponseString(buffer, s.toUpperCase());
    }
    
    private static void cLength(byte[] buffer) {
        String s = getString(buffer).toUpperCase();
        int originalLength = s.length();
        s = s.replaceAll("[BCDFGHJKLMNPQRSTVWXYZ]", "");
        buffer[0] = (byte) 3; 
        buffer[2] = (byte) (originalLength - s.length());
    }
    
    private static String getString(byte[] request) {
        int length = request[0];
        char[] array = new char[length];
        for (int i = 0; i < length; i++) {
            array[i] = (char) request[i + 3];
        }
        return new String(array);
    }

    private static void toResponseString(byte[] buffer, String data) {
        char[] chars = data.toCharArray();
        
        buffer[0] = (byte) (data.length() + 2);
        
        for (int i = 0; i < chars.length; i++) {
            buffer[i + 2] = (byte) chars[i];
        }
    }

}
