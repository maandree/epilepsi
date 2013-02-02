package test;
import se.kth.maandree.epilepsi.security.*;

public class ChecksumTest
{
    public static void main(final String... args) throws Throwable
    {
	final Checksum md5 = new Checksum("md5");
	final Checksum sha1 = new Checksum("sha1");
	final Checksum sha2_256 = new Checksum("sha2-256");
	final Checksum sha2_384 = new Checksum("sha2-384");
	final Checksum sha2_512 = new Checksum("sha2-512");
	
	print(3, md5.calculate(""));
	print(2, md5.calculate(""));
	print(3, md5.calculate("epilepsi"));
	print(2, md5.calculate("epilepsi"));
	print(3, md5.calculate("epilepsi\n"));
	System.out.println("\033[32m7e110f8b51025c007e9c45a20e3d229f\033[00m");
	print(3, sha1.calculate("epilepsi\n"));
	System.out.println("\033[32m97ceefb96c3de1a9b28a0f3af15c9411584d32fd\033[00m");
	print(3, sha2_256.calculate("epilepsi\n"));
	System.out.println("\033[32m50f2da86bd2ac6d27af07d54817257fdfc157b5fae46214f7a11cc69550810b6\033[00m");
	print(3, sha2_384.calculate("epilepsi\n"));
	System.out.println("\033[32ma51eecfb7d1d0df8b511b150803ca7ff9928e2f4efbaa5b623f7ab4a5e934d12fc247e00ee912c301dd908eb3994989c\033[00m");
	print(3, sha2_512.calculate("epilepsi\n"));
	System.out.println("\033[32m87711ca37291050551299376722e84914db704fe32cccaaf5a54098f21f2fb802364ea295939a75e5e94adece9082edb54391da9c79237747f0ee5aa0e60500a\033[00m");
    }
    
    public static void print(int colour, byte[] bs)
    {
	System.out.print("\033[3" + colour + "m");
	for (final byte b : bs)
	{   System.out.print("0123456789abcdef".charAt((b & 255) >> 4));
	    System.out.print("0123456789abcdef".charAt(b & 15));
	}
	System.out.println("\033[00m");
    }
    
}

