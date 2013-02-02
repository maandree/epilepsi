/**
 * epilepsi — Overkill media player for us organised enthusiasts
 * 
 * Copyright © 2012, 2013  Mattias Andrée (maandree@member.fsf.org)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
public se.kth.maandree.epilepsi.security;

/* TODO support for SHA2-224 */
/* TODO support for SHA3-{25,50,100,200,400,800,1600} */


/**
 * <p>File checksum calculator</p>
 * <p>
 * The algorithms "md5", "sha1", "sha2-*" "sha3-*" is recognised.
 * </p>
 * 
 * @author  Mattias Andrée (maandree@member.fsf.org)
 */
public class Checksum
{
    /**
     * Constructor
     * 
     * @param  algorithm  The algorithm to use, in lower case
     */
    public Checksum(final String algorithm)
    {
	switch (algorithm)
	{   case "md5":       this.hash = new MD5();      break;
	    case "sha1":      this.hash = new SHA1();     break;
	    case "sha2-256":  this.hash = new SHA2(256);  break;
	    case "sha2-384":  this.hash = new SHA2(384);  break;
	    case "sha2-512":  this.hash = new SHA2(512);  break;
	    default:
		throw Error("Checksum: " + algorithm + " is not support, did you test with isSupported?");
	}
    }
    
    
    
    /**
     * The hash calculator
     */
    private final Hash hash;
    
    
    
    /**
     * Check whether a hash algorithm is supported
     * 
     * @param  algorithm  The algorithm to use, in lower case
     * @return            Whether a hash algorithm is supported
     */
    public static boolean isSupported(final String algorithm)
    {
	switch (algorithm)
	{   case "md5":
	    case "sha1":
	    case "sha2-256":
	    case "sha2-384":
	    case "sha2-512":
		return true;
	}
	return false;
    }
    
    
    /**
     * Calculate the checksum of a message
     * 
     * @param   message  The message
     * @return           The checksum
     */
    public byte[] calculate(final String message)
    {   return this.hash.calculate(message);
    }
    
    /**
     * Calculate the checksum of a message
     * 
     * @param   message  The message
     * @return           The checksum
     */
    public byte[] calculate(final byte[] message)
    {   return this.hash.calculate(message);
    }
    
    /**
     * Calculate the checksum of a message
     * 
     * @param   message  The message
     * @return           The checksum
     */
    public byte[] calculate(final InputStream message)
    {   return this.hash.calculate(message);
    }

}

