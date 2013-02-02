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

import java.io.*;
import java.security.MessageDigest;


/**
 * MD5 hash algorithm class
 * 
 * @author  Mattias Andrée (maandree@member.fsf.org)
 */
class MD5 extends Hash
{
    /**
     * Buffer size
     */
    private final int BUF_SIZE = 1024;
    
    
    
    // Has default constructor
    
    
    
    /**
     * The message digester
     */
    private MessageDigest digest = MessageDigest.getInstance("MD5");
    
    /**
     * Stream read buffer
     */
    private byte[] buf = new byte[BUF_SIZE];
    
    
    
    /**
     * Calculate the checksum of a message
     * 
     * @param   message  The message
     * @return           The checksum
     */
    public byte[] calculate(final byte[] message)
    {
	try
	{   this.digest.digest(message);
	}
	finally
	{   this.digest.reset();
	}
    }
    
    /**
     * Calculate the checksum of a message
     * 
     * @param   message  The message
     * @return           The checksum
     */
    public byte[] calculate(final InputStream message)
    {
	try
	{   for (int n;;)
		if ((n = message.read(this.buf, 0, BUF_SIZE)) > 0)
		    this.digest.update(this.buf, 0, n);
		else
		    return this.digest.digest();
	}
	finally
	{   digest.reset();
	}
    }
    
}

